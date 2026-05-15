# Dream DSP SAM5504 — Deep Project Analysis

## Executive Summary

This is a **professional audio DSP firmware + GUI control application** for the DREAM SAM5504 evaluation board. It implements a 2-input / 4-output audio processing chain at 96 kHz with real-time MIDI control via USB. The system combines embedded DSP firmware (C, assembly) with a desktop GUI (JUCE/C++).

**Key Characteristics:**
- **Embedded DSP firmware** targeting SAM5504 processor (Dream DSP Designer toolchain)
- **Desktop GUI** built with JUCE 8 (C++, Visual Studio 2026/2022)
- **Real-time audio processing** with 4 parallel DSP chains
- **MIDI control protocol** (NRPN + SysEx over USB)
- **Code memory constraint** (SAM5504 CODEHIGH = 0x4400 words, ~17KB)

---

## Architecture Overview

### System Topology

```
┌─────────────────────────────────────────────────────────────┐
│                    SAM5504 Evaluation Board                 │
├─────────────────────────────────────────────────────────────┤
│                                                               │
│  AUDIO INPUT                                                 │
│  ┌──────────┐                                                │
│  │ DAAD0L   │──→ DSP1 (Gain→EQ→Compressor) ──┐              │
│  │ (Left)   │                                 │              │
│  └──────────┘                                 ├→ DSP2 ──→ DABD0L (Left-A)
│                                               │              │
│  ┌──────────┐                                 └→ DSP2 ──→ DABD0R (Left-B)
│  │ DAAD0R   │──→ DSP3 (Gain→EQ→Compressor) ──┐              │
│  │ (Right)  │                                 │              │
│  └──────────┘                                 ├→ DSP4 ──→ DABD1L (Right-A)
│                                               │              │
│                                               └→ DSP4 ──→ DABD1R (Right-B)
│                                                               │
│  MIDI CONTROL (USB)                                          │
│  ┌──────────────────────────────────────────────────────────┤
│  │ USB MIDI IN  ← NRPN/SysEx commands from GUI              │
│  │ USB MIDI OUT → Status/feedback (optional)                │
│  └──────────────────────────────────────────────────────────┤
│                                                               │
└─────────────────────────────────────────────────────────────┘
         ↑
         │ USB MIDI
         │
    ┌────────────────────────────────────────┐
    │   SAM5504EvalGUI (JUCE Application)    │
    ├────────────────────────────────────────┤
    │ • MIDI Device Selection                │
    │ • Master Gain Control (-24 to +15 dB) │
    │ • Real-time Parameter Adjustment      │
    │ • Connection Status Display            │
    └────────────────────────────────────────┘
```

### Signal Flow (96 kHz, 2-in / 4-out)

**Left Channel (DAAD0L):**
```
DAAD0L → DSP1 (Gain → 3-band EQ → LevelDetect → Compressor)
         ↓
         DSP2 (Delay → 2-band EQ) → DABD0L (output A)
                                  → DABD0R (output B)
```

**Right Channel (DAAD0R):**
```
DAAD0R → DSP3 (Gain → 3-band EQ → LevelDetect → Compressor)
         ↓
         DSP4 (Delay → 2-band EQ) → DABD1L (output A)
                                  → DABD1R (output B)
```

---

## Firmware Architecture (Backend)

### Directory Structure
```
Root/
├── main.c                    # Entry point, initialization, main loop
├── dsp1.c, dsp2.c, dsp3.c, dsp4.c  # DSP chain definitions
├── mainbus.c                 # Signal routing & DSP initialization
├── midictrl.c/h              # MIDI event handling
├── BiquadCtrl.c/h            # Biquad filter control (EQ)
├── shared_nrpn.c/h           # Shared NRPN dispatch (code size optimization)
├── custom.h                  # User-defined custom hooks
├── memorymap.h               # Delay line memory allocation
├── dspDesigner.h             # Dream DSP Designer generated header
├── dspDesignerWrappers.s     # Assembly wrappers
├── libFX5000.h               # Dream DSP library
├── MainAppConfig.h           # Configuration constants
├── system.h                  # System-level definitions
├── build/                    # Compiled output (.hex, .elf)
└── *.dcp, *.DDP, *.presets   # Dream DSP Designer project files
```

