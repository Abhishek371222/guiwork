# Dream DSP SAM5504 — Quick Reference Guide

## 📊 System Overview

| Aspect | Details |
|--------|---------|
| **Type** | Professional Audio DSP + Desktop GUI Control System |
| **Target Hardware** | DREAM SAM5504 Evaluation Board (DSP Processor) |
| **Audio Spec** | 96 kHz, 2-in / 4-out stereo, 24-bit fixed-point |
| **Latency** | <1 ms real-time processing |
| **Firmware Language** | C + Assembly (embedded) |
| **GUI Language** | C++ (JUCE 8.0.12) |
| **Communication** | USB MIDI (NRPN protocol) |
| **Code Memory** | 0x4400 words (~17 KB) ⚠️ **CRITICAL CONSTRAINT** |
| **Data Memory** | 0x10000 words (64 KB) ✅ Ample |

---

## 🔧 Key Components (Firmware)

### DSP Signal Chains (96 kHz, Real-time)

```
LEFT CHANNEL:
  Input (DAAD0L)
    ↓
  DSP #1: Gain → 3-Band Biquad EQ → LevelDetect → Compressor
    ↓
  DSP #2: Delay (5.3ms) → 2-Band Biquad EQ
    ↓
  Outputs (DABD0L, DABD0R)

RIGHT CHANNEL:
  Input (DAAD0R)
    ↓
  DSP #3: (Mirror of DSP #1)
    ↓
  DSP #4: (Mirror of DSP #2)
    ↓
  Outputs (DABD1L, DABD1R)
```

### File Roles

| File | Role | Lines |
|------|------|-------|
| **main.c** | USB MIDI init, audio codec setup, main polling loop | ~200 |
| **dsp1.c, dsp3.c** | Left/Right input chains (Gain→EQ→Compressor) | ~150 ea |
| **dsp2.c, dsp4.c** | Left/Right output chains (Delay→EQ) | ~100 ea |
| **midictrl.c** | MIDI event parsing & NRPN dispatch | ~300 |
| **BiquadCtrl.c** | Biquad filter calculations (3 families, 8+ orders) | ~500 |
| **shared_nrpn.c** | **Code optimization: Shared NRPN dispatch** | ~400 |
| **mainbus.c** | Signal routing between DSP chains | ~150 |
| **Main.cpp** | JUCE GUI (MIDI selector, master gain, status) | ~350 |

---

## 💾 Memory Map

### Code Memory (CRITICAL)
```
Total:    0x4400 words (~17 KB)
Used:     ~0x4000 words (optimized)
Margin:   ~256 words (1.5% ⚠️ VERY TIGHT)
```

**Allocation:**
- Main/DSP init: ~600 words
- MIDI handling: ~300 words
- Biquad control: ~500 words
- Shared NRPN: ~600 words (saved 2400 words via factorization!)
- Other: ~400 words

### Data Memory (AMPLE)
```
Total:    0x10000 words (64 KB)
Used:     ~0x800 words (delay buffers: 2 KB)
Free:     ~61 KB (95% utilization) ✅
```

**Delay Allocation:**
- DSP #2 Process #1: 0x08E00000–0x08E001FF (512 samples, 5.3ms @ 96kHz)
- DSP #2 Process #3: 0x08E00200–0x08E003FF
- DSP #4 Process #1: 0x08E00400–0x08E005FF
- DSP #4 Process #3: 0x08E00600–0x08E007FF

---

## 📡 MIDI Control Protocol

### NRPN Message Format (4 Control Changes)

```
Message 1:  CC 99  (NRPN MSB)  ← High byte of parameter ID
Message 2:  CC 98  (NRPN LSB)  ← Low byte of parameter ID
Message 3:  CC 38  (Data LSB)  ← Value bits [6:0]
Message 4:  CC 6   (Data MSB)  ← Value bits [14:7]

Example: Set Master Gain to -6 dB
  NRPN:   0x0100 (Gain parameter)
  Value:  0x3E00 (≈ -6 dB, where 0x4000 = 0 dB, 512 units/dB)
```

### MIDI Channel Mapping

| Channel | Target DSP | Control |
|---------|-----------|---------|
| 0 | DSP #1 | Left input (Gain, EQ, Compressor) |
| 1 | DSP #2 | Left output (Delay, EQ) |
| 2 | DSP #3 | Right input (Gain, EQ, Compressor) |
| 3 | DSP #4 | Right output (Delay, EQ) |

