#include "MidiController.h"

MidiController::MidiController()
{
    // Start with a moderate transmission rate
    setTransmissionRate(100);
}

MidiController::~MidiController()
{
    disconnect();
}

// ============================================================================
// Device management
// ============================================================================

juce::Array<juce::MidiDeviceInfo> MidiController::getAvailableDevices() const
{
    return juce::MidiOutput::getAvailableDevices();
}

bool MidiController::connectToDevice(const juce::MidiDeviceInfo& device)
{
    disconnect(); // Disconnect from any existing device
    
    try
    {
        midiOutput = juce::MidiOutput::openDevice(device.identifier);
        
        if (midiOutput)
        {
            if (onConnectionChanged)
                onConnectionChanged(true);
            
            return true;
        }
        else
        {
            handleError("Failed to open MIDI device: " + device.name);
            return false;
        }
    }
    catch (const std::exception& e)
    {
        handleError(juce::String("Exception connecting to MIDI device: ") + e.what());
        return false;
    }
}

void MidiController::disconnect()
{
    if (midiOutput)
    {
        stopTimer();
        flushQueue();
        midiOutput.reset();
        
        if (onConnectionChanged)
            onConnectionChanged(false);
    }
}

bool MidiController::isConnected() const
{
    return midiOutput != nullptr;
}

juce::String MidiController::getConnectedDeviceName() const
{
    if (midiOutput)
        return midiOutput->getName();
    
    return juce::String();
}

// ============================================================================
// Message transmission
// ============================================================================

void MidiController::sendNrpn(int channel, int nrpn, int value)
{
    if (!isConnected())
        return;
    
    // Validate inputs
    channel = juce::jlimit(0, 15, channel);
    nrpn = juce::jlimit(0, 0x3FFF, nrpn); // 14-bit NRPN
    value = juce::jlimit(0, 0x7FFF, value); // 14-bit value
    
    QueuedMessage msg;
    msg.channel = channel;
    msg.nrpn = nrpn;
    msg.value = value;
    msg.timestamp = juce::Time::getCurrentTime();
    
    {
        juce::ScopedLock lock(queueLock);
        messageQueue.add(msg);
    }
    
    // Start timer if not already running
    if (!isTimerRunning())
    {
        startTimer(1000 / transmissionRate);
    }
}

void MidiController::sendParameterChange(const juce::String& paramId, float value)
{
    if (!isConnected() || !getMidiChannelForParam || !getNrpnForParam || !convertValueToSam)
        return;
    
    int channel = getMidiChannelForParam(paramId);
    int nrpn = getNrpnForParam(paramId);
    int samValue = convertValueToSam(value, paramId);
    
    if (channel >= 0 && nrpn >= 0 && samValue >= 0)
    {
        sendNrpn(channel, nrpn, samValue);
    }
}

// ============================================================================
// Queue management
// ============================================================================

void MidiController::flushQueue()
{
    juce::ScopedLock lock(queueLock);
    messageQueue.clear();
}

int MidiController::getQueueSize() const
{
    juce::ScopedLock lock(queueLock);
    return messageQueue.size();
}

void MidiController::setTransmissionRate(int messagesPerSecond)
{
    transmissionRate = juce::jlimit(10, 1000, messagesPerSecond);
    
    if (isTimerRunning())
    {
        stopTimer();
        startTimer(1000 / transmissionRate);
    }
}

// ============================================================================
// Timer callback
// ============================================================================

void MidiController::timerCallback()
{
    processQueue();
}

void MidiController::processQueue()
{
    juce::ScopedLock lock(queueLock);
    
    if (messageQueue.isEmpty() || !midiOutput)
    {
        stopTimer();
        return;
    }
    
    // Calculate how many messages to send this tick
    // Based on transmission rate and elapsed time
    int maxMessagesPerTick = transmissionRate / 60; // Assuming 60Hz timer
    
    // Send messages
    int messagesSent = 0;
    for (int i = 0; i < juce::jmin(maxMessagesPerTick, messageQueue.size()); ++i)
    {
        const auto& msg = messageQueue[i];
        sendDreamNrpn(msg.channel, msg.nrpn, msg.value);
        messagesSent++;
    }
    
    // Remove sent messages from queue
    if (messagesSent > 0)
    {
        messageQueue.removeRange(0, messagesSent);
        
        if (onMessagesSent)
            onMessagesSent(messagesSent);
    }
    
    // Stop timer if queue is empty
    if (messageQueue.isEmpty())
    {
        stopTimer();
    }
}

// ============================================================================
// NRPN message construction
// ============================================================================

void MidiController::sendDreamNrpn(int channel, int nrpn, int value)
{
    if (!midiOutput)
        return;
    
    try
    {
        // Send NRPN messages according to Dream DSP protocol
        // CC 99: NRPN MSB
        // CC 98: NRPN LSB
        // CC 38: Value LSB (shifted right by 1)
        // CC 6:  Value MSB
        
        const auto nrpnMsb = (nrpn >> 8) & 0x7f;
        const auto nrpnLsb = nrpn & 0x7f;
        const auto valueMsb = (value >> 8) & 0x7f;
        const auto valueLsb = (value >> 1) & 0x7f;
        
        // Send all four controller messages
        midiOutput->sendMessageNow(juce::MidiMessage::controllerEvent(channel + 1, 99, nrpnMsb));
        midiOutput->sendMessageNow(juce::MidiMessage::controllerEvent(channel + 1, 98, nrpnLsb));
        midiOutput->sendMessageNow(juce::MidiMessage::controllerEvent(channel + 1, 38, valueLsb));
        midiOutput->sendMessageNow(juce::MidiMessage::controllerEvent(channel + 1, 6, valueMsb));
    }
    catch (const std::exception& e)
    {
        handleError(juce::String("Exception sending NRPN message: ") + e.what());
    }
}

juce::MidiMessage MidiController::createNrpnMessage(int channel, int nrpn, int value)
{
    // This is a helper function that creates a single NRPN message
    // Note: Dream DSP protocol requires 4 separate CC messages
    // This function creates the first one (NRPN MSB)
    
    const auto nrpnMsb = (nrpn >> 8) & 0x7f;
    return juce::MidiMessage::controllerEvent(channel + 1, 99, nrpnMsb);
}

// ============================================================================
// Error handling
// ============================================================================

void MidiController::handleError(const juce::String& error)
{
    if (onError)
        onError(error);
    
    // Log error to console for debugging
    DBG("MidiController error: " + error);
}