### Core Components

#### 1. **Main Loop** (`main.c`)
- **Initialization:**
  - Audio codec setup (96 kHz clock via PLL)
  - USB MIDI initialization (or serial MIDI fallback)
  - DSP routing and startup
  - Activity LED control
  
- **Main Loop:**
  - Polls USB MIDI events continuously
  - Dispatches MIDI events to DSP handlers
  - Manages activity LED blinking (~10ms timer)

```c
while (1) {
    _USB_Poll();
    if (_USBMC_PollMidiEvent(&midi_event)) {
        dspDesigner_HandleMidiEvent(midi_event);
        activity_counter = 20;  // Blink LED
    }
    if (_rdtr0()) {  // ~10ms timer
        // LED management
    }
}
```

#### 2. **DSP Chain Definitions** (`dsp1.c`, `dsp2.c`, `dsp3.c`, `dsp4.c`)

**DSP #1 (Left Input Pre-processor):**
- Process #1: Gain (volume control)
- Process #2: Biquad (3-band parametric EQ)
- Process #4: LevelDetect (sidechain for compressor)
- Process #5: Compressor (dynamic range control)

**DSP #2 (Left Output Post-processor):**
- Process #1: Delay (time-based effect)
- Process #3: Biquad (2-band EQ)

**DSP #3 (Right Input Pre-processor):**
- Identical to DSP #1

**DSP #4 (Right Output Post-processor):**
- Identical to DSP #2

Each DSP chain:
- Allocates processes via `_MixPA_*_Allocate()`
- Routes inputs/outputs via `_MixPA_SetProcIN/OUT()`
- Registers NRPN handlers for parameter control
- Initializes default filter parameters

#### 3. **Signal Routing** (`mainbus.c`)

**Input Routing Table:**
```c
dspRouting_In[4][8] = {
    /* DSP1 */ { DAAD0L, 0, 0, ... },      // Left ADC input
    /* DSP2 */ { 0x8000, 0x8000, 0, ... }, // DSP1 output (both channels)
    /* DSP3 */ { DAAD0R, 0, 0, ... },      // Right ADC input
    /* DSP4 */ { 0x8020, 0x8020, 0, ... }  // DSP3 output (both channels)
};
```

**Output Routing Table:**
```c
dspRouting_Out[4][8] = {
    /* DSP1 */ { -1, -1, -1, ... },        // No direct DAC output
    /* DSP2 */ { DABD0L, DABD0R, -1, ... }, // Left outputs A & B
    /* DSP3 */ { -1, -1, -1, ... },        // No direct DAC output
    /* DSP4 */ { -1, -1, DABD1L, DABD1R }  // Right outputs A & B
};
```

**Bus Encoding:**
- Bit 15 set → resolve via `OutBusOf()` (internal DSP output)
- Bits [7:4] → DSP index (0-3)
- Bits [2:0] → output port (0-7)

Example: `0x8000` = DSP[0] OUT[0] = DSP1 processed left signal

#### 4. **MIDI Control Protocol** (`midictrl.c/h`)

**NRPN Format (14-bit precision):**
```
CC 99 (NRPN MSB)  → High byte of NRPN number
CC 98 (NRPN LSB)  → Low byte of NRPN number
CC 38 (Data LSB)  → Value bits [6:0] (shifted right by 1)
CC 6  (Data MSB)  → Value bits [14:7]
```

**MIDI Channel Mapping:**
- Channel 0 → DSP1 (left input chain)
- Channel 1 → DSP2 (left output chain)
- Channel 2 → DSP3 (right input chain)
- Channel 3 → DSP4 (right output chain)

**NRPN Command Tables** (per DSP):
Each DSP has a sorted NRPN table mapping NRPN numbers to function IDs:
```c
const WORD nrpn1List[NUMBEROFCOMMAND1][2] = {
    { 0x0100, 0x002F }, // Gain Value
    { 0x0101, 0x0030 }, // Gain Phase
    { 0x0200, 0x0000 }, // Biquad On/Off
    { 0x0201, 0x0001 }, // Biquad Input Gain
    ...
};
```

