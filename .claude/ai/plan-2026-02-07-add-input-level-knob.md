---
date: 2026-02-07T00:00:00Z
planner: Claude
git_commit: (no commits yet)
branch: master
repository: Z-Filter
plan_type: "implementation"
topic: "Add Input Level Knob"
tags: [implementation, planning, gui, dsp, new-feature]
status: complete
last_updated: 2026-02-07
last_updated_by: Claude
---
# Add Input Level Knob - Implementation Plan

**Date**: 2026-02-07
**Planner**: Claude
**Git Commit**: (no commits yet)
**Branch**: master
**Repository**: Z-Filter

## Overview

Add an Input Level knob to Z-Filter so the user has both input and output level controls. The existing "Level" knob controls output gain. The new Input knob will be placed as the leftmost small knob, giving the order: **Input, Res, Level, Mix**. The input gain range is unity-centered: 0.5 = unity (0dB), 0.0 = -20dB, 1.0 = +20dB.

## Current State Analysis

The plugin currently has 3 small knobs (Res, Level, Mix) positioned on the bottom-right panel area. The DSP already has `inTrim` smoothing infrastructure (`inTrimA`/`inTrimB` with per-sample interpolation at `PluginProcessor.cpp:235`), but `inTrim` is hardcoded to 1.0 with no parameter driving it. The `inTrim` application site at line 249-252 multiplies the input signal before the hard clip and filter stages.

### Key Discoveries:

- `inTrimA`/`inTrimB` smoothing state already exists in `PluginProcessor.h:89`
- Per-sample interpolation already computes `inTrim` at `PluginProcessor.cpp:235`
- Application site at `PluginProcessor.cpp:249-252` already applies `inTrim` with a != 1.0 guard
- `prepareToPlay` initializes `inTrimA = inTrimB = 1.0` at `PluginProcessor.cpp:74`
- Current small knob positions: Res(699), Level(789), Mix(879) with 90px spacing at `PluginEditor.cpp:441-443`
- Current knob labels at `PluginEditor.cpp:411-413`: "Res", "Level", "Mix"
- LCD row2 shows: `"RES:xx%  LVL:xx%"` at `PluginEditor.cpp:373`
- Plugin width is 954px, set at `PluginEditor.cpp:289`

## Desired End State

- A new `"input"` parameter (0.0-1.0, default 0.5 = unity) in the APVTS
- The `inTrim` smoothing variables are driven by this parameter each block, converting to dB gain: `pow(10.0, (inputParam - 0.5) * 2.0)` which maps 0.0 → 0.01x (-40dB), 0.5 → 1.0x (0dB), 1.0 → 100x (+40dB). **Correction**: for -20dB to +20dB range, use `pow(10.0, (inputParam - 0.5) * 0.4 * 2.0)` → actually simplest: `pow(10.0, (inputParam * 2.0 - 1.0) * (20.0 / 20.0))` = `pow(10.0, inputParam * 2.0 - 1.0)`. At 0.0: 10^-1 = 0.1 (-20dB). At 0.5: 10^0 = 1.0 (0dB). At 1.0: 10^1 = 10.0 (+20dB). This is the correct formula.
- Four small knobs in order: Input, Res, Level, Mix
- LCD display updated to show input level
- Plugin builds and runs cleanly

### Verification:

- Build succeeds without errors
- Plugin loads with Input knob at center position (unity gain)
- Turning Input knob left reduces signal going into the filter
- Turning Input knob right boosts signal into the filter (engaging the hard clip/saturation)
- Input knob smoothly transitions without clicks/pops
- All existing parameters and behavior unchanged

## What We're NOT Doing

- **NOT changing the output "Level" knob** — it stays as-is with its existing range and behavior
- **NOT changing the plugin window size** — we'll tighten the small knob spacing to fit 4 knobs in the existing space
- **NOT adding metering** — just the gain control
- **NOT changing the background image** — knobs are rendered programmatically on top of it

## Implementation Approach

