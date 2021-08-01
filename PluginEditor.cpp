/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FlangerAudioProcessorEditor::FlangerAudioProcessorEditor(FlangerAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    startTimerHz(60);
    setSize (500, 300);
    setLookAndFeel(&mixFeel);
    
    initColors();
    for (int i = 0; i < 2; i++)
    {
        tapeReels.add(new TapeReel(p, i));
        addAndMakeVisible(tapeReels[i]);
    }
    tapeReels[0]->setBounds(100, 150, 125, 125);
    tapeReels[1]->setBounds(100, 150, 125, 125);

    buttonHigh1 = std::make_unique<juce::TextButton>("FM");
    buttonHigh1->setBounds(10, 37, 25, 12);
    buttonHigh1->setClickingTogglesState(true);
    buttonHigh1->addListener(this);
    buttonHigh1->setRadioGroupId(1);
    addAndMakeVisible(buttonHigh1.get());

    buttonHigh2 = std::make_unique<juce::TextButton>("FM");
    buttonHigh2->setBounds(465, 37, 25, 12);
    buttonHigh2->setClickingTogglesState(true);
    buttonHigh2->addListener(this);
    buttonHigh2->setRadioGroupId(2);
    addAndMakeVisible(buttonHigh2.get());

    buttonMid1 = std::make_unique<juce::TextButton>("LFO");
    buttonMid1->setBounds(10, 52, 30, 12);
    buttonMid1->setClickingTogglesState(true);
    buttonMid1->addListener(this);
    buttonMid1->setRadioGroupId(1);
    addAndMakeVisible(buttonMid1.get());

    buttonMid2 = std::make_unique<juce::TextButton>("LFO");
    buttonMid2->setBounds(460, 52, 30, 12);
    buttonMid2->setClickingTogglesState(true);
    buttonMid2->addListener(this);
    buttonMid2->setRadioGroupId(2);
    addAndMakeVisible(buttonMid2.get());

    buttonLow1 = std::make_unique<juce::TextButton>("Ultra-Low");
    buttonLow1->setBounds(10, 67, 35, 12);
    buttonLow1->setClickingTogglesState(true);
    buttonLow1->addListener(this);
    buttonLow1->setRadioGroupId(1);
    addAndMakeVisible(buttonLow1.get());

    buttonLow2 = std::make_unique<juce::TextButton>("Ultra-Low");
    buttonLow2->setBounds(455, 67, 35, 12);
    buttonLow2->setClickingTogglesState(true);
    buttonLow2->addListener(this);
    buttonLow2->setRadioGroupId(2);
    addAndMakeVisible(buttonLow2.get());

   // tapeReels[0]->setBoundsRelative(0.2, 0.25, 0.25, 0.25);
    
    amp1 = std::make_unique<juce::Slider>(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::NoTextBox);
    amp1->addListener(this);
    addAndMakeVisible(amp1.get());
    amp1->setBoundsRelative(0.03, 0.3, 0.1, 0.1);
    amp1->setColour(juce::Slider::rotarySliderFillColourId, *babyBlue);

    label1 = std::make_unique<juce::Label>("Depth");
    label1->setJustificationType(juce::Justification::centred);
    addAndMakeVisible(label1.get());
    label1->setBoundsRelative(0.3, 0.03, 0.05, 0.05);

    attach1 = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "LFO DEPTH 1", *amp1);

    rate1 = std::make_unique<juce::Slider>(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::NoTextBox);
    rate1->addListener(this);
    addAndMakeVisible(rate1.get());
    rate1->setBoundsRelative(0.03, 0.45, 0.1, 0.1);
    rate1->setColour(juce::Slider::rotarySliderFillColourId, *yellow);
    rlabel1 = std::make_unique<juce::Label>("Rate");
    addAndMakeVisible(rlabel1.get());
    rlabel1->attachToComponent(rate1.get(), false);
    rlabel1->setJustificationType(juce::Justification::centred);
    rattach1 = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "LFO RATE 1", *rate1);


    amp2 = std::make_unique<juce::Slider>(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::NoTextBox);
    amp2->addListener(this);
    addAndMakeVisible(amp2.get());
    amp2->setBoundsRelative(0.87, 0.3, 0.1, 0.1);
    amp2->setColour(juce::Slider::rotarySliderFillColourId, *babyBlue);
    label2 = std::make_unique<juce::Label>("Depth");
    addAndMakeVisible(label2.get());
    label2->attachToComponent(amp2.get(), false);
    label2->setJustificationType(juce::Justification::centred);
    attach2 = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "LFO DEPTH 2", *amp2);

    rate2 = std::make_unique<juce::Slider>(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::NoTextBox);
    rate2->addListener(this);
    addAndMakeVisible(rate2.get());
    rate2->setBoundsRelative(0.87, 0.45, 0.1, 0.1);
    rate2->setColour(juce::Slider::rotarySliderFillColourId, *yellow);
    rlabel2 = std::make_unique<juce::Label>("Rate");
    addAndMakeVisible(rlabel2.get());
    rlabel2->attachToComponent(rate2.get(), false);
    rlabel2->setJustificationType(juce::Justification::centred);
    rattach2 = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "LFO RATE 2", *rate2);

    fb1 = std::make_unique<juce::Slider>(juce::Slider::SliderStyle:: RotaryHorizontalVerticalDrag, juce::Slider::NoTextBox);
    fb1->addListener(this);
    addAndMakeVisible(fb1.get());
    fb1->setBoundsRelative(0.03, 0.6, 0.1, 0.1);
    fb1->setColour(juce::Slider::rotarySliderFillColourId, juce::Colour (254, 0, 254));
    fbLabel1 = std::make_unique<juce::Label>("Rate");
    addAndMakeVisible(fbLabel1.get());
    fbLabel1->attachToComponent(fb1.get(), false);
    fbLabel1->setJustificationType(juce::Justification::centred);
    fbAttach1 = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "FEEDBACK 1", *fb1);

    fb2 = std::make_unique<juce::Slider>(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::NoTextBox);
    fb2->addListener(this);
    addAndMakeVisible(fb2.get());
    fb2->setBoundsRelative(0.87, 0.6, 0.1, 0.1);
    fb2->setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(254, 0, 254));
    fbLabel2 = std::make_unique<juce::Label>("Rate");
    addAndMakeVisible(fbLabel2.get());
    fbLabel2->attachToComponent(fb2.get(), false);
    fbLabel2->setJustificationType(juce::Justification::centred);
    fbAttach2 = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "FEEDBACK 2", *fb2);
}

