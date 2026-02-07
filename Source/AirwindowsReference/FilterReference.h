/*
================================================================================
AIRWINDOWS FILTER REFERENCE - MIT LICENSE
================================================================================

Copyright (c) 2016 airwindows

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

================================================================================
REFERENCE IMPLEMENTATION NOTES
================================================================================

This file documents the key DSP algorithms and biquad coefficient formulas
from the Airwindows filter suite, which forms the reference basis for the
Z-Filter plugin implementation.

The filters implement 2-pole biquad designs with different frequency response
characteristics and resonance behaviors. Each filter type has unique coefficient
calculation formulas and scaling parameters.

Common Variables:
  - B: normalized parameter (0.0 to 1.0)
  - sr: sample rate (typically 44100, 48000, etc.)
  - K: tan(PI * freq)
  - norm: 1.0 / (1.0 + K/reso + K*K)
  - freq: normalized frequency (0.0 to 0.5)

All filters use the standard biquad difference equation:
  y[n] = a0*x[n] + a1*x[n-1] + a2*x[n-2] - b1*y[n-1] - b2*y[n-2]

where b1 = 2*K*norm and b2 = (K*K - 1/reso - K*K)*norm

================================================================================
LOWPASS FILTER (ZLowpass2)
================================================================================

Description:
  Butterworth-style lowpass filter with soft clipping for high resonance.
  Provides smooth frequency attenuation above the cutoff.

Frequency Calculation:
  freq = (pow(B, 3) * 18930.0 / sr) + 0.00162

  Cubic power scaling provides responsive control across the audio range.
  The B^3 characteristic emphasizes lower frequencies for finer control.

Clipping Factor:
  clipFactor = 1.212 - ((1.0 - B) * 0.496)

  Prevents output clipping at high resonance by scaling output gain.
  At B=0: clipFactor = 0.716
  At B=1: clipFactor = 1.212

Resonance (Q):
  reso = 0.7071 (sqrt(2) / 2, Butterworth)

Biquad Coefficients:
  a0 = K * K * norm
  a1 = 2 * a0
  a2 = a0

  The doubled a1 coefficient provides the peak gain characteristic of lowpass.

================================================================================
HIGHPASS FILTER (ZHighpass2)
================================================================================

Description:
  Complementary highpass filter with 90-degree phase shift.
  Removes low-frequency content while preserving high-frequency details.

Frequency Calculation:
  freq = (pow(B, 4) * 9500.0 / sr) + 0.00076

  Quartic (4th power) scaling provides tighter control in the lower range,
  compensating for human hearing's logarithmic frequency perception.

Clipping Factor:
  None applied (highpass output naturally constrained)

Resonance (Q):
  reso = 1.0 (no resonance peak)

Biquad Coefficients:
  a0 = norm
  a1 = -2 * a0
  a2 = a0

  The negative a1 creates the characteristic highpass response by inverting
  the middle frequency content relative to low and high frequencies.

================================================================================
BANDPASS FILTER (ZBandpass2)
================================================================================

Description:
  Narrow bandpass filter with sharp frequency selectivity.
  Isolates a specific frequency band with steep rolloff on both sides.

Frequency Calculation:
  freq = (pow(B, 4) * 14300.0 / sr) + 0.00079

  Quartic scaling provides precise center frequency tuning.
  Higher frequency constant than highpass for tighter response.

Clipping Factor:
  clipFactor = 1.0 - ((1.0 - B) * 0.304)

  Moderate clipping prevention suitable for bandpass gain peaks.
  At B=0: clipFactor = 0.696
  At B=1: clipFactor = 1.0

Resonance (Q):
  reso = 0.314 (fixed, provides characteristic bandpass sharpness)

Biquad Coefficients:
  a0 = K / Q * norm
  a1 = 0
  a2 = -a0

  The zero a1 coefficient and opposite a0/a2 creates the bandpass
  peak response. Q-dependent gain prevents excessive output swing.

================================================================================
NOTCH FILTER (ZNotch2)
================================================================================

Description:
  Narrow notch (band-reject) filter complementary to bandpass.
  Attenuates specific frequencies while preserving surrounding spectrum.

Frequency Calculation:
  freq = (pow(B, 3) * 4700.0 / sr) + 0.0009963

  Cubic scaling provides smooth frequency control.
  Lowest frequency constant optimizes notch depth positioning.

Clipping Factor:
  clipFactor = 0.91 - ((1.0 - B) * 0.15)

  Minimal clipping factor maintains notch characteristics.
  At B=0: clipFactor = 0.76
  At B=1: clipFactor = 0.91

Resonance (Q):
  reso = 0.618 (golden ratio, optimizes notch sharpness)

Biquad Coefficients:
  a0 = (1 + K * K) * norm
  a1 = 2 * (K * K - 1) * norm
  a2 = a0

  The high a0 value and frequency-dependent a1 create the characteristic
  notch null at the center frequency with smooth adjacent response.

================================================================================
IMPLEMENTATION NOTES
================================================================================

Frequency Normalization:
  All frequency parameters are normalized to sample rate using different
  power functions (cubic, quartic) and additive constants. This provides
  the characteristic Airwindows response curve where lower B values (0-0.3)
  provide fine control for subtle filtering, while higher B values (0.7-1.0)
  allow rapid frequency sweeping.

Resonance/Q Values:
  - Lowpass:  0.7071 (Butterworth, maximally flat passband)
  - Highpass: 1.0 (critical damping, no resonance)
  - Bandpass: 0.314 (narrow, sharp response)
  - Notch:    0.618 (golden ratio, optimal selectivity)

Clipping Factors:
  Applied as output multiplier to prevent distortion at high resonance.
  Bandpass requires the most clipping (0.696-1.0) due to potential gain peaks.
  Notch requires minimal clipping (0.76-0.91) to preserve null depth.

Sample Rate Compensation:
  All frequency calculations include sr (sample rate) in the denominator,
  making formulas frequency-normalized across different sample rates.
  Additional constants (18930.0, 9500.0, etc.) are empirically optimized
  for musically useful frequency ranges at standard sample rates.

================================================================================
END OF REFERENCE
================================================================================
*/
