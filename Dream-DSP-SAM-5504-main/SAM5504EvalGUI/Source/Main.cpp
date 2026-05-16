#include <JuceHeader.h>

namespace
{
constexpr int appWidth = 700;
constexpr int appHeight = 850;

constexpr int masterGainNrpn = 0x0100;
constexpr int outputGainProcess1Nrpn = 0x0102;
constexpr int outputGainProcess3Nrpn = 0x0302;
constexpr double masterGainCalibrationDb = 18.0;
constexpr int leftInputChannel = 0;    // DSP1 (left input)
constexpr int leftOutputChannel = 1;   // DSP2 (left output)
constexpr int rightInputChannel = 2;   // DSP3 (right input)
constexpr int rightOutputChannel = 3;  // DSP4 (right output)

int dbToSamGainValue (double db)
{
    constexpr double zeroDb = 0x4000;
    constexpr double unitsPerDb = 512.0;
    constexpr int minValue = 0x1000;
    constexpr int maxValue = 0x7fff;

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
        // ============== TOP SECTION ==============
        addAndMakeVisible (title);
        title.setText ("SAM5504 Audio DSP Control", juce::dontSendNotification);
        title.setJustificationType (juce::Justification::centredLeft);
        title.setFont (juce::FontOptions (26.0f, juce::Font::bold));

        // MIDI Device Selection
        addAndMakeVisible (deviceLabel);
        deviceLabel.setText ("MIDI out:", juce::dontSendNotification);
        deviceLabel.setFont (juce::FontOptions (14.0f, juce::Font::bold));

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

        // ============== MASTER GAIN SECTION ==============
        addAndMakeVisible (masterGainSeparator);
        masterGainSeparator.setText ("Master Control", juce::dontSendNotification);
        masterGainSeparator.setFont (juce::FontOptions (16.0f, juce::Font::bold));

        addAndMakeVisible (masterGainLabel);
        masterGainLabel.setText ("Master Gain:", juce::dontSendNotification);
        masterGainLabel.setFont (juce::FontOptions (13.0f));

        addAndMakeVisible (masterGainSlider);
        masterGainSlider.setSliderStyle (juce::Slider::LinearHorizontal);
        masterGainSlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 70, 24);
        masterGainSlider.setRange (-24.0, 24.0, 0.1);
        masterGainSlider.setValue (0.0, juce::dontSendNotification);
        masterGainSlider.setTextValueSuffix (" dB");
        masterGainSlider.addListener (this);

        // ============== OUTPUT CONTROLS SECTION ==============
        addAndMakeVisible (outputsSeparator);
        outputsSeparator.setText ("Individual Output Gain Control", juce::dontSendNotification);
        outputsSeparator.setFont (juce::FontOptions (16.0f, juce::Font::bold));

        // Output A (Left channel output DSP#2)
        addAndMakeVisible (outputALabel);
        outputALabel.setText ("Output A (Left):", juce::dontSendNotification);
        outputALabel.setFont (juce::FontOptions (12.0f));

