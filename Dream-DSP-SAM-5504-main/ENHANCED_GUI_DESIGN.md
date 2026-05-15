# Enhanced GUI with Full Parameter Control - Technical Design Document

## 1. Architecture Overview

### 1.1 System Architecture
```
┌─────────────────────────────────────────────────────────────┐
│                    Enhanced GUI Application                 │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  ┌─────────────────────────────────────────────────────┐   │
│  │                MainComponent (UI Layer)             │   │
│  │  • Tabbed interface (Gain/EQ, Compressor, Delay)   │   │
│  │  • Parameter controls (Sliders, ComboBoxes, Buttons)│   │
│  │  • Layout management (FlexBox)                      │   │
│  └─────────────────────────────────────────────────────┘   │
│                            │                                │
│  ┌─────────────────────────────────────────────────────┐   │
│  │            ParameterManager (Business Logic)        │   │
│  │  • Parameter value storage & validation             │   │
│  │  • Value conversion (dB ↔ SAM units)                │   │
│  │  • Preset management                                │   │
│  │  • Undo/redo stack                                  │   │
│  └─────────────────────────────────────────────────────┘   │
│                            │                                │
│  ┌─────────────────────────────────────────────────────┐   │
│  │            MidiController (Communication Layer)     │   │
│  │  • MIDI device management                           │   │
│  │  • NRPN message generation                          │   │
│  │  • Message queuing & transmission                   │   │
│  └─────────────────────────────────────────────────────┘   │
│                            │                                │
│  ┌─────────────────────────────────────────────────────┐   │
│  │                 JUCE Framework                      │   │
│  │  • UI components & rendering                        │   │
│  │  • MIDI I/O                                         │   │
│  │  • File I/O                                         │   │
│  └─────────────────────────────────────────────────────┘   │
│                                                             │
└─────────────────────────────────────────────────────────────┘
         │
         ▼
┌─────────────────────────────────────────────────────────────┐
│                    SAM5504 Hardware                         │
│  • DSP1-DSP4 processing chains                             │
│  • USB MIDI interface                                      │
│  • Audio I/O                                               │
└─────────────────────────────────────────────────────────────┘
```

### 1.2 Component Relationships
```
MainComponent ── owns ──► ParameterManager ── uses ──► MidiController
      │                         │                         │
      │                         │                         ▼
      │                         │                  MIDI Device
      ▼                         ▼
   UI Events ──────────► Parameter Updates ─────► NRPN Messages
```

## 2. Class Design

### 2.1 MainComponent (Extended)
```cpp
class MainComponent : public juce::Component,
                      private juce::Button::Listener,
                      private juce::ComboBox::Listener,
                      private juce::Slider::Listener
{
public:
    // Existing members
    juce::Label title;
    juce::ComboBox deviceList;
    juce::TextButton refreshButton;
    juce::TextButton connectButton;
    juce::Label status;
    juce::Slider gainSlider;
    juce::Label hint;
    
    // New members for enhanced GUI
    juce::TabbedComponent tabbedComponent;
    
    // Tab 1: Gain & EQ
    juce::Component gainEqTab;
    juce::ComboBox channelSelector;
    juce::ToggleButton linkChannelsButton;
    
    // Gain controls
    juce::Slider gainSliderLeft;
    juce::Slider gainSliderRight;
    juce::Slider gainPhaseSliderLeft;
    juce::Slider gainPhaseSliderRight;
    
    // EQ controls (3 bands for DSP1/DSP3, 2 bands for DSP2/DSP4)
    struct EqBandControls {
        juce::ToggleButton enableButton;
        juce::ComboBox filterTypeCombo;
        juce::Slider frequencySlider;
        juce::Slider gainSlider;
        juce::Slider qSlider;
    };
    
    std::array<EqBandControls, 3> eqBandsLeft;  // DSP1
    std::array<EqBandControls, 2> eqBandsRightA; // DSP2 Channel A
    std::array<EqBandControls, 2> eqBandsRightB; // DSP2 Channel B
    
    // Tab 2: Compressor
    juce::Component compressorTab;
    juce::ToggleButton compressorEnableButton;
    juce::Slider thresholdSlider;
    juce::Slider ratioSlider;
    juce::Slider attackSlider;
    juce::Slider releaseSlider;
    juce::Slider makeupGainSlider;
    juce::Label gainReductionLabel;  // Read-only display
    
    // Tab 3: Delay
    juce::Component delayTab;
    struct DelayChannelControls {
        juce::ToggleButton enableButton;
        juce::Slider timeSlider;
        juce::Slider gainSlider;
        juce::Slider phaseSlider;
    };
    
    DelayChannelControls delayLeftA;   // DSP2 Process 1
    DelayChannelControls delayLeftB;   // DSP2 Process 3
    DelayChannelControls delayRightA;  // DSP4 Process 1
    DelayChannelControls delayRightB;  // DSP4 Process 3
    
    // Tab 4: Settings & Presets
    juce::Component settingsTab;
    juce::TextButton savePresetButton;
    juce::TextButton loadPresetButton;
    juce::TextButton factoryResetButton;
    juce::ComboBox presetSelector;
    juce::Label versionLabel;
    
    // Business logic components
    std::unique_ptr<ParameterManager> parameterManager;
    std::unique_ptr<MidiController> midiController;
    
    // ... existing methods plus new ones
};
```

