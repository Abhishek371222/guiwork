#include <JuceHeader.h>

namespace
{
constexpr int appWidth = 560;
constexpr int appHeight = 320;

constexpr int masterGainNrpn = 0x0100;
constexpr int leftInputChannel = 0;   // DSP1
constexpr int rightInputChannel = 2;  // DSP3

int dbToSamGainValue (double db)
{
    constexpr double zeroDb = 0x4000;
    constexpr double unitsPerDb = 512.0;
    constexpr int minValue = 0x1000;
    constexpr int maxValue = 0x5e00;

    return juce::jlimit (minValue, maxValue, juce::roundToInt (zeroDb + db * unitsPerDb));
}

juce::MidiMessage cc (int channelZeroBased, int controller, int value)
{
    return juce::MidiMessage::controllerEvent (channelZeroBased + 1,
                                               controller,
                                               juce::jlimit (0, 127, value));
}

void sendDreamNrpn (juce::MidiOutput& output, int channelZeroBased, int nrpn, int value)
{
    value = juce::jlimit (0, 0x7fff, value);

    const auto nrpnMsb = (nrpn >> 8) & 0x7f;
    const auto nrpnLsb = nrpn & 0x7f;
    const auto valueMsb = (value >> 8) & 0x7f;
    const auto valueLsb = (value >> 1) & 0x7f;

    output.sendMessageNow (cc (channelZeroBased, 99, nrpnMsb));
    output.sendMessageNow (cc (channelZeroBased, 98, nrpnLsb));
    output.sendMessageNow (cc (channelZeroBased, 38, valueLsb));
    output.sendMessageNow (cc (channelZeroBased, 6, valueMsb));
}
}

class MainComponent final : public juce::Component,
                            private juce::Button::Listener,
                            private juce::ComboBox::Listener,
                            private juce::Slider::Listener
{
public:
    MainComponent()
    {
        addAndMakeVisible (title);
        title.setText ("SAM5504 MIDI Control", juce::dontSendNotification);
        title.setJustificationType (juce::Justification::centredLeft);
        title.setFont (juce::FontOptions (24.0f, juce::Font::bold));

        addAndMakeVisible (deviceLabel);
        deviceLabel.setText ("MIDI out", juce::dontSendNotification);
        deviceLabel.attachToComponent (&deviceList, true);

        addAndMakeVisible (deviceList);
        deviceList.addListener (this);

        addAndMakeVisible (refreshButton);
        refreshButton.setButtonText ("Refresh");
        refreshButton.addListener (this);

        addAndMakeVisible (connectButton);
        connectButton.setButtonText ("Connect");
        connectButton.addListener (this);

        addAndMakeVisible (status);
        status.setJustificationType (juce::Justification::centredLeft);

        addAndMakeVisible (gainLabel);
        gainLabel.setText ("Master gain", juce::dontSendNotification);
        gainLabel.attachToComponent (&gainSlider, false);

        addAndMakeVisible (gainSlider);
        gainSlider.setSliderStyle (juce::Slider::LinearHorizontal);
        gainSlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 88, 26);
        gainSlider.setRange (-24.0, 15.0, 0.1);
        gainSlider.setValue (0.0, juce::dontSendNotification);
        gainSlider.setTextValueSuffix (" dB");
        gainSlider.addListener (this);

        addAndMakeVisible (hint);
        hint.setText ("Sends NRPN 0x0100 to DSP1 and DSP3 in real time.",
                      juce::dontSendNotification);
        hint.setJustificationType (juce::Justification::centredLeft);
        hint.setColour (juce::Label::textColourId, juce::Colours::grey);

        setSize (appWidth, appHeight);
        refreshMidiDevices();
        updateStatus();
    }

    ~MainComponent() override
    {
        gainSlider.removeListener (this);
        deviceList.removeListener (this);
        refreshButton.removeListener (this);
        connectButton.removeListener (this);
    }

    void paint (juce::Graphics& g) override
    {
        g.fillAll (juce::Colour (0xfff6f7f8));
        g.setColour (juce::Colour (0xffc7ccd1));
        g.drawRoundedRectangle (contentArea.toFloat(), 8.0f, 1.0f);
    }

    void resized() override
    {
        auto bounds = getLocalBounds().reduced (24);
        title.setBounds (bounds.removeFromTop (40));
        bounds.removeFromTop (18);

        contentArea = bounds.removeFromTop (210);
        auto inner = contentArea.reduced (24);

        auto row = inner.removeFromTop (32);
        deviceList.setBounds (row.removeFromLeft (260));
        row.removeFromLeft (12);
        refreshButton.setBounds (row.removeFromLeft (92));
        row.removeFromLeft (12);
        connectButton.setBounds (row.removeFromLeft (104));

        inner.removeFromTop (22);
        status.setBounds (inner.removeFromTop (24));

        inner.removeFromTop (34);
        gainSlider.setBounds (inner.removeFromTop (44));

        inner.removeFromTop (12);
        hint.setBounds (inner.removeFromTop (24));
    }