Three files need changes: `PluginProcessor.cpp` (add parameter, wire into DSP), `PluginEditor.h` (add knob + attachment members), and `PluginEditor.cpp` (add knob to UI, reposition, update LCD). The existing `inTrim` smoothing infrastructure means the DSP change is minimal — we just need to update `inTrimA`/`inTrimB` each block from the new parameter.

## Phase 1: Add Input Parameter and Wire DSP

### Overview

Add the `"input"` parameter to the APVTS and connect it to the existing `inTrim` smoothing variables in `processBlock`.

### Changes Required:

#### 1. Add parameter definition

**File**: `Source/PluginProcessor.cpp`
**Location**: In `createParameterLayout()`, after the `"output"` parameter (line 41), add the `"input"` parameter before `"resonance"`.

```cpp
// Add after the "output" parameter block (after line 41):
params.push_back(std::make_unique<juce::AudioParameterFloat>(
    juce::ParameterID("input", 1), "Input",
    juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f), 0.5f));
```

#### 2. Read parameter and compute inTrim in processBlock

**File**: `Source/PluginProcessor.cpp`
**Location**: In `processBlock`, after reading existing parameters (line 110), read the input parameter.

```cpp
// Add after line 110 (const float D = ...):
const float inputGain = *apvts.getRawParameterValue("input");
```

**Location**: In the smoothing section (around line 188-194), add inTrim update.

Replace lines 188-194:
```cpp
// Smoothing
mixA = mixB;
mixB = (double)mixParam;
inTrimA = inTrimB;
inTrimB = pow(10.0, (double)inputGain * 2.0 - 1.0);
outTrimA = outTrimB;
outTrimB = C * 10.0;
wetA = wetB;
wetB = pow(D, 2);
```

This converts the 0.0-1.0 parameter to a gain multiplier:
- inputGain=0.0 → `pow(10, -1)` = 0.1 (-20dB)
- inputGain=0.5 → `pow(10, 0)` = 1.0 (0dB, unity)
- inputGain=1.0 → `pow(10, 1)` = 10.0 (+20dB)

The existing per-sample interpolation at line 235 (`double inTrim = (inTrimA * interp) + (inTrimB * (1.0 - interp))`) and application at lines 249-252 will handle the rest automatically.

### Success Criteria:

#### Automated Verification:

- [x] CMake configures successfully: `cmake -B build -S .`
- [x] Build succeeds without errors: `cmake --build build --config Release`
- [x] No new compiler warnings introduced

#### Manual Verification:

- [ ] Input parameter exists and defaults to 0.5 (verifiable in DAW parameter list)
- [ ] At default (0.5), audio passes through at unity — no change from current behavior
- [ ] Signal is smoothly attenuated/boosted when input parameter changes

---

## Phase 2: Add Input Knob to GUI

### Overview

Add the Input Level knob as the leftmost small knob, reposition the existing 3 small knobs to accommodate it, and update labels and LCD display.

### Changes Required:

#### 1. Add knob and attachment members

**File**: `Source/PluginEditor.h`

Add after `mixKnob` declaration (line 241):
```cpp
KnobComponent inputKnob    { KnobComponent::Small };
```

Add after `mixAttachment` declaration (line 261):
```cpp
std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inputAttachment;
```

#### 2. Add knob to editor constructor

**File**: `Source/PluginEditor.cpp`

In the constructor, after `addAndMakeVisible(mixKnob);` (line 301):
```cpp
addAndMakeVisible(inputKnob);
```

After the mixAttachment creation (line 340), add:
```cpp
inputAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
    processorRef.apvts, "input", inputKnob);
```

#### 3. Update knob positions in resized()

**File**: `Source/PluginEditor.cpp`

Replace the current small knob positioning (lines 441-443) with 4 knobs.

The current 3 knobs span x=699 to x=917 (879+38). For 4 knobs at 38px wide with even spacing, use ~65px center-to-center:

```cpp
// Small knobs - 4 knobs: Input, Res, Level, Mix
inputKnob.setBounds(655, 263, 38, 38);
resonanceKnob.setBounds(728, 263, 38, 38);
outputKnob.setBounds(801, 263, 38, 38);
mixKnob.setBounds(874, 263, 38, 38);
```