FlangerAudioProcessorEditor::~FlangerAudioProcessorEditor()
{
}

//==============================================================================
void FlangerAudioProcessorEditor::paint (juce::Graphics& g)
{

    g.fillAll(juce::Colour (5, 0, 5));

    g.setColour(*babyBlue);
    auto frame = juce::Rectangle<float>(getWidth() * 0.15f, getHeight() * 0.2f, getWidth() * 0.7f, getHeight() * 0.6f);
    g.drawRoundedRectangle(frame, 3.f, 3.f);

  //  g.setColour(babyBlue->withAlpha(0.8f));
    g.drawLine(getWidth() * 0.15f, getHeight() * 0.2f, 0, 0, 2.f);
    g.drawLine(frame.getBottomLeft().getX(), frame.getBottomLeft().getY(), 0, getHeight(), 2.f);
    g.drawLine(frame.getTopRight().getX(), frame.getTopRight().getY(), getWidth(), 0, 2.f);
    g.drawLine(frame.getBottomRight().getX(), frame.getBottomRight().getY(), getWidth(), getHeight(), 2.f);

    g.setColour(juce::Colour (15, 0, 15));
    g.fillRoundedRectangle(frame, 3.f);


    int numCombs1 = audioProcessor.getDelay(0) / 2.f;

    for (int comb = 1; comb < numCombs1; comb++)
    {
        g.setColour(yellow->withAlpha(0.7f));

        float spacing = pow((double) comb, 2);
        auto bounds = juce::Rectangle<float>(100, 150, 125 + spacing, 125 + spacing);
        bounds.setCentre(165, 150);
        g.drawEllipse(bounds, 2.f);
    }


    int numCombs2 = audioProcessor.getDelay(1) / 2.f;

    for (int comb = 1; comb < numCombs2; comb++)
    {
        g.setColour(babyBlue->withAlpha(0.7f));

        float spacing = pow((double)comb, 2);
        auto bounds = juce::Rectangle<float>(100, 150, 125 + spacing, 125 + spacing);
        bounds.setCentre(333, 150);
        g.drawEllipse(bounds, 2.f);
    }

    g.setColour(juce::Colours::white);

    juce::GlyphArrangement ga;
    ga.addLineOfText(juce::Font("Consolas", "Regular", 10.f), "Depth      Rate    Feedback", 10, 151);
    juce::Path p;
    ga.createPath(p);
    auto pathBounds = p.getBounds();
    p.applyTransform(juce::AffineTransform().rotated(juce::MathConstants<float>::pi/2, pathBounds.getCentreX(), pathBounds.getCentreY()).
        translated(- pathBounds.getWidth() * 0.5f, pathBounds.getHeight() * 0.5f));
    g.fillPath(p);

    juce::GlyphArrangement ga1;
    ga1.addLineOfText(juce::Font("Consolas", "Regular", 10.f), "Depth      Rate    Feedback", 490, 151);
    juce::Path p1;
    ga1.createPath(p1);
    auto pathBounds1 = p1.getBounds();
    p1.applyTransform(juce::AffineTransform().rotated(juce::MathConstants<float>::pi / 2, pathBounds1.getCentreX(), pathBounds1.getCentreY()).
        translated(-pathBounds1.getWidth() * 0.5f, pathBounds1.getHeight() * 0.5f));
    g.fillPath(p1);

    g.drawRect(0, 0, getWidth(), getHeight(), 2);
    g.setFont(juce::Font("Consolas", "Regular", 24.f));
    juce::String str = "Flanger";
    g.drawText(str, 200, 10, 100, 20, juce::Justification::centred);

    //g.setFont(juce::Font("Consolas", "Regular", 14.f));
    //juce::String de = "Depth";
    //g.drawText(de, 100, 10, 100, 20, juce::Justification::centred);

}

void FlangerAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

}

void FlangerAudioProcessorEditor::timerCallback()
{

    for (int i = 0; i < tapeReels.size(); i++)
        tapeReels[i]->incrementPhase();

    tapeReels[0]->setCentreRelative(0.33f, 0.5f);
    tapeReels[1]->setCentreRelative(0.67f, 0.5f);
    repaint();
}

void FlangerAudioProcessorEditor::sliderValueChanged(juce::Slider* s)
{

}

void FlangerAudioProcessorEditor::buttonClicked(juce::Button* b)
{
    if (b == buttonHigh1.get())
        audioProcessor.setLFOMode(0, 0);
    if (b == buttonHigh2.get())
        audioProcessor.setLFOMode(1, 0);
    if (b == buttonMid1.get())
        audioProcessor.setLFOMode(0, 1);
    if (b == buttonMid2.get())
        audioProcessor.setLFOMode(1, 1);
    if (b == buttonLow1.get())
        audioProcessor.setLFOMode(0, 2);
    if (b == buttonLow2.get())
        audioProcessor.setLFOMode(1, 2);
}

void FlangerAudioProcessorEditor::initColors()
{
    lavender = std::make_unique<juce::Colour>(119, 0, 166);
    pink = std::make_unique<juce::Colour>(254, 0, 254);
    yellow = std::make_unique<juce::Colour>(222, 254, 71);
    babyBlue = std::make_unique<juce::Colour>(115, 255, 254);


}




