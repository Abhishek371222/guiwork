#pragma once

#include <JuceHeader.h>

/**
 * Handles MIDI device management and NRPN message transmission.
 */
class MidiController : private juce::Timer
{
public:
    MidiController();
    ~MidiController();
    
    // Device management
    juce::Array<juce::MidiDeviceInfo> getAvailableDevices() const;
    bool connectToDevice(const juce::MidiDeviceInfo& device);
    void disconnect();
    bool isConnected() const;
    juce::String getConnectedDeviceName() const;
    
    // Message transmission
    void sendNrpn(int channel, int nrpn, int value);
    void sendParameterChange(const juce::String& paramId, float value);
    
    // Queue management
    void flushQueue();
    int getQueueSize() const;
    void setTransmissionRate(int messagesPerSecond);
    
    // Callbacks
    std::function<void(bool connected)> onConnectionChanged;
    std::function<void(int messagesSent)> onMessagesSent;
    std::function<void(const juce::String& error)> onError;
    
    // Parameter mapping (to be set by MainComponent)
    std::function<int(const juce::String& paramId)> getMidiChannelForParam;
    std::function<int(const juce::String& paramId)> getNrpnForParam;
    std::function<int(float value, const juce::String& paramId)> convertValueToSam;
    
private:
    struct QueuedMessage {
        int channel;
        int nrpn;
        int value;
        juce::Time timestamp;
    };
    
    std::unique_ptr<juce::MidiOutput> midiOutput;
    juce::Array<QueuedMessage> messageQueue;
    int transmissionRate = 100;  // messages per second
    juce::CriticalSection queueLock;
    
    void timerCallback() override;
    void sendDreamNrpn(int channel, int nrpn, int value);
    void processQueue();
    
    // NRPN message construction
    static juce::MidiMessage createNrpnMessage(int channel, int nrpn, int value);
    
    // Error handling
    void handleError(const juce::String& error);
};