private:
    void refreshMidiDevices()
    {
        outputs = juce::MidiOutput::getAvailableDevices();

        const auto previous = deviceList.getSelectedId();
        deviceList.clear (juce::dontSendNotification);

        for (int i = 0; i < outputs.size(); ++i)
            deviceList.addItem (outputs.getReference (i).name, i + 1);

        if (outputs.isEmpty())
            deviceList.setText ("No MIDI outputs found", juce::dontSendNotification);
        else if (previous > 0 && previous <= outputs.size())
            deviceList.setSelectedId (previous, juce::dontSendNotification);
        else
            deviceList.setSelectedId (1, juce::dontSendNotification);

        updateStatus();
    }

    void connectSelectedDevice()
    {
        midiOut.reset();

        const auto index = deviceList.getSelectedId() - 1;
        if (! juce::isPositiveAndBelow (index, outputs.size()))
        {
            updateStatus();
            return;
        }

        midiOut = juce::MidiOutput::openDevice (outputs.getReference (index).identifier);
        updateStatus();

        if (midiOut != nullptr)
            sendMasterGain();
    }

    void sendMasterGain()
    {
        if (midiOut == nullptr)
            return;

        const auto value = dbToSamGainValue (gainSlider.getValue());
        sendDreamNrpn (*midiOut, leftInputChannel, masterGainNrpn, value);
        sendDreamNrpn (*midiOut, rightInputChannel, masterGainNrpn, value);
    }

    void updateStatus()
    {
        if (midiOut != nullptr)
        {
            connectButton.setButtonText ("Disconnect");
            status.setText ("Connected: " + midiOut->getName(), juce::dontSendNotification);
            status.setColour (juce::Label::textColourId, juce::Colour (0xff176b35));
            return;
        }

        connectButton.setButtonText ("Connect");
        status.setText ("Not connected", juce::dontSendNotification);
        status.setColour (juce::Label::textColourId, juce::Colour (0xff8a2d2d));
    }

    void buttonClicked (juce::Button* button) override
    {
        if (button == &refreshButton)
        {
            refreshMidiDevices();
            return;
        }

        if (button == &connectButton)
        {
            if (midiOut != nullptr)
                midiOut.reset();
            else
                connectSelectedDevice();

            updateStatus();
        }
    }

    void comboBoxChanged (juce::ComboBox*) override
    {
        if (midiOut != nullptr)
            connectSelectedDevice();
    }

    void sliderValueChanged (juce::Slider*) override
    {
        sendMasterGain();
    }

    juce::Label title;
    juce::Label deviceLabel;
    juce::ComboBox deviceList;
    juce::TextButton refreshButton;
    juce::TextButton connectButton;
    juce::Label status;
    juce::Label gainLabel;
    juce::Slider gainSlider;
    juce::Label hint;
    juce::Rectangle<int> contentArea;

    juce::Array<juce::MidiDeviceInfo> outputs;
    std::unique_ptr<juce::MidiOutput> midiOut;
};

class SAM5504EvalGUIApplication final : public juce::JUCEApplication
{
public:
    const juce::String getApplicationName() override { return "SAM5504EvalGUI"; }
    const juce::String getApplicationVersion() override { return "0.1.0"; }
    bool moreThanOneInstanceAllowed() override { return true; }

    void initialise (const juce::String&) override
    {
        mainWindow = std::make_unique<MainWindow> (getApplicationName());
    }

    void shutdown() override
    {
        mainWindow = nullptr;
    }

    void systemRequestedQuit() override
    {
        quit();
    }

private:
    class MainWindow final : public juce::DocumentWindow
    {
    public:
        explicit MainWindow (juce::String name)
            : DocumentWindow (std::move (name),
                              juce::Desktop::getInstance().getDefaultLookAndFeel()
                                  .findColour (juce::ResizableWindow::backgroundColourId),
                              DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar (true);
            setResizable (false, false);
            setContentOwned (new MainComponent(), true);
            centreWithSize (getWidth(), getHeight());
            setVisible (true);
        }

        void closeButtonPressed() override
        {
            juce::JUCEApplication::getInstance()->systemRequestedQuit();
        }
    };

    std::unique_ptr<MainWindow> mainWindow;
};

START_JUCE_APPLICATION (SAM5504EvalGUIApplication)
