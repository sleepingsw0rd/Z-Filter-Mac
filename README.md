# Z-Filter Mini (macOS)

A streamlined single-knob filter plugin built with JUCE, based on the Airwindows Z-series filters by Chris Johnson. Minimal controls, maximum tone. **Mac (Apple Silicon + Intel) universal binary builds** — VST3 and AudioUnit.

> For the Windows version, see [Z-Filter Mini (Windows)](https://github.com/sleepingsw0rd/Z-Filter/tree/mini).

![Z-Filter Mini GUI](GUI/screenshot-mini.png)

## All Versions (macOS)

| Z-Filter Mini | Z-Filter | Z-Filter XL |
|:---:|:---:|:---:|
| ![Mini](GUI/screenshot-mini.png) | ![Main](GUI/Screenshot-main.png) | ![XL](GUI/Screenshot-XL.png) |
| Single filter, no LFO, minimal UI | Single filter, single LFO, compact UI | Dual filter, dual LFO, morph, expanded UI |
| **[Download VST3](https://github.com/sleepingsw0rd/Z-Filter-Mac/raw/mini/Releases/Z-FilterMini-Mac-VST3.zip)** | **[Download VST3](https://github.com/sleepingsw0rd/Z-Filter-Mac/raw/main/Releases/Z-Filter-Mac-VST3.zip)** | **[Download VST3](https://github.com/sleepingsw0rd/Z-Filter-Mac/raw/v2-xl/Releases/Z-FilterXL-Mac-VST3.zip)** |
| **[Download AU](https://github.com/sleepingsw0rd/Z-Filter-Mac/raw/mini/Releases/Z-FilterMini-Mac-AU.zip)** | **[Download AU](https://github.com/sleepingsw0rd/Z-Filter-Mac/raw/main/Releases/Z-Filter-Mac-AU.zip)** | **[Download AU](https://github.com/sleepingsw0rd/Z-Filter-Mac/raw/v2-xl/Releases/Z-FilterXL-Mac-AU.zip)** |
| *You are here* | [Documentation](https://github.com/sleepingsw0rd/Z-Filter-Mac/tree/main#readme) | [Documentation](https://github.com/sleepingsw0rd/Z-Filter-Mac/tree/v2-xl#readme) |

### Installation

- **VST3**: Unzip and copy `Z-FilterMini.vst3` to `~/Library/Audio/Plug-Ins/VST3/`
- **AU**: Unzip and copy `Z-FilterMini.component` to `~/Library/Audio/Plug-Ins/Components/`

Rescan in your DAW. All three versions can run side-by-side.

## Features

- **4 Filter Types**: Lowpass, Highpass, Bandpass, Notch
- **Cascaded Biquad Architecture**: Up to 4 cascaded filter stages (12-48 dB/octave) controlled by the Poles knob
- **Large Frequency Knob**: Prominent cutoff control front and center
- **Input & Output Level Controls**: Input gain drives the filter's saturation character; Output gain for final level adjustment
- **Opamp Modeling Stage**: Bandwidth-limited analog op-amp simulation with polynomial soft saturation
- **Dot-Matrix LCD Display**: Real-time parameter readout in an HD44780-style display
- **Dry/Wet Mix**: Blend filtered and dry signals
- **Per-Sample Coefficient Smoothing**: Click-free parameter automation
- **Tiny Footprint**: 723x342 pixel window — fits anywhere

## Controls

| Control | Function |
|---------|----------|
| **Frequency** (large knob) | Filter cutoff frequency |
| **LP / HP / BP / NT** | Filter type selector buttons with LED indicators |
| **Input** | Pre-filter input gain |
| **Poles** | Filter resonance / cascade depth (1-4 stages) |
| **Level** | Post-filter output gain |
| **Mix** | Dry/wet blend |

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

- **VST3**: `build/ZFilterMini_artefacts/Release/VST3/Z-FilterMini.vst3`
- **AU**: `build/ZFilterMini_artefacts/Release/AU/Z-FilterMini.component`
- **Standalone**: `build/ZFilterMini_artefacts/Release/Standalone/Z-FilterMini.app`

With `COPY_PLUGIN_AFTER_BUILD` enabled, plugins are automatically installed to `~/Library/Audio/Plug-Ins/`.

## DSP Signal Flow

1. Input gain with hard clip to [-1, 1]
2. Frequency-dependent trim scaling
3. Up to 4 cascaded biquad filter stages (Transposed Direct Form II) with inter-stage clipping
4. DC-blocking highpass (~5 Hz)
5. Opamp stage: 15.5 kHz lowpass → polynomial soft saturation → 15.5 kHz lowpass
6. Output gain
7. Wet/dry mix blend
8. TPDF dither

## Credits

- **Filter algorithms**: Based on [Airwindows](https://www.airwindows.com/) ZLowpass2, ZHighpass2, ZBandpass2, ZNotch2 by Chris Johnson (MIT License)
- **Framework**: [JUCE](https://juce.com/) 8.0.12

## License

Filter DSP algorithms are derived from Airwindows code, licensed under the MIT License. See `Source/AirwindowsReference/FilterReference.h` for details.