### 2.2 ParameterManager Class
```cpp
class ParameterManager
{
public:
    struct Parameter {
        juce::String id;
        juce::String name;
        juce::String description;
        int nrpnAddress;
        int midiChannel;  // 0=DSP1, 1=DSP2, 2=DSP3, 3=DSP4
        float minValue;
        float maxValue;
        float defaultValue;
        float currentValue;
        juce::String unit;
        std::function<float(float)> toSamValue;  // Conversion to SAM units
        std::function<float(float)> fromSamValue; // Conversion from SAM units
    };
    
    ParameterManager();
    
    // Parameter access
    float getParameterValue(const juce::String& id) const;
    void setParameterValue(const juce::String& id, float value, bool sendMidi = true);
    
    // Preset management
    bool savePreset(const juce::File& file);
    bool loadPreset(const juce::File& file);
    void factoryReset();
    
    // Undo/redo
    bool canUndo() const;
    bool canRedo() const;
    void undo();
    void redo();
    
    // Parameter groups
    juce::Array<juce::String> getGainParameters() const;
    juce::Array<juce::String> getEqParameters(int dspIndex) const;
    juce::Array<juce::String> getCompressorParameters() const;
    juce::Array<juce::String> getDelayParameters(int dspIndex) const;
    
private:
    juce::HashMap<juce::String, Parameter> parameters;
    juce::Array<juce::String> parameterHistory;
    int historyIndex = -1;
    
    void initializeParameters();
    void addParameter(const Parameter& param);
    
    // Conversion functions
    static float dbToSamGain(float db);
    static float samGainToDb(float samValue);
    static float hzToSamFreq(float hz);
    static float samFreqToHz(float samValue);
    static float qToSamQ(float q);
    static float samQToQ(float samValue);
    static float msToSamTime(float ms);
    static float samTimeToMs(float samValue);
    static float ratioToSamRatio(float ratio);
    static float samRatioToRatio(float samValue);
};
```

### 2.3 MidiController Class
```cpp
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
};
```

## 3. Data Structures

