#include "PluginEditor.h"

//==============================================================================
// 5x7 dot-matrix font data (ASCII 32-127)
// Each character is 5 bytes, one per column, LSB = top row
//==============================================================================
const uint8_t DotMatrixLCD::font5x7[] = {
    // 32 SPACE
    0x00, 0x00, 0x00, 0x00, 0x00,
    // 33 !
    0x00, 0x00, 0x5F, 0x00, 0x00,
    // 34 "
    0x00, 0x07, 0x00, 0x07, 0x00,
    // 35 #
    0x14, 0x7F, 0x14, 0x7F, 0x14,
    // 36 $
    0x24, 0x2A, 0x7F, 0x2A, 0x12,
    // 37 %
    0x23, 0x13, 0x08, 0x64, 0x62,
    // 38 &
    0x36, 0x49, 0x55, 0x22, 0x50,
    // 39 '
    0x00, 0x05, 0x03, 0x00, 0x00,
    // 40 (
    0x00, 0x1C, 0x22, 0x41, 0x00,
    // 41 )
    0x00, 0x41, 0x22, 0x1C, 0x00,
    // 42 *
    0x14, 0x08, 0x3E, 0x08, 0x14,
    // 43 +
    0x08, 0x08, 0x3E, 0x08, 0x08,
    // 44 ,
    0x00, 0x50, 0x30, 0x00, 0x00,
    // 45 -
    0x08, 0x08, 0x08, 0x08, 0x08,
    // 46 .
    0x00, 0x60, 0x60, 0x00, 0x00,
    // 47 /
    0x20, 0x10, 0x08, 0x04, 0x02,
    // 48 0
    0x3E, 0x51, 0x49, 0x45, 0x3E,
    // 49 1
    0x00, 0x42, 0x7F, 0x40, 0x00,
    // 50 2
    0x42, 0x61, 0x51, 0x49, 0x46,
    // 51 3
    0x21, 0x41, 0x45, 0x4B, 0x31,
    // 52 4
    0x18, 0x14, 0x12, 0x7F, 0x10,
    // 53 5
    0x27, 0x45, 0x45, 0x45, 0x39,
    // 54 6
    0x3C, 0x4A, 0x49, 0x49, 0x30,
    // 55 7
    0x01, 0x71, 0x09, 0x05, 0x03,
    // 56 8
    0x36, 0x49, 0x49, 0x49, 0x36,
    // 57 9
    0x06, 0x49, 0x49, 0x29, 0x1E,
    // 58 :
    0x00, 0x36, 0x36, 0x00, 0x00,
    // 59 ;
    0x00, 0x56, 0x36, 0x00, 0x00,
    // 60 <
    0x08, 0x14, 0x22, 0x41, 0x00,
    // 61 =
    0x14, 0x14, 0x14, 0x14, 0x14,
    // 62 >
    0x00, 0x41, 0x22, 0x14, 0x08,
    // 63 ?
    0x02, 0x01, 0x51, 0x09, 0x06,
    // 64 @
    0x32, 0x49, 0x79, 0x41, 0x3E,
    // 65 A
    0x7E, 0x11, 0x11, 0x11, 0x7E,
    // 66 B
    0x7F, 0x49, 0x49, 0x49, 0x36,
    // 67 C
    0x3E, 0x41, 0x41, 0x41, 0x22,
    // 68 D
    0x7F, 0x41, 0x41, 0x22, 0x1C,
    // 69 E
    0x7F, 0x49, 0x49, 0x49, 0x41,
    // 70 F
    0x7F, 0x09, 0x09, 0x09, 0x01,
    // 71 G
    0x3E, 0x41, 0x49, 0x49, 0x7A,
    // 72 H
    0x7F, 0x08, 0x08, 0x08, 0x7F,
    // 73 I
    0x00, 0x41, 0x7F, 0x41, 0x00,
    // 74 J
    0x20, 0x40, 0x41, 0x3F, 0x01,
    // 75 K
    0x7F, 0x08, 0x14, 0x22, 0x41,
    // 76 L
    0x7F, 0x40, 0x40, 0x40, 0x40,
    // 77 M
    0x7F, 0x02, 0x0C, 0x02, 0x7F,
    // 78 N
    0x7F, 0x04, 0x08, 0x10, 0x7F,
    // 79 O
    0x3E, 0x41, 0x41, 0x41, 0x3E,
    // 80 P
    0x7F, 0x09, 0x09, 0x09, 0x06,
    // 81 Q
    0x3E, 0x41, 0x51, 0x21, 0x5E,
    // 82 R
    0x7F, 0x09, 0x19, 0x29, 0x46,
    // 83 S
    0x46, 0x49, 0x49, 0x49, 0x31,
    // 84 T
    0x01, 0x01, 0x7F, 0x01, 0x01,
    // 85 U
    0x3F, 0x40, 0x40, 0x40, 0x3F,
    // 86 V
    0x1F, 0x20, 0x40, 0x20, 0x1F,
    // 87 W
    0x3F, 0x40, 0x38, 0x40, 0x3F,
    // 88 X
    0x63, 0x14, 0x08, 0x14, 0x63,
    // 89 Y
    0x07, 0x08, 0x70, 0x08, 0x07,
    // 90 Z
    0x61, 0x51, 0x49, 0x45, 0x43,
    // 91 [
    0x00, 0x7F, 0x41, 0x41, 0x00,
    // 92 backslash
    0x02, 0x04, 0x08, 0x10, 0x20,
    // 93 ]
    0x00, 0x41, 0x41, 0x7F, 0x00,
    // 94 ^
    0x04, 0x02, 0x01, 0x02, 0x04,
    // 95 _
    0x40, 0x40, 0x40, 0x40, 0x40,
    // 96 `
    0x00, 0x01, 0x02, 0x04, 0x00,
    // 97 a
    0x20, 0x54, 0x54, 0x54, 0x78,
    // 98 b
    0x7F, 0x48, 0x44, 0x44, 0x38,
    // 99 c
    0x38, 0x44, 0x44, 0x44, 0x20,
    // 100 d
    0x38, 0x44, 0x44, 0x48, 0x7F,
    // 101 e
    0x38, 0x54, 0x54, 0x54, 0x18,
    // 102 f
    0x08, 0x7E, 0x09, 0x01, 0x02,
    // 103 g
    0x0C, 0x52, 0x52, 0x52, 0x3E,
    // 104 h
    0x7F, 0x08, 0x04, 0x04, 0x78,
    // 105 i
    0x00, 0x44, 0x7D, 0x40, 0x00,
    // 106 j
    0x20, 0x40, 0x44, 0x3D, 0x00,
    // 107 k
    0x7F, 0x10, 0x28, 0x44, 0x00,
    // 108 l
    0x00, 0x41, 0x7F, 0x40, 0x00,
    // 109 m
    0x7C, 0x04, 0x18, 0x04, 0x78,
    // 110 n
    0x7C, 0x08, 0x04, 0x04, 0x78,
    // 111 o
    0x38, 0x44, 0x44, 0x44, 0x38,
    // 112 p
    0x7C, 0x14, 0x14, 0x14, 0x08,
    // 113 q
    0x08, 0x14, 0x14, 0x18, 0x7C,
    // 114 r
    0x7C, 0x08, 0x04, 0x04, 0x08,
    // 115 s
    0x48, 0x54, 0x54, 0x54, 0x20,
    // 116 t
    0x04, 0x3F, 0x44, 0x40, 0x20,
    // 117 u
    0x3C, 0x40, 0x40, 0x20, 0x7C,
    // 118 v
    0x1C, 0x20, 0x40, 0x20, 0x1C,
    // 119 w
    0x3C, 0x40, 0x30, 0x40, 0x3C,
    // 120 x
    0x44, 0x28, 0x10, 0x28, 0x44,
    // 121 y
    0x0C, 0x50, 0x50, 0x50, 0x3C,
    // 122 z
    0x44, 0x64, 0x54, 0x4C, 0x44,
    // 123 {
    0x00, 0x08, 0x36, 0x41, 0x00,
    // 124 |
    0x00, 0x00, 0x7F, 0x00, 0x00,
    // 125 }
    0x00, 0x41, 0x36, 0x08, 0x00,
    // 126 ~
    0x10, 0x08, 0x08, 0x10, 0x08,
    // 127 DEL
    0x00, 0x00, 0x00, 0x00, 0x00,
};

