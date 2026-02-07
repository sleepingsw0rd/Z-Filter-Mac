---
date: 2026-02-07T00:00:00Z
planner: Claude
git_commit: (no commits yet)
branch: master
repository: Z-Filter
plan_type: "implementation"
topic: "Fix Resonance Distortion and Hiss"
tags: [implementation, planning, dsp, bugfix, gain-staging]
status: complete
last_updated: 2026-02-07
last_updated_by: Claude
---
# Fix Resonance Distortion and Hiss - Implementation Plan

**Date**: 2026-02-07
**Planner**: Claude
**Git Commit**: (no commits yet)
**Branch**: master
**Repository**: Z-Filter

## Overview

The Z-Filter plugin suffers from severe distortion and hiss caused by a porting bug from the original Airwindows source. The `inTrim` variable is hardcoded to 625.0 and never updated, causing every input sample to be multiplied by 625x and then hard-clipped to a square wave before reaching the filter stages. A secondary issue is that the resonance (poles) parameter defaults to maximum, engaging all four cascaded biquad stages with their inter-stage hard clipping, compounding the distortion.

## Current State Analysis

The Z-Filter is a JUCE port of four Airwindows filters (ZLowpass2, ZHighpass2, ZBandpass2, ZNotch2). The DSP signal flow was ported correctly in terms of structure, but the `inTrim` gain parameter was mishandled during the port.

### Key Discoveries:

- **Critical Bug**: `inTrimA = inTrimB = 625.0` at `PluginProcessor.cpp:74`, never recalculated in `processBlock`. In the original Airwindows code, `inTrim` is computed as `pow(A * 10.0, 4)` where A is an "Input" parameter defaulting to 0.1, yielding `inTrim = 1.0` at default.
- **Effect**: At line 250, every sample is multiplied by 625, then hard-clipped to [-1, 1] at line 254. Any signal above -56 dBFS becomes a clipped square wave.
- **Secondary Bug**: Resonance parameter defaults to 1.0 (`PluginProcessor.cpp:45`) instead of the original's 0.5, activating all 4 cascaded filter stages at default, amplifying the distortion through repeated inter-stage hard clipping.
- **Missing Parameter**: The original's "Input" (A) parameter was removed during porting but its gain variable (`inTrim`) was left in the code with a nonsensical initialization value.
- **Everything else is correct**: Biquad coefficients, frequency mapping, clipFactor, trim, outTrim, wet/dry staging, coefficient interpolation, opamp stage, fixed filters, soft saturation, and dither all match the original implementation.

## Desired End State

After this fix:
1. The `inTrim` gain stage no longer destroys the signal - input audio passes through at unity gain (matching original default behavior)
2. The resonance parameter defaults to 0.5 (matching original "Poles" parameter default), giving a single-stage filter as the starting point
3. The plugin produces clean, musical filtering across all four filter types with no unwanted distortion or hiss
4. All existing parameter ranges and DSP architecture remain unchanged

### Verification:
- Build succeeds without errors or warnings
- Plugin loads in a DAW and processes audio cleanly
- Lowpass filter at default settings produces a gentle roll-off, not a distorted square wave
- Sweeping the frequency knob produces smooth, musical filtering
- Resonance (poles) control smoothly adds filter stages from 1 to 4
- Output level is reasonable at default settings (not clipping, not silent)

## What We're NOT Doing

- **NOT adding an Input gain parameter** - The original had it, but our UI was designed without it. We'll simply remove the broken gain stage entirely (equivalent to the original at A=0.1 default).
- **NOT renaming "Resonance" to "Poles"** - The UI already labels it "Resonance" and users understand this; a cosmetic rename is out of scope for this bugfix.
- **NOT changing the DSP architecture** - The cascaded biquad design, opamp stage, and all other processing are correct and should not be modified.
- **NOT changing the output gain default** - `outTrim = C * 10.0` with C defaulting to 1.0 gives +20dB max output, which matches the original and is intentional for gain staging flexibility.

## Implementation Approach

This is a minimal, surgical fix targeting exactly the two bugs causing the problem. Both fixes are single-line changes in `PluginProcessor.cpp`.

## Phase 1: Fix the inTrim Gain Bug

### Overview

Remove the 625x input gain multiplication that's destroying the signal by setting `inTrim` to 1.0 (unity gain), matching the original Airwindows default behavior.

### Changes Required:

#### 1. Fix inTrim initialization

**File**: `Source/PluginProcessor.cpp`
**Line 74**
**Change**: Set `inTrimA` and `inTrimB` to `1.0` instead of `625.0`

