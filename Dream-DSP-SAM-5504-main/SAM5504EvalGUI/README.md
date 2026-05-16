# SAM5504 Eval GUI

First-phase JUCE/Projucer GUI for the DREAM SAM5504 firmware in this repository.

## What works now

- Lists Windows MIDI output devices.
- Opens the selected MIDI output.
- Sends master gain changes in real time.

## Firmware mapping used

The current firmware maps MIDI channels to DSP blocks like this:

- MIDI channel 0 -> DSP1, left input chain
- MIDI channel 2 -> DSP3, right input chain

The master gain slider sends NRPN `0x0100` to both channels. That reaches `_MixPA_Gain_Value` in `dsp1.c` and `dsp3.c`.

The sender matches this firmware's NRPN parser:

- CC 99 carries the NRPN high byte.
- CC 98 carries the NRPN low byte.
- CC 38 carries value bits shifted right by 1.
- CC 6 carries value bits shifted right by 8.

The value mapping follows the firmware EQ/gain convention:

- `0x4000` = 0 dB
- `0x1000` = -24 dB
- `0x5e00` = +15 dB
- 512 units = 1 dB
- dB values are packed as `0x4000 + (dB * 512)` before being sent over NRPN

The GUI uses the same fixed-point dB mapping as the firmware parameter model, so a displayed `0 dB` value is a true unity-gain request.

## Open in Projucer

1. Open `SAM5504EvalGUI.jucer`.
2. Set the JUCE modules path if Projucer asks for it.
3. Save the project from Projucer to generate the Visual Studio solution.
4. Open the generated solution in Visual Studio.
5. Build and run.

If your Projucer does not have a Visual Studio 2026 exporter, use the Visual Studio 2022 exporter and open that solution in newer Visual Studio.