DotMatrixLCD::DotMatrixLCD()
{
    setOpaque(false);
}

void DotMatrixLCD::setText(int row, const juce::String& text)
{
    if (row >= 0 && row < kRows && rows[row] != text)
    {
        rows[row] = text;
        dirty = true;
        repaint();
    }
}

void DotMatrixLCD::drawChar(juce::Graphics& g, char ch, float x, float y, float dotSize, float dotPitch) const
{
    int idx = (int)ch - 32;
    if (idx < 0 || idx > 95) idx = 0; // fallback to space

    const uint8_t* glyph = &font5x7[idx * 5];

    for (int col = 0; col < kCharW; col++)
    {
        uint8_t colData = glyph[col];
        for (int row = 0; row < kCharH; row++)
        {
            float dx = x + col * dotPitch;
            float dy = y + row * dotPitch;

            if (colData & (1 << row))
            {
                // Active dot - dark green/black
                g.setColour(juce::Colour(0xff0a2a0a));
                g.fillEllipse(dx, dy, dotSize, dotSize);
            }
            else
            {
                // Ghost dot - very faint
                g.setColour(juce::Colour(0x15000000));
                g.fillEllipse(dx, dy, dotSize, dotSize);
            }
        }
    }
}

void DotMatrixLCD::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();

    // Calculate dot sizing to fit within bounds
    float totalDotsW = (float)(kCols * (kCharW + 1) - 1);
    float totalDotsH = (float)(kRows * (kCharH + 2) - 2);
    float dotPitch = juce::jmin(bounds.getWidth() / (totalDotsW + 2.0f),
                                 bounds.getHeight() / (totalDotsH + 2.0f));
    float dotSize = dotPitch * 0.75f;

    float charPitchX = (kCharW + 1) * dotPitch;
    float charPitchY = (kCharH + 2) * dotPitch;

    // Center the display
    float totalW = kCols * charPitchX - dotPitch;
    float totalH = kRows * charPitchY - 2 * dotPitch;
    float startX = bounds.getX() + (bounds.getWidth() - totalW) / 2.0f;
    float startY = bounds.getY() + (bounds.getHeight() - totalH) / 2.0f;

    for (int row = 0; row < kRows; row++)
    {
        auto text = rows[row];
        for (int col = 0; col < kCols; col++)
        {
            char ch = (col < text.length()) ? (char)text[col] : ' ';
            float cx = startX + col * charPitchX;
            float cy = startY + row * charPitchY;
            drawChar(g, ch, cx, cy, dotSize, dotPitch);
        }
    }

    dirty = false;
}