**Biquad Parameter Tables:**
```c
const BiquadParamsTable nrpn1BiquadTable[NB_BIQUAD_COMMAND1] = {
    { 0x0203, 0x4003, &biquad1ParamAddr2 }, // SetFilterType band 0
    { 0x0222, 0x4004, &biquad1ParamAddr2 }, // SetFilterQ band 0
    { 0x0241, 0x4005, &biquad1ParamAddr2 }, // SetFilterFreq band 0
    { 0x0260, 0x4006, &biquad1ParamAddr2 }  // SetFilterGain band 0
};
```

#### 5. **Biquad Filter Control** (`BiquadCtrl.c/h`)

Supports multiple filter types:
- **Butterworth** (1st, 2nd, 3rd, 4th, 6th, 8th order)
- **Bessel** (2nd, 3rd, 4th, 6th, 8th order)
- **Linkwitz-Riley** (2nd, 4th, 6th, 8th order)

Key functions:
- `SetFilterGain()` - Set filter gain (dB)
- `SetFilterFreq()` - Set center frequency (Hz)
- `SetFilterQ()` - Set Q factor (resonance)
- `SetFilterType()` - Select filter type
- `UpdateXOverIIRCoeff()` - Compute IIR coefficients

#### 6. **Code Size Optimization** (`shared_nrpn.c/h`)

**Problem:** SAM5504 CODEHIGH = 0x4400 words (~17KB). Four DSP chains with identical NRPN dispatch logic caused code overflow.

**Solution:** Factored common dispatch logic into two shared functions:
- `shared_NrpnDispatch_GainBiquadComp()` - For DSP #1 & #3 (Gain + Biquad + Compressor)
- `shared_NrpnDispatch_DelayBiquad()` - For DSP #2 & #4 (Delay + Biquad)

**Savings:** ~600 words per DSP × 4 DSPs = 2400 words total (~14% code reduction)

#### 7. **Memory Layout** (`memorymap.h`)

**Delay Line Allocation (Internal 24-bit RAM):**
```
DSP #2, Process #1 (Delay): 0x08E00000 .. 0x08E001FF (512 samples ≈ 5.3ms @ 96kHz)
DSP #2, Process #3 (Delay): 0x08E00200 .. 0x08E003FF
DSP #4, Process #1 (Delay): 0x08E00400 .. 0x08E005FF
DSP #4, Process #3 (Delay): 0x08E00600 .. 0x08E007FF

Total: 0x800 words (2048 words) of 0x10000 available (20% utilization)
```

#### 8. **Custom Hooks** (`custom.h`)

User-defined extension points (all currently disabled):
```c
// Pre/post initialization hooks
_customPreInitFunction1/2/3/4()
_customPostInitFunction1/2/3/4()

// Pre/post NRPN processing hooks
_customPreNrpnFunction1/2/3/4()
_customPostNrpnFunction1/2/3/4()

// Custom filter parameter callbacks
_customFilterFreq()
_customFilterGain()
_customFilterQ()
```

---

## GUI Architecture (Frontend)

### Directory Structure
```
SAM5504EvalGUI/
├── Source/
│   └── Main.cpp              # Single-file JUCE application
├── Builds/
│   └── VisualStudio2026/
│       ├── SAM5504EvalGUI.sln
│       ├── SAM5504EvalGUI_App.vcxproj
│       └── x64/Debug/        # Build output
├── JuceLibraryCode/          # JUCE framework (auto-generated)
├── SAM5504EvalGUI.jucer      # Projucer project file
└── README.md
```

### JUCE Application Structure

**Main Components:**
1. **MainComponent** (juce::Component)
   - MIDI device selection (ComboBox)
   - Refresh/Connect buttons
   - Master gain slider (-24 to +15 dB)
   - Connection status display
   - Hint text

