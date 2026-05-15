# Enhanced GUI with Full Parameter Control - Requirements Document

## 1. Overview

**Project:** Enhanced SAM5504 Eval GUI with Full Parameter Control  
**Type:** Feature Enhancement  
**Target:** Current JUCE GUI application (`SAM5504EvalGUI/Source/Main.cpp`)  
**Goal:** Extend the existing GUI to control all available DSP parameters (EQ, compressor, delay) in addition to master gain.

## 2. Current State Analysis

### 2.1 Existing GUI Capabilities
- ✅ MIDI device selection and connection
- ✅ Master gain control (-24 to +15 dB)
- ✅ Real-time NRPN transmission (NRPN 0x0100)
- ✅ Connection status display
- ✅ Basic UI layout (560×320 pixels)

### 2.2 Current Limitations
- ❌ Only controls master gain (NRPN 0x0100)
- ❌ No EQ band controls (3 bands per DSP1/DSP3, 2 bands per DSP2/DSP4)
- ❌ No compressor controls (threshold, ratio, attack, release)
- ❌ No delay controls (time, gain, phase)
- ❌ No channel selection (left/right independent control)
- ❌ No visual feedback of parameter values

## 3. Available DSP Parameters (From Firmware Analysis)

### 3.1 DSP #1 & #3 (Input Chains - Gain→EQ→Compressor)

#### Gain Control
- **NRPN 0x0100**: `_MixPA_Gain_Value` (master gain)
- **NRPN 0x0101**: `_MixPA_Gain_Phase` (gain phase)

#### Biquad EQ (3 bands per DSP)
- **NRPN 0x0200**: `_MixPA_Biquad_OnOff` (enable/disable)
- **NRPN 0x0201**: `_MixPA_Biquad_InGainPhase` (input gain phase)
- **NRPN 0x0202**: `_MixPA_Biquad_InGainValue` (input gain value)

**Per-band controls (band 0):**
- **NRPN 0x0203**: `SetFilterType` (filter type selection)
- **NRPN 0x0222**: `SetFilterQ` (Q factor/resonance)
- **NRPN 0x0241**: `SetFilterFreq` (center frequency)
- **NRPN 0x0260**: `SetFilterGain` (gain in dB)

#### LevelDetect (Compressor Sidechain)
- **NRPN 0x0400**: `_MixPA_LevelDetect_Attack` (attack time)
- **NRPN 0x0401**: `_MixPA_LevelDetect_Release` (release time)

#### Compressor
- **NRPN 0x0500**: `_MixPA_Compressor_GetGainReduction` (read-only metering)
- **NRPN 0x0501**: `_MixPA_Compressor_OnOff` (enable/disable)
- **NRPN 0x0502**: `_MixPA_Compressor_Threshold` (compression threshold)
- **NRPN 0x0503**: `_MixPA_Compressor_Ratio` (compression ratio)
- **NRPN 0x0504**: `_MixPA_Compressor_Boost` (makeup gain)
- **NRPN 0x0505**: `_MixPA_Compressor_BoostPhase` (boost phase)

### 3.2 DSP #2 & #4 (Output Chains - Delay→EQ)

#### Delay Process #1 (Channel A)
- **NRPN 0x0100**: `_MixPA_Delay_OnOff` (enable/disable)
- **NRPN 0x0101**: `_MixPA_Delay_SetTime` (delay time)
- **NRPN 0x0102**: `_MixPA_Delay_OutGainValue` (output gain)
- **NRPN 0x0103**: `_MixPA_Delay_OutGainPhase` (output phase)

#### Biquad EQ Process #2 (Channel A, 2 bands)
- **NRPN 0x0200**: `_MixPA_Biquad_OnOff` (enable/disable)
- **NRPN 0x0201**: `_MixPA_Biquad_InGainPhase` (input gain phase)
- **NRPN 0x0202**: `_MixPA_Biquad_InGainValue` (input gain value)

**Per-band controls (band 0):**
- **NRPN 0x0203**: `SetFilterType` (filter type)
- **NRPN 0x0222**: `SetFilterQ` (Q factor)
- **NRPN 0x0241**: `SetFilterFreq` (frequency)
- **NRPN 0x0260**: `SetFilterGain` (gain)

#### Delay Process #3 (Channel B)
- **NRPN 0x0300**: `_MixPA_Delay_OnOff` (enable/disable)
- **NRPN 0x0301**: `_MixPA_Delay_SetTime` (delay time)
- **NRPN 0x0302**: `_MixPA_Delay_OutGainValue` (output gain)
- **NRPN 0x0303**: `_MixPA_Delay_OutGainPhase` (output phase)

#### Biquad EQ Process #4 (Channel B, 2 bands)
- **NRPN 0x0400**: `_MixPA_Biquad_OnOff` (enable/disable)
- **NRPN 0x0401**: `_MixPA_Biquad_InGainPhase` (input gain phase)
- **NRPN 0x0402**: `_MixPA_Biquad_InGainValue` (input gain value)