### 3.1 Parameter Definition Database
```cpp
// Parameter definitions for all DSPs
const std::vector<ParameterManager::Parameter> parameterDefinitions = {
    // DSP1 Parameters (Left Input Chain)
    {
        "dsp1_gain_value",
        "Left Gain",
        "Input gain for left channel",
        0x0100, 0,  // NRPN 0x0100, MIDI channel 0
        -24.0f, 15.0f, 0.0f, 0.0f, "dB",
        ParameterManager::dbToSamGain,
        ParameterManager::samGainToDb
    },
    {
        "dsp1_gain_phase",
        "Left Gain Phase",
        "Phase adjustment for left input gain",
        0x0101, 0,
        0.0f, 360.0f, 0.0f, 0.0f, "°",
        [](float deg) { return static_cast<int>(deg * 182.04f) & 0x7FFF; },
        [](float sam) { return sam / 182.04f; }
    },
    
    // DSP1 EQ Band 0
    {
        "dsp1_eq0_enable",
        "Left EQ Band 1 Enable",
        "Enable/disable first EQ band",
        0x0200, 0,
        0.0f, 1.0f, 1.0f, 1.0f, "",
        [](float enabled) { return enabled > 0.5f ? 0x7FFF : 0x0000; },
        [](float sam) { return sam > 0x3FFF ? 1.0f : 0.0f; }
    },
    {
        "dsp1_eq0_freq",
        "Left EQ Band 1 Frequency",
        "Center frequency for first EQ band",
        0x0241, 0,
        20.0f, 20000.0f, 1000.0f, 1000.0f, "Hz",
        ParameterManager::hzToSamFreq,
        ParameterManager::samFreqToHz
    },
    {
        "dsp1_eq0_gain",
        "Left EQ Band 1 Gain",
        "Gain adjustment for first EQ band",
        0x0260, 0,
        -12.0f, 12.0f, 0.0f, 0.0f, "dB",
        ParameterManager::dbToSamGain,
        ParameterManager::samGainToDb
    },
    {
        "dsp1_eq0_q",
        "Left EQ Band 1 Q",
        "Q factor (resonance) for first EQ band",
        0x0222, 0,
        0.1f, 10.0f, 1.0f, 1.0f, "",
        ParameterManager::qToSamQ,
        ParameterManager::samQToQ
    },
    
    // DSP1 Compressor
    {
        "dsp1_comp_threshold",
        "Left Compressor Threshold",
        "Level at which compression begins",
        0x0502, 0,
        -60.0f, 0.0f, -20.0f, -20.0f, "dB",
        ParameterManager::dbToSamGain,
        ParameterManager::samGainToDb
    },
    {
        "dsp1_comp_ratio",
        "Left Compressor Ratio",
        "Compression ratio (e.g., 4:1)",
        0x0503, 0,
        1.0f, 20.0f, 4.0f, 4.0f, ":1",
        ParameterManager::ratioToSamRatio,
        ParameterManager::samRatioToRatio
    },
    
    // DSP2 Delay (Channel A)
    {
        "dsp2_delayA_time",
        "Left Delay A Time",
        "Delay time for left channel output A",
        0x0101, 1,  // NRPN 0x0101, MIDI channel 1
        0.0f, 5.33f, 0.0f, 0.0f, "ms",  // Max 512 samples @ 96kHz = 5.33ms
        ParameterManager::msToSamTime,
        ParameterManager::samTimeToMs
    },
    
    // ... continue for all 100+ parameters
};
```

### 3.2 Preset File Format (JSON)
```json
{
  "version": "1.0",
  "name": "Vocal Enhancement",
  "description": "EQ and compression settings for vocal processing",
  "timestamp": "2024-01-15T10:30:00Z",
  "parameters": {
    "dsp1_gain_value": 2.5,
    "dsp1_eq0_freq": 120.0,
    "dsp1_eq0_gain": 3.0,
    "dsp1_eq0_q": 1.5,
    "dsp1_comp_threshold": -15.0,
    "dsp1_comp_ratio": 3.0,
    "dsp1_comp_attack": 10.0,
    "dsp1_comp_release": 100.0,
    "dsp3_gain_value": 2.5,
    "dsp3_eq0_freq": 120.0,
    "dsp3_eq0_gain": 3.0,
    "dsp3_eq0_q": 1.5,
    "dsp3_comp_threshold": -15.0,
    "dsp3_comp_ratio": 3.0,
    "dsp3_comp_attack": 10.0,
    "dsp3_comp_release": 100.0
  }
}
```