        addAndMakeVisible (outputASlider);
        outputASlider.setSliderStyle (juce::Slider::LinearHorizontal);
        outputASlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 60, 22);
        outputASlider.setRange (-24.0, 24.0, 0.1);
        outputASlider.setValue (0.0, juce::dontSendNotification);
        outputASlider.setTextValueSuffix (" dB");
        outputASlider.addListener (this);

        // Output B (Left channel output DSP#2)
        addAndMakeVisible (outputBLabel);
        outputBLabel.setText ("Output B (Left):", juce::dontSendNotification);
        outputBLabel.setFont (juce::FontOptions (12.0f));

        addAndMakeVisible (outputBSlider);
        outputBSlider.setSliderStyle (juce::Slider::LinearHorizontal);
        outputBSlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 60, 22);
        outputBSlider.setRange (-24.0, 24.0, 0.1);
        outputBSlider.setValue (0.0, juce::dontSendNotification);
        outputBSlider.setTextValueSuffix (" dB");
        outputBSlider.addListener (this);

        // Output C (Right channel output DSP#4)
        addAndMakeVisible (outputCLabel);
        outputCLabel.setText ("Output C (Right):", juce::dontSendNotification);
        outputCLabel.setFont (juce::FontOptions (12.0f));

        addAndMakeVisible (outputCSlider);
        outputCSlider.setSliderStyle (juce::Slider::LinearHorizontal);
        outputCSlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 60, 22);
        outputCSlider.setRange (-24.0, 24.0, 0.1);
        outputCSlider.setValue (0.0, juce::dontSendNotification);
        outputCSlider.setTextValueSuffix (" dB");
        outputCSlider.addListener (this);

        // Output D (Right channel output DSP#4)
        addAndMakeVisible (outputDLabel);
        outputDLabel.setText ("Output D (Right):", juce::dontSendNotification);
        outputDLabel.setFont (juce::FontOptions (12.0f));

        addAndMakeVisible (outputDSlider);
        outputDSlider.setSliderStyle (juce::Slider::LinearHorizontal);
        outputDSlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 60, 22);
        outputDSlider.setRange (-24.0, 24.0, 0.1);
        outputDSlider.setValue (0.0, juce::dontSendNotification);
        outputDSlider.setTextValueSuffix (" dB");
        outputDSlider.addListener (this);

        // ============== BOARD INFO SECTION ==============
        addAndMakeVisible (boardInfoSeparator);
        boardInfoSeparator.setText ("Board Information", juce::dontSendNotification);
        boardInfoSeparator.setFont (juce::FontOptions (16.0f, juce::Font::bold));

        addAndMakeVisible (boardInfo);
        juce::String infoText;
        infoText += "DREAM SAM5504 Evaluation Board";
        infoText += "\nAudio: 96 kHz, 2-in / 4-out stereo";
        infoText += "\nPrecision: 24-bit fixed-point";
        infoText += "\nDSP Chains: 4 parallel processors";
        infoText += "\nFeatures: Gain, 3-band & 2-band EQ";
        infoText += "\nStatus: Production-Ready";
        boardInfo.setText (infoText, juce::dontSendNotification);
        boardInfo.setJustificationType (juce::Justification::topLeft);
        boardInfo.setFont (juce::FontOptions (11.0f));
        boardInfo.setColour (juce::Label::textColourId, juce::Colours::darkgrey);

        setSize (appWidth, appHeight);
        refreshMidiDevices();
        updateStatus();
    }

    ~MainComponent() override
    {
        masterGainSlider.removeListener (this);
        outputASlider.removeListener (this);
        outputBSlider.removeListener (this);
        outputCSlider.removeListener (this);
        outputDSlider.removeListener (this);
        deviceList.removeListener (this);
        refreshButton.removeListener (this);
        connectButton.removeListener (this);
    }

    void paint (juce::Graphics& g) override
    {
        g.fillAll (juce::Colour (0xfff6f7f8));
        
        // Draw section backgrounds
        g.setColour (juce::Colour (0xffefeff2));
        g.fillRoundedRectangle (midiArea.toFloat(), 6.0f);
        g.fillRoundedRectangle (controlArea.toFloat(), 6.0f);
        g.fillRoundedRectangle (outputArea.toFloat(), 6.0f);
        g.fillRoundedRectangle (boardArea.toFloat(), 6.0f);

        // Draw borders
        g.setColour (juce::Colour (0xffc7ccd1));
        g.drawRoundedRectangle (midiArea.toFloat(), 6.0f, 1.0f);
        g.drawRoundedRectangle (controlArea.toFloat(), 6.0f, 1.0f);
        g.drawRoundedRectangle (outputArea.toFloat(), 6.0f, 1.0f);
        g.drawRoundedRectangle (boardArea.toFloat(), 6.0f, 1.0f);
    }

    void resized() override
    {
        auto bounds = getLocalBounds().reduced (20);
        
        // Title
        title.setBounds (bounds.removeFromTop (40));
        bounds.removeFromTop (16);

        // MIDI Section
        midiArea = bounds.removeFromTop (110);
        auto midiInner = midiArea.reduced (16);

        auto midiRow = midiInner.removeFromTop (32);
        deviceLabel.setBounds (midiRow.removeFromLeft (70));
        deviceList.setBounds (midiRow.removeFromLeft (200));
        midiRow.removeFromLeft (10);
        refreshButton.setBounds (midiRow.removeFromLeft (85));
        midiRow.removeFromLeft (8);
        connectButton.setBounds (midiRow.removeFromLeft (85));

        midiInner.removeFromTop (14);
        status.setBounds (midiInner.removeFromTop (24));

        bounds.removeFromTop (12);

        // Master Gain Section
        controlArea = bounds.removeFromTop (80);
        auto controlInner = controlArea.reduced (16);
        masterGainSeparator.setBounds (controlInner.removeFromTop (24));
        controlInner.removeFromTop (8);
        masterGainLabel.setBounds (controlInner.removeFromLeft (80).removeFromTop (24));
        masterGainSlider.setBounds (controlInner.removeFromTop (28));

        bounds.removeFromTop (12);

        // Output Controls Section
        outputArea = bounds.removeFromTop (180);
        auto outputInner = outputArea.reduced (16);
        outputsSeparator.setBounds (outputInner.removeFromTop (24));
        outputInner.removeFromTop (8);

        auto outRow = outputInner.removeFromTop (28);
        outputALabel.setBounds (outRow.removeFromLeft (100));
        outputASlider.setBounds (outRow);

        outputInner.removeFromTop (6);
        outRow = outputInner.removeFromTop (28);
        outputBLabel.setBounds (outRow.removeFromLeft (100));
        outputBSlider.setBounds (outRow);

        outputInner.removeFromTop (6);
        outRow = outputInner.removeFromTop (28);
        outputCLabel.setBounds (outRow.removeFromLeft (100));
        outputCSlider.setBounds (outRow);

        outputInner.removeFromTop (6);
        outRow = outputInner.removeFromTop (28);
        outputDLabel.setBounds (outRow.removeFromLeft (100));
        outputDSlider.setBounds (outRow);

        bounds.removeFromTop (12);

        // Board Info Section
        boardArea = bounds;
        auto boardInner = boardArea.reduced (16);
        boardInfoSeparator.setBounds (boardInner.removeFromTop (24));
        boardInner.removeFromTop (8);
        boardInfo.setBounds (boardInner);
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
        {
            sendMasterGain();
            sendOutputGains();
        }
    }

    void sendMasterGain()
    {
        if (midiOut == nullptr)
            return;

        const auto value = dbToSamGainValue (masterGainSlider.getValue() + masterGainCalibrationDb);
        sendDreamNrpn (*midiOut, leftInputChannel, masterGainNrpn, value);
        sendDreamNrpn (*midiOut, rightInputChannel, masterGainNrpn, value);
    }

    void sendOutputGains()
    {
        if (midiOut == nullptr)
            return;

        // DSP #2: Output A = process 1, Output B = process 3
        auto valueA = dbToSamGainValue (outputASlider.getValue());
        sendDreamNrpn (*midiOut, leftOutputChannel, outputGainProcess1Nrpn, valueA);

        auto valueB = dbToSamGainValue (outputBSlider.getValue());
        sendDreamNrpn (*midiOut, leftOutputChannel, outputGainProcess3Nrpn, valueB);

        // DSP #4: Output C = process 1, Output D = process 3
        auto valueC = dbToSamGainValue (outputCSlider.getValue());
        sendDreamNrpn (*midiOut, rightOutputChannel, outputGainProcess1Nrpn, valueC);

        auto valueD = dbToSamGainValue (outputDSlider.getValue());
        sendDreamNrpn (*midiOut, rightOutputChannel, outputGainProcess3Nrpn, valueD);
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

    void sliderValueChanged (juce::Slider* slider) override
    {
        if (slider == &masterGainSlider)
        {
            sendMasterGain();
            return;
        }

        if (slider == &outputASlider || slider == &outputBSlider ||
            slider == &outputCSlider || slider == &outputDSlider)
        {
            sendOutputGains();
            return;
        }
    }

    // Top section
    juce::Label title;
    juce::Label deviceLabel;
    juce::ComboBox deviceList;
    juce::TextButton refreshButton;
    juce::TextButton connectButton;
    juce::Label status;
    juce::Rectangle<int> midiArea;

    // Master gain section
    juce::Label masterGainSeparator;
    juce::Label masterGainLabel;
    juce::Slider masterGainSlider;
    juce::Rectangle<int> controlArea;

    // Output control section
    juce::Label outputsSeparator;
    juce::Label outputALabel;
    juce::Slider outputASlider;
    juce::Label outputBLabel;
    juce::Slider outputBSlider;
    juce::Label outputCLabel;
    juce::Slider outputCSlider;
    juce::Label outputDLabel;
    juce::Slider outputDSlider;
    juce::Rectangle<int> outputArea;

    // Board info section
    juce::Label boardInfoSeparator;
    juce::Label boardInfo;
    juce::Rectangle<int> boardArea;

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
