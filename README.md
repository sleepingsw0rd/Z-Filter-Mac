# Z-Filter XL (macOS)

A dual-filter morphing biquad audio plugin with expanded UI, built with JUCE. Based on the Airwindows Z-series filters by Chris Johnson. **Mac (Apple Silicon + Intel) universal binary builds** — VST3 and AudioUnit.

> For the Windows version, see [Z-Filter XL (Windows)](https://github.com/sleepingsw0rd/Z-Filter/tree/v2-xl).

![Z-Filter XL GUI](GUI/Screenshot-XL.png)

## All Versions (macOS)

| Z-Filter Mini | Z-Filter | Z-Filter XL |
|:---:|:---:|:---:|
| ![Mini](GUI/screenshot-mini.png) | ![Main](GUI/Screenshot-main.png) | ![XL](GUI/Screenshot-XL.png) |
| Single filter, no LFO, minimal UI | Single filter, single LFO, compact UI | Dual filter, dual LFO, morph, expanded UI |
| **[Download VST3](https://github.com/sleepingsw0rd/Z-Filter-Mac/raw/mini/Releases/Z-FilterMini-Mac-VST3.zip)** | **[Download VST3](https://github.com/sleepingsw0rd/Z-Filter-Mac/raw/main/Releases/Z-Filter-Mac-VST3.zip)** | **[Download VST3](https://github.com/sleepingsw0rd/Z-Filter-Mac/raw/v2-xl/Releases/Z-FilterXL-Mac-VST3.zip)** |
| **[Download AU](https://github.com/sleepingsw0rd/Z-Filter-Mac/raw/mini/Releases/Z-FilterMini-Mac-AU.zip)** | **[Download AU](https://github.com/sleepingsw0rd/Z-Filter-Mac/raw/main/Releases/Z-Filter-Mac-AU.zip)** | **[Download AU](https://github.com/sleepingsw0rd/Z-Filter-Mac/raw/v2-xl/Releases/Z-FilterXL-Mac-AU.zip)** |
| [Documentation](https://github.com/sleepingsw0rd/Z-Filter-Mac/tree/mini#readme) | [Documentation](https://github.com/sleepingsw0rd/Z-Filter-Mac/tree/main#readme) | *You are here* |

### Installation

- **VST3**: Unzip and copy `Z-FilterXL.vst3` to `~/Library/Audio/Plug-Ins/VST3/`
- **AU**: Unzip and copy `Z-FilterXL.component` to `~/Library/Audio/Plug-Ins/Components/`

Rescan in your DAW. All three versions can run side-by-side.

## Features

- **Dual Filters (A + B)**: Independent filter type selection with 5 modes each — Lowpass, Highpass, Bandpass, Notch, Region Gate
- **Serial / Parallel Routing**: Chain filters in series (A→B) or run in parallel (A||B)
- **Filter Morphing**: Smoothly blend between Filter A and B with coefficient interpolation or region crossfade
- **Morph LFO**: Dedicated LFO for morph position with tempo sync support
- **Dual LFOs (A + B)**: Independent tempo-syncable LFOs modulating filter cutoff, with LFO Link mode
- **Cascaded Biquad Architecture**: Up to 4 stages per filter (12-48 dB/octave) via independent Poles knobs
- **Z-Output Stage**: Optional Airwindows output processing for analog character
- **Opamp Modeling Stage**: Bandwidth-limited analog op-amp simulation
- **Dot-Matrix LCD Display**: 4-row real-time parameter readout (HD44780 style)
- **Per-Sample Coefficient Smoothing**: Click-free parameter automation
- **Dry/Wet Mix + Bypass**: Full signal control with LED indicators

## Controls

### FILTERS Section

| Control | Function |
|---------|----------|
| **On** (A / B) | Enable/disable each filter independently |
| **LP / HP / BP / NT / RG** | Filter type quick-select buttons (per row) |
| **Poles** (A / B) | Filter resonance / cascade depth per filter |
| **SER / PAR** | Toggle serial (A→B) or parallel (A\|\|B) routing |

### LFO Section

| Control | Function |
|---------|----------|
| **Spd** (A / B) | LFO speed — free-running Hz or tempo-synced divisions |
| **Dpt** (A / B) | LFO modulation depth |
| **Sync** (A / B) | Toggle tempo sync per LFO |
| **Link** | Link LFO B to LFO A (speed, depth, sync, phase) |

### MORPH Section

| Control | Function |
|---------|----------|
| **On** | Enable morph mode (forces both filters active) |
| **Mrph** | Manual morph position (A↔B blend) |
| **MSpd** | Morph LFO speed |
| **MDpt** | Morph LFO depth |
| **Sync** | Morph LFO tempo sync |

### MASTER Section

| Control | Function |
|---------|----------|
| **Frequency** (large knob) | Master filter cutoff |
| **Input** | Pre-filter input gain |
| **Level** | Post-filter output gain |
| **Mix** | Dry/wet blend |
| **ZOut** | Enable Z-Output stage |
| **Byp** | Bypass all processing |

## Building

### Requirements

- macOS 11.0+ (Big Sur or later)
- Xcode 12.4+ with Command Line Tools (`xcode-select --install`)
- CMake 3.22+ (`brew install cmake`)
- JUCE 8.0.12 (fetched automatically via CMake)

### Build Steps

```bash
cmake -B build -G Xcode
cmake --build build --config Release
```

Builds universal binaries (arm64 + x86_64) by default.

Build artifacts:

- **VST3**: `build/ZFilter_artefacts/Release/VST3/Z-FilterXL.vst3`
- **AU**: `build/ZFilter_artefacts/Release/AU/Z-FilterXL.component`
- **Standalone**: `build/ZFilter_artefacts/Release/Standalone/Z-FilterXL.app`

With `COPY_PLUGIN_AFTER_BUILD` enabled, plugins are automatically installed to `~/Library/Audio/Plug-Ins/`.

## DSP Signal Flow

1. Input gain with hard clip to [-1, 1]
2. Frequency-dependent trim scaling
3. Dual LFO modulation of filter cutoff (per-sample)
4. Morph LFO modulation of morph position (per-sample)
5. Filter processing: coefficient blend (non-Region) or output crossfade (Region)
6. Up to 4 cascaded biquad stages per filter (Transposed Direct Form II)
7. Opamp stage: 15.5 kHz lowpass → polynomial soft saturation → 15.5 kHz lowpass
8. Output gain
9. Wet/dry mix blend
10. TPDF dither

## Credits

- **Filter algorithms**: Based on [Airwindows](https://www.airwindows.com/) ZLowpass2, ZHighpass2, ZBandpass2, ZNotch2 by Chris Johnson (MIT License)
- **Framework**: [JUCE](https://juce.com/) 8.0.12

## License

Filter DSP algorithms are derived from Airwindows code, licensed under the MIT License. See `Source/AirwindowsReference/FilterReference.h` for details.