**Per-band controls (band 0):**
- **NRPN 0x0403**: `SetFilterType` (filter type)
- **NRPN 0x0422**: `SetFilterQ` (Q factor)
- **NRPN 0x0441**: `SetFilterFreq` (frequency)
- **NRPN 0x0460**: `SetFilterGain` (gain)

## 4. Functional Requirements

### 4.1 Core Requirements

**FR-001: Channel Selection**
- User can select which channel(s) to control: Left (DSP1/DSP2) or Right (DSP3/DSP4)
- Option to control both channels simultaneously (linked mode)
- Visual indication of active channel(s)

**FR-002: Gain Control Enhancement**
- Maintain existing master gain control
- Add input gain phase control (NRPN 0x0101)
- Add output gain controls for delay blocks (NRPN 0x0102, 0x0302)

**FR-003: EQ Band Controls**
- For DSP1/DSP3: 3-band parametric EQ controls
- For DSP2/DSP4: 2-band parametric EQ controls per channel
- Per-band controls: Frequency, Gain, Q, Filter Type
- Enable/disable toggle per EQ band

**FR-004: Compressor Controls**
- Threshold control (NRPN 0x0502)
- Ratio control (NRPN 0x0503)
- Attack time control (NRPN 0x0400)
- Release time control (NRPN 0x0401)
- Makeup gain control (NRPN 0x0504)
- Enable/disable toggle (NRPN 0x0501)

**FR-005: Delay Controls**
- Delay time control per channel (NRPN 0x0101, 0x0301)
- Delay output gain per channel (NRPN 0x0102, 0x0302)
- Delay output phase per channel (NRPN 0x0103, 0x0303)
- Enable/disable toggle per delay block

**FR-006: Real-time Parameter Updates**
- All parameter changes transmitted immediately via MIDI NRPN
- Maintain existing NRPN transmission protocol (CC 99, 98, 38, 6)
- Support 14-bit precision format (0x0000-0x7FFF range)

### 4.2 UI/UX Requirements

**FR-007: Tabbed Interface**
- Tab 1: Gain & EQ Controls
- Tab 2: Compressor Controls
- Tab 3: Delay Controls
- Tab 4: Global Settings

**FR-008: Responsive Layout**
- Window size: 800×600 pixels (expanded from current 560×320)
- Responsive component arrangement
- Clear visual hierarchy and grouping

**FR-009: Visual Feedback**
- Current parameter values displayed numerically
- Slider positions reflect current values
- Connection status prominently displayed
- Channel selection visual feedback

**FR-010: Preset Management**
- Save current parameter set to file
- Load parameter set from file
- Factory reset to default values
- Quick preset buttons (e.g., "Flat", "Vocal", "Bass")

### 4.3 Technical Requirements

**FR-011: MIDI Communication**
- Maintain backward compatibility with existing MIDI device selection
- Support multiple MIDI output devices
- Handle device disconnection/reconnection gracefully
- Queue MIDI messages to avoid USB buffer overflow

**FR-012: Parameter Mapping**
- Map GUI values to SAM5504 NRPN values correctly
- Handle value ranges and conversions (dB ↔ SAM units)
- Support both 14-bit and 28-bit precision where applicable

**FR-013: State Management**
- Store current parameter values locally
- Track which parameters have been modified
- Support undo/redo for parameter changes
- Auto-save settings on exit

**FR-014: Performance**
- UI updates at 60 FPS minimum
- MIDI transmission latency < 10ms
- Memory usage < 100MB
- CPU usage < 5% when idle

## 5. Non-Functional Requirements

### 5.1 Usability
**NFR-001:** Intuitive interface for audio engineers
**NFR-002:** Clear visual grouping of related controls
**NFR-003:** Tooltips for all controls explaining function
**NFR-004:** Keyboard shortcuts for common operations
**NFR-005:** Accessible color scheme and font sizes

### 5.2 Reliability
**NFR-006:** No crashes during MIDI device changes
**NFR-007:** Graceful handling of disconnected MIDI devices
**NFR-008:** Parameter values persist across application restarts
**NFR-009:** No memory leaks after 24+ hours of operation

### 5.3 Compatibility
**NFR-010:** Works with existing SAM5504 firmware (no firmware changes required)
**NFR-011:** Compatible with Windows 10/11 (primary target)
**NFR-012:** Buildable with JUCE 8.0.12 and Visual Studio 2022/2026
**NFR-013:** Supports standard MIDI interfaces (USB MIDI, virtual MIDI ports)

### 5.4 Performance
**NFR-014:** Application starts in < 2 seconds
**NFR-015:** UI responds to user input within 50ms
**NFR-016:** MIDI messages transmitted within 5ms of user action
**NFR-017:** Memory footprint < 50MB typical

## 6. User Stories

### 6.1 Primary User: Audio Engineer
**US-001:** As an audio engineer, I want to adjust EQ bands for the left and right channels independently, so I can fine-tune the frequency response for my speakers.