## 4. UI Layout Design

### 4.1 Window Dimensions
- **Width:** 800 pixels (increased from 560)
- **Height:** 600 pixels (increased from 320)
- **Minimum size:** 640×480 pixels
- **Default position:** Centered on screen

### 4.2 Tabbed Interface Layout
```
┌─────────────────────────────────────────────────────────────┐
│  SAM5504 DSP Control v2.0                                  │
├─────────────────────────────────────────────────────────────┤
│  [Gain & EQ]  [Compressor]  [Delay]  [Settings]            │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐        │
│  │   Channel   │  │   Left EQ   │  │  Right EQ   │        │
│  │ [● Left]    │  │ ┌─────────┐ │  │ ┌─────────┐ │        │
│  │ [○ Right]   │  │ │ Band 1  │ │  │ │ Band 1  │ │        │
│  │ [○ Both]    │  │ └─────────┘ │  │ └─────────┘ │        │
│  │ [✓ Linked]  │  │ ┌─────────┐ │  │ ┌─────────┐ │        │
│  └─────────────┘  │ │ Band 2  │ │  │ │ Band 2  │ │        │
│                   │ └─────────┘ │  │ └─────────┘ │        │
│  ┌─────────────┐  │ ┌─────────┐ │  │ ┌─────────┐ │        │
│  │   Gain      │  │ │ Band 3  │ │  │ │ Band 3  │ │        │
│  │ Left:  [══] │  │ └─────────┘ │  │ └─────────┘ │        │
│  │ Right: [══] │  └─────────────┘  └─────────────┘        │
│  └─────────────┘                                          │
│                                                             │
│  MIDI: [Device List ▼] [Refresh] [Connect]                │
│  Status: Connected to SAM5504                             │
└─────────────────────────────────────────────────────────────┘
```

### 4.3 Component Spacing and Sizing
- **Margin:** 16 pixels around window edges
- **Padding:** 8 pixels between components
- **Control height:** 24 pixels (sliders, buttons)
- **Label height:** 20 pixels
- **Group box padding:** 12 pixels
- **Tab content padding:** 16 pixels

### 4.4 Color Scheme
- **Background:** `#f6f7f8` (light gray)
- **Foreground:** `#2c3e50` (dark blue-gray)
- **Accent:** `#3498db` (blue)
- **Success:** `#27ae60` (green)
- **Warning:** `#e74c3c` (red)
- **Disabled:** `#bdc3c7` (light gray)

## 5. Implementation Details

### 5.1 Value Conversion Functions
```cpp
// In ParameterManager.cpp
float ParameterManager::dbToSamGain(float db) {
    // 0x4000 = 0 dB, 512 units = 1 dB
    constexpr float zeroDb = 0x4000;
    constexpr float unitsPerDb = 512.0f;
    constexpr float minValue = 0x1000;  // -24 dB
    constexpr float maxValue = 0x5e00;  // +15 dB
    
    float samValue = zeroDb + db * unitsPerDb;
    return juce::jlimit(minValue, maxValue, samValue);
}

float ParameterManager::hzToSamFreq(float hz) {
    // Logarithmic mapping: 20Hz-20kHz to 0x0000-0x7FFF
    constexpr float minHz = 20.0f;
    constexpr float maxHz = 20000.0f;
    
    // Convert to logarithmic scale
    float logMin = std::log10(minHz);
    float logMax = std::log10(maxHz);
    float logValue = std::log10(juce::jlimit(minHz, maxHz, hz));
    
    // Map to 0-1 range
    float normalized = (logValue - logMin) / (logMax - logMin);
    
    // Convert to 14-bit SAM value
    return normalized * 0x7FFF;
}

float ParameterManager::msToSamTime(float ms) {
    // 0-5.33ms maps to 0x0000-0x7FFF
    constexpr float maxMs = 5.33f;  // 512 samples @ 96kHz
    
    float normalized = juce::jlimit(0.0f, maxMs, ms) / maxMs;
    return normalized * 0x7FFF;
}
```

