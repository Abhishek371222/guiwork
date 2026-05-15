# Dream DSP SAM5504 — Firmware + Eval Board GUI

SAM5504 evaluation board project: Dream DSP Designer firmware (96 kHz, 2-in / 4-out) and a JUCE host control application.

## Repository layout

| Path | Description |
|------|-------------|
| Root (`dsp1.c`, `main.c`, …) | **Firmware** — build with Dream DSP Designer / SAMVS, flash `build/*.hex` |
| [`SAM5504EvalGUI/`](SAM5504EvalGUI/) | **GUI** — JUCE 8 control app (USB MIDI NRPN/SysEx to the board) |

## Firmware (backend)

1. Open `5504DK_2in_4out_DDP_File_96k_DREAM_Z_2.dcp` in Dream DSP Designer.
2. Build for **5504-DK** / SAM5504 @ 96 kHz.
3. Flash `build/5504DK_2in_4out_DDP_File_96k_DREAM_Z_2.hex`.

Control protocol: USB MIDI NRPN + SysEx (`midictrl.c`). MIDI channel 0–3 maps to DSP1–4 (L Sub, L Mid, R Sub, R Mid).

## GUI (frontend)

See [SAM5504EvalGUI/README.md](SAM5504EvalGUI/README.md).

- Open `SAM5504EvalGUI/Builds/VisualStudio2026/SAM5504EvalGUI.sln` (or VS2022).
- Requires JUCE 8.0.12 (path set in `.jucer`).
- Connect USB MIDI to the eval board and use the app to adjust parameters in real time.

## Signal flow

```
DAAD0L → DSP1 (Gain/EQ/Comp) → DSP2 (Delay/EQ) → DABD0L/R
DAAD0R → DSP3 (Gain/EQ/Comp) → DSP4 (Delay/EQ) → DABD1L/R
```