```cpp
// BEFORE (line 74):
inTrimA = inTrimB = 625.0;

// AFTER:
inTrimA = inTrimB = 1.0;
```

**Why this works**: With `inTrim = 1.0`, the conditional at line 249 (`if (inTrim != 1.0)`) will evaluate to `false`, skipping the multiplication entirely. The signal passes through at unity gain to the `trim` and `clipFactor` stages, which is exactly what happens in the original Airwindows code at its default Input setting (A=0.1).

**Why not remove the inTrim code entirely**: The interpolation code at line 235 and the conditional multiplication at lines 249-252 are harmless when inTrim=1.0 (the branch is never taken). Leaving them in preserves the code structure's fidelity to the original Airwindows source, making future reference easier. The compiler will likely optimize away the dead branch.

### Success Criteria:

#### Automated Verification:

- [x] CMake configures successfully: `cmake -B build -S .`
- [x] Build succeeds without errors: `cmake --build build --config Release`
- [x] No new compiler warnings introduced

#### Manual Verification:

- [ ] Input signal is no longer hard-clipped to a square wave
- [ ] Audio passes through the filter cleanly at typical input levels (-18 to 0 dBFS)
- [ ] No audible distortion or hiss at default settings
- [ ] Frequency sweep sounds smooth and musical

---

## Phase 2: Fix Resonance Default Value

### Overview

Change the resonance parameter default from 1.0 to 0.5, matching the original Airwindows "Poles" parameter default. This activates only the first biquad stage at default, giving a gentler filter response.

### Changes Required:

#### 1. Change resonance parameter default

**File**: `Source/PluginProcessor.cpp`
**Lines 43-45**
**Change**: Set default from `1.0f` to `0.5f`

```cpp
// BEFORE (lines 43-45):
params.push_back(std::make_unique<juce::AudioParameterFloat>(
    juce::ParameterID("resonance", 1), "Resonance",
    juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f), 1.0f));

// AFTER:
params.push_back(std::make_unique<juce::AudioParameterFloat>(
    juce::ParameterID("resonance", 1), "Resonance",
    juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f), 0.5f));
```

**Why 0.5**: At D=0.5, `wet = pow(0.5, 2) = 0.25`, `dWet = 0.25 * 4.0 = 1.0`. This means `aWet = 1.0` and `bWet = cWet = dWet = 0.0` — only Stage A is active, giving a standard 12dB/octave (2-pole) filter response. This is the original default and a sensible starting point. Users can increase resonance to engage more stages (up to 48dB/octave at maximum).

### Success Criteria:

#### Automated Verification:

- [x] CMake configures successfully: `cmake -B build -S .`
- [x] Build succeeds without errors: `cmake --build build --config Release`
- [x] No new compiler warnings introduced

#### Manual Verification:

- [ ] At default resonance (0.5), only one filter stage is audibly active (gentle slope)
- [ ] Increasing resonance progressively steepens the filter slope
- [ ] At resonance = 1.0, all four stages are active (steep slope)
- [ ] At resonance = 0.0, filter output approaches dry signal (near-bypass)
- [ ] No existing presets or saved states are broken (parameter range is unchanged, only the default changed)

---

## Testing Strategy

### Manual Testing Steps:

1. Load plugin in a DAW (or use standalone build)
2. Send pink noise or music through the plugin at -18 dBFS
3. At default settings: verify clean lowpass filtering with no distortion
4. Sweep frequency knob fully: verify smooth, musical response
5. Set resonance to 0.0: verify near-bypass behavior
6. Set resonance to 1.0: verify steep filtering (no distortion, just steeper slope)
7. Switch through all four filter types (LP/HP/BP/NT) and verify clean operation
8. Test at both 44.1kHz and 48kHz sample rates
9. Test with hot input signal (0 dBFS) to verify no unexpected clipping

## Performance Considerations

No performance impact. Phase 1 actually improves performance marginally by eliminating the 625x multiply and subsequent hard-clip operations (the `if (inTrim != 1.0)` branch will never be taken). Phase 2 reduces default CPU usage by activating fewer filter stages.

## References

- Original Airwindows ZLowpass2: `https://github.com/airwindows/airwindows/tree/master/plugins/LinuxVST/src/ZLowpass2`
- Local reference: `Source/AirwindowsReference/FilterReference.h`
- Signal flow: `Source/PluginProcessor.cpp:94-386` (processBlock)
- Parameter definitions: `Source/PluginProcessor.cpp:20-48`
- State initialization: `Source/PluginProcessor.cpp:63-81` (prepareToPlay)