### 5.2 MIDI Message Queue Implementation
```cpp
// In MidiController.cpp
void MidiController::sendNrpn(int channel, int nrpn, int value) {
    QueuedMessage msg;
    msg.channel = channel;
    msg.nrpn = nrpn;
    msg.value = juce::jlimit(0, 0x7FFF, value);
    msg.timestamp = juce::Time::getCurrentTime();
    
    {
        juce::ScopedLock lock(queueLock);
        messageQueue.add(msg);
    }
    
    // Start timer if not already running
    if (!isTimerRunning()) {
        startTimer(1000 / transmissionRate);
    }
}

void MidiController::timerCallback() {
    processQueue();
}

void MidiController::processQueue() {
    juce::ScopedLock lock(queueLock);
    
    if (messageQueue.isEmpty() || !midiOutput) {
        stopTimer();
        return;
    }
    
    // Send up to 10 messages per timer tick
    int messagesToSend = juce::jmin(10, messageQueue.size());
    
    for (int i = 0; i < messagesToSend; ++i) {
        const auto& msg = messageQueue[i];
        sendDreamNrpn(msg.channel, msg.nrpn, msg.value);
    }
    
    messageQueue.removeRange(0, messagesToSend);
    
    if (onMessagesSent) {
        onMessagesSent(messagesToSend);
    }
}
```

### 5.3 UI Event Handling
```cpp
// In MainComponent.cpp
void MainComponent::sliderValueChanged(juce::Slider* slider) {
    if (!parameterManager || !midiController || !midiController->isConnected()) {
        return;
    }
    
    // Map slider to parameter ID
    juce::String paramId = sliderToParamMap[slider];
    if (paramId.isEmpty()) {
        return;
    }
    
    // Get value and update parameter
    float value = static_cast<float>(slider->getValue());
    parameterManager->setParameterValue(paramId, value);
    
    // If channels are linked, update corresponding parameter
    if (linkChannelsButton.getToggleState()) {
        juce::String linkedParamId = getLinkedParameterId(paramId);
        if (!linkedParamId.isEmpty()) {
            parameterManager->setParameterValue(linkedParamId, value);
            
            // Update linked slider if it exists
            juce::Slider* linkedSlider = paramToSliderMap[linkedParamId];
            if (linkedSlider && linkedSlider != slider) {
                linkedSlider->setValue(value, juce::dontSendNotification);
            }
        }
    }
}
```

## 6. Build Configuration

### 6.1 Projucer Project Updates
```xml
<!-- In SAM5504EvalGUI.jucer -->
<JUCEPROJECT>
  <MAINGROUP>
    <GROUP name="Source">
      <FILE id="Main.cpp" compile="1"/>
      <FILE id="ParameterManager.cpp" compile="1"/>
      <FILE id="ParameterManager.h" compile="0"/>
      <FILE id="MidiController.cpp" compile="1"/>
      <FILE id="MidiController.h" compile="0"/>
    </GROUP>
  </MAINGROUP>
  
  <EXPORTFORMATS>
    <VS2022 targetFolder="Builds/VisualStudio2022">
      <CONFIGURATIONS>
        <CONFIGURATION name="Debug" isDebug="1" optimisation="0"/>
        <CONFIGURATION name="Release" isDebug="0" optimisation="2"/>
      </CONFIGURATIONS>
    </VS2022>
  </EXPORTFORMATS>
  
  <MODULEPATHS>
    <PATH id="juce_module_path">../../JUCE/modules</PATH>
  </MODULEPATHS>
  
  <MODULES>
    <MODULE id="juce_core"/>
    <MODULE id="juce_events"/>
    <MODULE id="juce_graphics"/>
    <MODULE id="juce_gui_basics"/>
    <MODULE id="juce_gui_extra"/>
    <MODULE id="juce_audio_devices"/>
    <MODULE id="juce_audio_formats"/>
    <MODULE id="juce_audio_processors"/>
    <MODULE id="juce_audio_utils"/>
  </MODULES>
</JUCEPROJECT>
```