//==============================================================================
// Editor
//==============================================================================
ZFilterEditor::ZFilterEditor(ZFilterProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    setSize(954, 344);

    // Load background image
    backgroundImage = juce::ImageCache::getFromMemory(BinaryData::GUINoControlsNoBigKnob_png, BinaryData::GUINoControlsNoBigKnob_pngSize);

    // LCD
    addAndMakeVisible(lcd);

    // Knobs
    addAndMakeVisible(frequencyKnob);
    addAndMakeVisible(resonanceKnob);
    addAndMakeVisible(outputKnob);
    addAndMakeVisible(mixKnob);
    addAndMakeVisible(inputKnob);

    // Buttons
    auto addBtn = [this](RoundButtonComponent& btn) { addAndMakeVisible(btn); };
    addBtn(bypassBtn);
    addBtn(lpBtn);
    addBtn(hpBtn);
    addBtn(bpBtn);
    addBtn(ntBtn);

    // LEDs
    addAndMakeVisible(bypassLED);
    addAndMakeVisible(lpLED);
    addAndMakeVisible(hpLED);
    addAndMakeVisible(bpLED);
    addAndMakeVisible(ntLED);

    // Button callbacks
    bypassBtn.onClick = [this]() {
        auto* param = processorRef.apvts.getParameter("bypass");
        param->setValueNotifyingHost(param->getValue() < 0.5f ? 1.0f : 0.0f);
    };

    auto setFilter = [this](int index) {
        processorRef.apvts.getParameter("filterType")->setValueNotifyingHost((float)index / 3.0f);
    };
    lpBtn.onClick = [setFilter]() { setFilter(0); };
    hpBtn.onClick = [setFilter]() { setFilter(1); };
    bpBtn.onClick = [setFilter]() { setFilter(2); };
    ntBtn.onClick = [setFilter]() { setFilter(3); };

    // Parameter attachments
    frequencyAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.apvts, "frequency", frequencyKnob);
    resonanceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.apvts, "resonance", resonanceKnob);
    outputAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.apvts, "output", outputKnob);
    mixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.apvts, "mix", mixKnob);
    inputAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.apvts, "input", inputKnob);

    updateDisplay();
    startTimerHz(15);
}

ZFilterEditor::~ZFilterEditor()
{
    stopTimer();
}