**US-002:** As an audio engineer, I want to control compressor settings (threshold, ratio, attack, release), so I can manage dynamic range and prevent clipping.

**US-003:** As an audio engineer, I want to adjust delay times for each output channel, so I can time-align multiple speakers in a sound system.

**US-004:** As an audio engineer, I want to save and recall parameter presets, so I can quickly switch between different configurations for different use cases.

### 6.2 Secondary User: System Integrator
**US-005:** As a system integrator, I want to see all available parameters in one interface, so I can configure the entire DSP system without using multiple tools.

**US-006:** As a system integrator, I want clear visual feedback of parameter values, so I can verify settings and troubleshoot issues.

**US-007:** As a system integrator, I want the interface to be responsive and reliable, so I can work efficiently during system setup and calibration.

## 7. Technical Constraints

### 7.1 Existing Codebase Constraints
- Must extend `MainComponent` class in `Main.cpp`
- Must maintain existing MIDI communication functions (`sendDreamNrpn`, `dbToSamGainValue`)
- Must preserve current window management and application lifecycle
- Cannot change firmware NRPN protocol (fixed by hardware)

### 7.2 JUCE Framework Constraints
- UI components must use JUCE widgets (Slider, ComboBox, Button, Label)
- Layout must use JUCE's FlexBox or manual bounds calculation
- MIDI communication must use JUCE's `MidiOutput` class
- Thread safety: UI updates must happen on message thread

### 7.3 SAM5504 Firmware Constraints
- NRPN values: 14-bit range (0x0000-0x7FFF)
- Gain mapping: 0x4000 = 0 dB, 512 units = 1 dB
- MIDI channels: 0=DSP1, 1=DSP2, 2=DSP3, 3=DSP4
- Parameter update rate: Limited by USB MIDI bandwidth

## 8. Success Criteria

### 8.1 Must Have (MVP)
- [ ] All NRPN parameters from DSP1-DSP4 accessible via GUI
- [ ] Tabbed interface for different parameter groups
- [ ] Channel selection (Left/Right/Both)
- [ ] Real-time parameter transmission
- [ ] Builds and runs without errors

### 8.2 Should Have
- [ ] Preset save/load functionality
- [ ] Visual parameter value display
- [ ] Tooltips for all controls
- [ ] Responsive UI layout

### 8.3 Nice to Have
- [ ] Undo/redo functionality
- [ ] Parameter automation recording
- [ ] Metering display (if firmware supports feedback)
- [ ] Skin/theming options
- [ ] Multi-language support

## 9. Risks and Mitigations

### 9.1 Technical Risks
**Risk 1:** USB MIDI bandwidth limitations causing parameter update lag
- **Mitigation:** Implement message queuing with priority system
- **Mitigation:** Batch parameter updates where possible

**Risk 2:** JUCE UI performance with many controls
- **Mitigation:** Use efficient layout (FlexBox)
- **Mitigation:** Lazy-load tab contents
- **Mitigation:** Optimize repaint regions

**Risk 3:** Parameter value mapping errors
- **Mitigation:** Comprehensive unit tests for value conversions
- **Mitigation:** Validation against firmware documentation
- **Mitigation:** Log NRPN messages for debugging

### 9.2 Usability Risks
**Risk 4:** Overwhelming interface with too many controls
- **Mitigation:** Logical grouping and tab organization
- **Mitigation:** Progressive disclosure (advanced vs basic mode)
- **Mitigation:** Clear visual hierarchy

**Risk 5:** Confusion between similar parameters
- **Mitigation:** Clear labeling with DSP/process context
- **Mitigation:** Color coding by channel
- **Mitigation:** Interactive help/tooltips

## 10. Dependencies

### 10.1 External Dependencies
- JUCE 8.0.12 framework
- Visual Studio 2022 or 2026 compiler
- Windows MIDI API (via JUCE)
- SAM5504 firmware (must match NRPN protocol)

### 10.2 Internal Dependencies
- Existing `Main.cpp` structure
- Current MIDI communication functions
- NRPN parameter tables from firmware
- Build system (Projucer project)

## 11. Out of Scope

- Firmware modifications
- Audio signal visualization (oscilloscope, spectrum analyzer)
- Multi-board control (single SAM5504 only)
- Network/remote control
- Scripting/automation API
- Mobile/tablet version
- Linux/macOS specific optimizations (Windows primary)

## 12. Next Steps

1. **Design Phase:** Create UI mockups and component architecture
2. **Implementation Phase:** 
   - Extend `MainComponent` with new controls
   - Implement parameter mapping and NRPN transmission
   - Add tabbed interface and layout
   - Implement preset management
3. **Testing Phase:**
   - Unit tests for value conversions
   - Integration tests with SAM5504 hardware
   - Usability testing with target users
4. **Deployment Phase:**
   - Update Projucer project file
   - Build and package executable
   - Update documentation

---

*Document Version: 1.0*  
*Last Updated: Current*  
*Status: Requirements Complete - Ready for Design Phase*