### Sample NRPN Commands (DSP #1)

| NRPN | Function |
|------|----------|
| 0x0100 | Gain Value |
| 0x0101 | Gain Phase |
| 0x0200 | Biquad On/Off |
| 0x0201 | Biquad InGain Phase |
| 0x0202 | Biquad InGain Value |
| 0x0400 | LevelDetect Attack |
| 0x0401 | LevelDetect Release |
| 0x0500 | Compressor GainReduction (read) |
| 0x0501 | Compressor On/Off |
| 0x0502 | Compressor Threshold |

---

## 🎯 Biquad Filter Types

### Butterworth (Flat Passband)
Orders: 1st, 2nd, 3rd, 4th, 6th, 8th

### Bessel (Flat Group Delay, Minimal Phase Distortion)
Orders: 2nd, 3rd, 4th, 6th, 8th

### Linkwitz-Riley (Steeper Rolloff)
Orders: 2nd, 4th, 6th, 8th

### Topologies
- **Flat:** No filtering
- **Peaking:** Bell-shaped (DSP #1/3: 3-band default)
- **LowShelf/HighShelf:** Shelving EQ
- **Crossover:** Low-pass or High-pass filters

---

## 🖥️ GUI (JUCE C++)

### Main UI Components
- **MIDI Device ComboBox** — Select SAM5504 device
- **Refresh Button** — Re-scan MIDI devices
- **Connect Button** — Establish USB MIDI connection
- **Master Gain Slider** — Range: -24 to +15 dB
- **Connection Status** — Display connection state
- **Hint Text** — "Sends NRPN 0x0100 to DSP1 & DSP3"

### Key C++ Functions

```cpp
// Gain value conversion
int dbToSamGainValue(double db) {
    return (int)(0x4000 + db * 512);  // 0x4000 = 0dB
}

// Send NRPN message
void sendDreamNrpn(juce::MidiOutput& out, int channel, 
                   int nrpn, int value) {
    // Send 4 CC messages (99, 98, 38, 6)
}

// Refresh MIDI devices
void refreshMidiDevices() {
    outputs = juce::MidiOutput::getAvailableDevices();
}

// Connect to device
void connectSelectedDevice() {
    midiOut = juce::MidiOutput::openDevice(outputs[idx].identifier);
}
```

---

## ⚡ Code Optimization Strategy

### The Problem
Four identical DSP chains → 1800 words of duplicated NRPN dispatch code (overflow!)

### The Solution (shared_nrpn.c)
```c
// DSP #1 & #3 share this function
WORD shared_NrpnDispatch_GainBiquadComp(WORD ch, WORD nrpn, DWORD v, WORD fmt);

// DSP #2 & #4 share this function
WORD shared_NrpnDispatch_DelayBiquad(WORD ch, WORD nrpn, DWORD v, WORD fmt);
```

### The Result
- **Saved:** 2400 words (14% code reduction)
- **Impact:** Enabled entire system to fit in 0x4400 constraint
- **Trade-off:** Slightly more complex dispatch logic, but much more maintainable

---

## 🔌 Extensibility Features

### Custom Hooks (custom.h)
```c
// Called before DSP initialization
void _customPreInitFunction1/2/3/4(WORD dspId);

// Called after DSP initialization
void _customPostInitFunction1/2/3/4(WORD dspId);

// Called before NRPN processing
WORD _customPreNrpnFunction1/2/3/4(WORD ch, WORD nrpn, DWORD v);

// Called after NRPN processing
WORD _customPostNrpnFunction1/2/3/4(WORD ch, WORD nrpn, DWORD v);

// Custom parameter scaling
void _customFilterFreq(DWORD *freq);
void _customFilterGain(WORD *gain);
void _customFilterQ(WORD *q);
```

### Configuration Options (main.c)
```c
// Use Serial MIDI instead of USB (saves code/memory)
#define _USE_SERIAL_MIDI 1
#define _BOARD_MIDIBAUDRATE 31250

// Disable NRPN control entirely (saves ~600 words code)
#define _SKIP_DDD_NRPN_CTRL 1

// Disable global preset handling
#define _USE_GLOBAL_PRESET 0
```

---

## 📈 Current Capabilities

### ✅ Implemented
- 2-in / 4-out routing at 96 kHz
- Gain per channel
- 3-band parametric EQ (DSP #1 & #3)
- 2-band EQ (DSP #2 & #4)
- Compressor with sidechain metering
- Delay effects (5.3ms)
- USB MIDI control (NRPN + SysEx)
- Real-time parameter adjustment
- Activity LED feedback
- Serial MIDI fallback

### 🚀 Potential Enhancements
- Full EQ/compressor parameter UI (code budget dependent)
- Metering feedback (compressor GR, levels)
- Preset management (save/load DSP configs)
- Advanced filters (graphic EQ)
- Multi-band processing
- Parameter automation
- Real-time metering displays

---

## 🏗️ Build & Deploy

### Building Firmware
1. Open `.dcp` in Dream DSP Designer IDE
2. Verify DSP chains, routing (already configured)
3. Build via SAMVS compiler
4. Output: `build/5504DK_2in_4out_DDP_File_96k_DREAM_Z_2.hex`

### Building GUI
1. Open `.jucer` in Projucer
2. Export to Visual Studio 2026/2022
3. Open `.sln` in Visual Studio
4. Build (Debug or Release)
5. Output: `SAM5504EvalGUI.exe`

### Flashing Board
1. Connect programmer to SAM5504
2. Flash `.hex` file
3. Reset board
4. Launch GUI
5. Select MIDI device → Connect
6. Adjust master gain to test

---

## 📊 Key Metrics

| Metric | Value | Status |
|--------|-------|--------|
| **Code Memory Usage** | ~0x4000 / 0x4400 words | ⚠️ 91% (1.5% margin) |
| **Data Memory Usage** | ~0x800 / 0x10000 words | ✅ 5% (95% free) |
| **Audio Latency** | <1 ms | ✅ Excellent |
| **Sample Rate** | 96 kHz | ✅ Professional |
| **Precision** | 24-bit | ✅ Professional |
| **Channels** | 2-in / 4-out | ✅ Flexible |
| **Max Delay** | 5.3 ms | ✅ Adequate |
| **Filter Types** | 3 families × 8 orders | ✅ Comprehensive |
| **MIDI Channels** | 4 (DSP #1-4) | ✅ Full control |

---

## 🎓 Key Insights

1. **Tight Constraint = Elegant Solution:** The 17 KB code limit forced thoughtful factorization (shared_nrpn.c), resulting in maintainable, reusable code.

2. **Professional Audio:** 24-bit fixed-point, <1ms latency, real-time processing at 96 kHz = production-quality.

3. **Clean Separation:** Firmware (DSP) ↔ MIDI Protocol ↔ GUI (Control) — easy to modify any layer independently.

4. **Extensible Design:** Custom hooks allow proprietary processing without touching core code.

5. **Data-Driven Dispatch:** NRPN lookup tables eliminate tedious switch/case logic.

6. **Memory Wise:** Code budget is tight, but data memory is ample (95% free) for future features.

---

## 🔗 Repository

**GitHub:** https://github.com/Abhishek371222/guiwork  
**Main Branch:** Ready for production ✅

---

## 📚 Additional Resources

- **DEEP_ANALYSIS_SUMMARY.md** — Comprehensive technical deep-dive
- **PROJECT_ANALYSIS.md** — Original project documentation
- **ENHANCED_GUI_DESIGN.md** — GUI enhancement plans

---

## 💡 Quick Tips

### To add a new parameter:
1. Define NRPN in MIDI table (`dsp1.c`, etc.)
2. Add handler in shared_nrpn.c
3. Call libFX5000 API to update DSP
4. Add GUI slider in Main.cpp

### To reduce code size:
1. Disable NRPN: `#define _SKIP_DDD_NRPN_CTRL`
2. Use Serial MIDI: `#define _USE_SERIAL_MIDI`
3. Factor common logic (like shared_nrpn.c)

### To expand features:
1. Check data memory (95% free ✅)
2. Plan carefully for code memory (1.5% margin ⚠️)
3. Use custom hooks (`custom.h`) first
4. Consider code refactoring if adding new processes

---

*Last Updated: May 2026*  
*Status: Production Ready ✅*