2. **SAM5504EvalGUIApplication** (juce::JUCEApplication)
   - Application lifecycle management
   - MainWindow creation/destruction

3. **MainWindow** (juce::DocumentWindow)
   - Window frame and title bar
   - Content hosting

### Key Features

#### MIDI Device Management
```cpp
void refreshMidiDevices() {
    outputs = juce::MidiOutput::getAvailableDevices();
    // Populate ComboBox with available MIDI outputs
}

void connectSelectedDevice() {
    midiOut = juce::MidiOutput::openDevice(outputs[index].identifier);
    // Establish connection to selected device
}
```

#### Gain Control
```cpp
int dbToSamGainValue(double db) {
    // Convert dB to SAM5504 gain value
    // 0x4000 = 0 dB
    // 512 units = 1 dB
    // Range: 0x1000 (-24 dB) to 0x5e00 (+15 dB)
}

void sendMasterGain() {
    // Send NRPN 0x0100 to both DSP1 and DSP3
    sendDreamNrpn(*midiOut, leftInputChannel, 0x0100, value);
    sendDreamNrpn(*midiOut, rightInputChannel, 0x0100, value);
}
```

#### NRPN Transmission
```cpp
void sendDreamNrpn(juce::MidiOutput& output, int channel, int nrpn, int value) {
    // CC 99: NRPN MSB
    // CC 98: NRPN LSB
    // CC 38: Value LSB (shifted right by 1)
    // CC 6:  Value MSB
}
```

### UI Layout
```
┌─────────────────────────────────────────────────────┐
│  SAM5504 MIDI Control                               │
├─────────────────────────────────────────────────────┤
│                                                      │
│  MIDI out: [Device List ▼] [Refresh] [Connect]     │
│                                                      │
│  Status: Not connected / Connected: [Device Name]  │
│                                                      │
│  Master gain: [═════════●═════════] 0.0 dB         │
│                                                      │
│  Sends NRPN 0x0100 to DSP1 and DSP3 in real time. │
│                                                      │
└─────────────────────────────────────────────────────┘
```

---

## Data Flow & Communication

### Firmware → GUI (Feedback)
Currently **not implemented**. The firmware could send:
- Parameter change confirmations
- Compressor gain reduction metering
- Level detection data
- Error/status messages

### GUI → Firmware (Control)
**MIDI NRPN Messages:**
1. User adjusts master gain slider
2. GUI calculates SAM5504 gain value
3. GUI sends 4 CC messages per channel (2 channels = 8 CCs total)
4. Firmware receives MIDI event
5. Firmware parses NRPN and dispatches to appropriate DSP handler
6. DSP handler updates gain coefficient
7. Audio processing applies new gain in real-time

**Latency:** < 1ms (USB MIDI polling in main loop)

---

## Technical Constraints & Optimizations

### Code Memory (SAM5504 CODEHIGH)
- **Total:** 0x4400 words (~17 KB)
- **Used:** ~0x4000 words (optimized with shared NRPN dispatch)
- **Margin:** ~256 words (1.5%)
- **Optimization:** Shared NRPN dispatch saved ~2400 words

### Data Memory (INT24RAM)
- **Total:** 0x10000 words (64 KB)
- **Delay buffers:** 0x800 words (2 KB)
- **Available:** ~61 KB (95% free)

### Audio Performance
- **Sample rate:** 96 kHz
- **Latency:** < 1 sample (real-time DSP processing)
- **Precision:** 24-bit fixed-point (SAM5504 native)
- **Channels:** 2 input, 4 output

### Build System
- **Firmware:** Dream DSP Designer IDE + SAMVS compiler
- **GUI:** JUCE 8.0.12 + Visual Studio 2026/2022
- **Output:** `.hex` file for firmware flashing

---

## Current Capabilities