void ZFilterEditor::timerCallback()
{
    updateDisplay();
}

void ZFilterEditor::updateDisplay()
{
    // Read params
    int filterType = (int)*processorRef.apvts.getRawParameterValue("filterType");
    float freq = *processorRef.apvts.getRawParameterValue("frequency");
    float reso = *processorRef.apvts.getRawParameterValue("resonance");
    float output = *processorRef.apvts.getRawParameterValue("output");
    float mix = *processorRef.apvts.getRawParameterValue("mix");
    float input = *processorRef.apvts.getRawParameterValue("input");
    bool bypassed = *processorRef.apvts.getRawParameterValue("bypass") > 0.5f;

    // Filter type names
    const char* typeNames[] = { "LOWPASS", "HIGHPASS", "BANDPASS", "NOTCH" };
    juce::String typeName = (filterType >= 0 && filterType <= 3) ? typeNames[filterType] : "LOWPASS";

    // LCD content
    juce::String row0 = "Z-FILTER    " + typeName;
    juce::String row1 = "FREQ: " + juce::String(freq * 100.0f, 1) + "%";
    juce::String row2 = "IN:" + juce::String(input * 100.0f, 0) + "% RES:" + juce::String(reso * 100.0f, 0) + "%";
    juce::String row3 = "LVL:" + juce::String(output * 100.0f, 0) + "% MIX:" + juce::String(mix * 100.0f, 0) + "%" + (bypassed ? " BYP" : "");

    lcd.setText(0, row0);
    lcd.setText(1, row1);
    lcd.setText(2, row2);
    lcd.setText(3, row3);

    // LEDs
    bypassLED.setActive(bypassed);
    lpLED.setActive(filterType == 0);
    hpLED.setActive(filterType == 1);
    bpLED.setActive(filterType == 2);
    ntLED.setActive(filterType == 3);
}

//==============================================================================
void ZFilterEditor::paint(juce::Graphics& g)
{
    if (backgroundImage.isValid())
        g.drawImageAt(backgroundImage, 0, 0);
    else
        g.fillAll(juce::Colour(0xff1a1a1a));

    // Draw text labels on the panel
    g.setColour(juce::Colour(0xffb0b0b0));
    g.setFont(juce::Font(11.5f));

    // Bypass label (right of LED)
    g.drawText("Bypass", 62, 241, 55, 14, juce::Justification::centredLeft);

    // Filter type labels (centered above buttons)
    g.drawText("LP", 188, 250, 44, 14, juce::Justification::centred);
    g.drawText("HP", 284, 250, 44, 14, juce::Justification::centred);
    g.drawText("BP", 372, 250, 44, 14, juce::Justification::centred);
    g.drawText("NT", 464, 250, 44, 14, juce::Justification::centred);

    // Knob labels (centered above small knobs)
    g.drawText("Input", 649, 246, 50, 14, juce::Justification::centred);
    g.drawText("Res", 722, 246, 50, 14, juce::Justification::centred);
    g.drawText("Level", 795, 246, 50, 14, juce::Justification::centred);
    g.drawText("Mix", 868, 246, 50, 14, juce::Justification::centred);
}

void ZFilterEditor::resized()
{
    // LCD display - inset within the green area
    lcd.setBounds(115, 62, 468, 110);

    // Frequency knob (large) - centered in recessed area ~(800, 112)
    frequencyKnob.setBounds(706, 18, 188, 188);

    // Bypass button (pill-shaped) & LED
    bypassBtn.setBounds(39, 271, 48, 26);
    bypassLED.setBounds(48, 243, 10, 10);

    // Filter type buttons (pill-shaped) - shifted up 12px
    lpBtn.setBounds(188, 272, 44, 24);
    hpBtn.setBounds(284, 272, 44, 24);
    bpBtn.setBounds(372, 272, 44, 24);
    ntBtn.setBounds(464, 272, 44, 24);

    // Filter type LEDs - shifted up 12px
    lpLED.setBounds(205, 302, 10, 10);
    hpLED.setBounds(301, 302, 10, 10);
    bpLED.setBounds(389, 302, 10, 10);
    ntLED.setBounds(481, 302, 10, 10);

    // Small knobs - 4 knobs: Input, Res, Level, Mix
    inputKnob.setBounds(655, 263, 38, 38);
    resonanceKnob.setBounds(728, 263, 38, 38);
    outputKnob.setBounds(801, 263, 38, 38);
    mixKnob.setBounds(874, 263, 38, 38);
}