### 6.2 Compiler Settings
- **C++ Standard:** C++17
- **Warning Level:** Level 4 (/W4)
- **Treat Warnings as Errors:** Yes
- **Optimization:** Debug: None, Release: Full (/O2)
- **Runtime Library:** Multi-threaded DLL (/MD)

## 7. Testing Strategy

### 7.1 Unit Tests
```cpp
// ParameterManagerTests.cpp
TEST(ParameterManager, DbToSamGainConversion) {
    EXPECT_NEAR(ParameterManager::dbToSamGain(0.0f), 0x4000, 1.0f);
    EXPECT_NEAR(ParameterManager::dbToSamGain(-24.0f), 0x1000, 1.0f);
    EXPECT_NEAR(ParameterManager::dbToSamGain(15.0f), 0x5e00, 1.0f);
}

TEST(ParameterManager, HzToSamFreqConversion) {
    EXPECT_NEAR(ParameterManager::hzToSamFreq(20.0f), 0x0000, 10.0f);
    EXPECT_NEAR(ParameterManager::hzToSamFreq(1000.0f), 0x4000, 10.0f);
    EXPECT_NEAR(ParameterManager::hzToSamFreq(20000.0f), 0x7FFF, 10.0f);
}
```

### 7.2 Integration Tests
- MIDI communication with virtual MIDI port
- Preset file save/load round-trip
- UI responsiveness under load
- Memory usage profiling

### 7.3 Hardware Tests
- Connect to actual SAM5504 hardware
- Verify all NRPN parameters work correctly
- Test real-time parameter updates
- Validate audio processing changes

## 8. Performance Considerations

### 8.1 Memory Usage
- **Parameter storage:** ~100 parameters × 100 bytes = 10KB
- **UI components:** ~200 components × 1KB = 200KB
- **Message queue:** 1000 messages × 16 bytes = 16KB
- **Total estimated:** < 50MB

### 8.2 CPU Usage
- **UI rendering:** < 1% at 60 FPS
- **MIDI processing:** < 1% at 100 messages/second
- **Parameter calculations:** < 0.1%
- **Total estimated:** < 5% on modern CPU

### 8.3 Latency
- **UI event to MIDI message:** < 1ms
- **MIDI message transmission:** < 5ms (USB latency)
- **Total parameter update latency:** < 10ms

## 9. Deployment Plan

### 9.1 Build Artifacts
- `SAM5504EvalGUI.exe` (Windows executable)
- `README.txt` (updated instructions)
- `Presets/` (folder with example presets)
- `CHANGELOG.md` (version history)

### 9.2 Installation
1. Copy executable to desired location
2. Ensure JUCE runtime dependencies are available
3. Connect SAM5504 via USB
4. Run application

### 9.3 Updates
- Increment version number in application
- Maintain backward compatibility with presets
- Provide migration path for existing settings

## 10. Risk Mitigation

### 10.1 Technical Risks
- **Risk:** JUCE component performance with many controls
  - **Mitigation:** Use efficient layout, lazy loading, virtual scrolling
- **Risk:** MIDI message queue overflow
  - **Mitigation:** Implement priority queue, rate limiting
- **Risk:** Parameter value mapping errors
  - **Mitigation:** Comprehensive unit tests, validation

### 10.2 Usability Risks
- **Risk:** Overwhelming interface
  - **Mitigation:** Tabbed interface, progressive disclosure
- **Risk:** Confusing parameter organization
  - **Mitigation:** Clear grouping, tooltips, documentation

---

*Document Version: 1.0*  
*Last Updated: Current*  
*Status: Design Complete - Ready for Implementation*