#### 4. Update knob labels in paint()

**File**: `Source/PluginEditor.cpp`

Replace the current label drawing (lines 411-413) with 4 labels:

```cpp
// Knob labels (centered above small knobs)
g.drawText("Input", 649, 246, 50, 14, juce::Justification::centred);
g.drawText("Res", 722, 246, 50, 14, juce::Justification::centred);
g.drawText("Level", 795, 246, 50, 14, juce::Justification::centred);
g.drawText("Mix", 868, 246, 50, 14, juce::Justification::centred);
```

#### 5. Update LCD display

**File**: `Source/PluginEditor.cpp`

In `updateDisplay()`, read the input parameter and show it on the LCD.

After reading `mix` (line 363), add:
```cpp
float input = *processorRef.apvts.getRawParameterValue("input");
```

Update row2 (line 373) to include input level info. Since the LCD has 24 columns, we need to be concise:
```cpp
juce::String row2 = "IN:" + juce::String(input * 100.0f, 0) + "%  RES:" + juce::String(reso * 100.0f, 0) + "%";
juce::String row3 = "LVL:" + juce::String(output * 100.0f, 0) + "%  MIX:" + juce::String(mix * 100.0f, 0) + "%";
```

And move the bypass indicator into row0 or append it:
```cpp
juce::String row0 = "Z-FILTER    " + typeName;
// row3 already has LVL and MIX, add bypass state
row3 = row3 + " " + (bypassed ? "[BYP]" : "");
```

Actually, to keep it simple and within 24 chars, let's rearrange:
```cpp
juce::String row1 = "FREQ:" + juce::String(freq * 100.0f, 1) + "%";
juce::String row2 = "IN:" + juce::String(input * 100.0f, 0) + "% RES:" + juce::String(reso * 100.0f, 0) + "%";
juce::String row3 = "LVL:" + juce::String(output * 100.0f, 0) + "% MIX:" + juce::String(mix * 100.0f, 0) + "%" + (bypassed ? " BYP" : "");
```

### Success Criteria:

#### Automated Verification:

- [x] CMake configures successfully: `cmake -B build -S .`
- [x] Build succeeds without errors: `cmake --build build --config Release`
- [x] No new compiler warnings introduced
- [x] Both VST3 and Standalone targets build

#### Manual Verification:

- [ ] Input knob appears as leftmost small knob
- [ ] All 4 small knobs are evenly spaced and visually balanced
- [ ] Input knob label reads "Input"
- [ ] LCD shows input level percentage
- [ ] Input knob defaults to center position (50% / unity)
- [ ] Turning the Input knob audibly affects the signal level going into the filter
- [ ] All other knobs still work correctly
- [ ] Parameter automation works for the new Input parameter in a DAW

---

## Testing Strategy

### Manual Testing Steps:

1. Load plugin standalone or in DAW
2. Verify 4 small knobs visible: Input, Res, Level, Mix
3. Send audio through — at default settings, should sound identical to before (Input at unity)
4. Turn Input knob fully left — signal should get quieter (-20dB)
5. Turn Input knob fully right — signal should get louder/saturated (+20dB, engaging hard clip at line 254)
6. Sweep Input knob — no clicks or pops (smoothing working)
7. Verify LCD shows all 4 parameter values
8. Test all 4 filter types with Input at various settings
9. Test parameter save/recall (preset system)

## Performance Considerations

Negligible. One additional `pow()` call per buffer (not per sample), and the per-sample `inTrim` interpolation already exists.

## References

- Existing output gain: `PluginProcessor.cpp:192` (`outTrimB = C * 10.0`)
- inTrim smoothing: `PluginProcessor.cpp:235` (per-sample interpolation)
- inTrim application: `PluginProcessor.cpp:249-252`
- Current knob layout: `PluginEditor.cpp:441-443`
- Original Airwindows input gain: `pow(A * 10.0, 4)` — we use a different, more intuitive range