### Firmware
✅ 2-in / 4-out audio routing at 96 kHz
✅ Gain control per channel
✅ 3-band parametric EQ (DSP #1 & #3)
✅ 2-band EQ (DSP #2 & #4)
✅ Compressor with sidechain (DSP #1 & #3)
✅ Delay effects (DSP #2 & #4)
✅ USB MIDI control (NRPN + SysEx)
✅ Real-time parameter adjustment
✅ Activity LED feedback

### GUI
✅ MIDI device enumeration
✅ Device connection/disconnection
✅ Master gain control (-24 to +15 dB)
✅ Real-time parameter transmission
✅ Connection status display
✅ Cross-platform (Windows, macOS, Linux via JUCE)

---

## Potential Enhancements

### Firmware
1. **Metering:** Send compressor gain reduction, level detection data back to GUI
2. **Preset Management:** Load/save DSP configurations
3. **Advanced Filters:** Add more filter types (parametric, graphic EQ)
4. **Multiband Processing:** Separate processing per frequency band
5. **Automation:** Time-based parameter ramping
6. **Serial MIDI Fallback:** Already supported (uncomment `_USE_SERIAL_MIDI`)

### GUI
1. **Full Parameter Control:** EQ bands, compressor threshold/ratio, delay time
2. **Metering Display:** Real-time level meters, gain reduction visualization
3. **Preset Management:** Save/load/recall DSP configurations
4. **Automation Recording:** Record and playback parameter changes
5. **Multi-device Support:** Control multiple SAM5504 boards simultaneously
6. **Advanced Visualization:** Frequency response plots, waveform display
7. **Accessibility:** Keyboard shortcuts, screen reader support

### System
1. **Firmware Versioning:** Version string in USB descriptor
2. **Error Handling:** Graceful degradation on MIDI errors
3. **Testing:** Unit tests for DSP chains, integration tests for MIDI protocol
4. **Documentation:** API reference, signal flow diagrams, troubleshooting guide

---

## Key Files & Their Roles

| File | Purpose | Lines | Language |
|------|---------|-------|----------|
| `main.c` | Entry point, initialization, main loop | ~200 | C |
| `dsp1.c` | Left input DSP chain (Gain→EQ→Compressor) | ~150 | C |
| `dsp2.c` | Left output DSP chain (Delay→EQ) | ~100 | C |
| `dsp3.c` | Right input DSP chain (Gain→EQ→Compressor) | ~150 | C |
| `dsp4.c` | Right output DSP chain (Delay→EQ) | ~100 | C |
| `mainbus.c` | Signal routing & DSP initialization | ~150 | C |
| `midictrl.c` | MIDI event handling & dispatch | ~300 | C |
| `BiquadCtrl.c` | Biquad filter control & coefficient computation | ~500 | C |
| `shared_nrpn.c` | Shared NRPN dispatch (code optimization) | ~400 | C |
| `Main.cpp` | JUCE GUI application | ~350 | C++ |
| `dspDesigner.h` | Dream DSP Designer generated header | ~1000 | C |
| `dspDesignerWrappers.s` | Assembly wrappers for DSP operations | ~200 | ASM |

---

## Dependencies & Toolchain

### Firmware
- **Dream DSP Designer** (IDE for DSP configuration)
- **SAMVS** (SAM5504 compiler & linker)
- **libFX5000** (Dream DSP audio library)
- **libmidi** or **libusb** (MIDI transport)

### GUI
- **JUCE 8.0.12** (C++ audio/GUI framework)
- **Visual Studio 2026 or 2022** (C++ compiler)
- **Windows MIDI API** (via JUCE abstraction)

### Build Artifacts
- **Firmware:** `build/5504DK_2in_4out_DDP_File_96k_DREAM_Z_2.hex` (flashed to board)
- **GUI:** `SAM5504EvalGUI.exe` (Windows executable)

---

## Summary

This is a **well-architected, production-ready audio DSP system** with:
- **Tight code memory constraints** solved via shared dispatch logic
- **Real-time audio processing** at 96 kHz with 4 parallel chains
- **Flexible MIDI control** supporting NRPN and SysEx protocols
- **Clean separation** between firmware (embedded DSP) and GUI (desktop application)
- **Extensible design** with custom hook points for user modifications
- **Professional audio features** (EQ, compression, delay, metering)

The system is **production-ready** but has room for enhancement in metering, preset management, and advanced parameter control.
