/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include <MixFeel.h>
#include "TapeReel.h"
//==============================================================================
/**
*/
class FlangerAudioProcessorEditor  : public juce::AudioProcessorEditor, juce::Timer, public juce::Slider::Listener, juce::Button::Listener
{
public:
    FlangerAudioProcessorEditor (FlangerAudioProcessor&);
    ~FlangerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void timerCallback() override;
    void sliderValueChanged(juce::Slider* s) override;
    void buttonClicked(juce::Button* b) override;

    void initColors();
   
private:
    juce::OwnedArray<TapeReel> tapeReels;
    MixFeel mixFeel;

    std::unique_ptr<juce::Slider>   amp1;
    std::unique_ptr<juce::Label>    label1;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attach1;

    std::unique_ptr<juce::Slider>   amp2;
    std::unique_ptr<juce::Label>    label2;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attach2;

    std::unique_ptr<juce::Slider>   rate1;
    std::unique_ptr<juce::Label>    rlabel1;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rattach1;

    std::unique_ptr<juce::Slider>   rate2;
    std::unique_ptr<juce::Label>    rlabel2;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rattach2;

    std::unique_ptr<juce::Slider>   fb1;
    std::unique_ptr<juce::Label>    fbLabel1;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> fbAttach1;

    std::unique_ptr<juce::Slider>   fb2;
    std::unique_ptr<juce::Label>    fbLabel2;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> fbAttach2;

    std::unique_ptr<juce::TextButton> buttonHigh1;
    std::unique_ptr<juce::TextButton> buttonHigh2;
    std::unique_ptr<juce::TextButton> buttonMid1;
    std::unique_ptr<juce::TextButton> buttonMid2;
    std::unique_ptr<juce::TextButton> buttonLow1;
    std::unique_ptr<juce::TextButton> buttonLow2;
    std::unique_ptr<juce::Colour> lavender;
    std::unique_ptr<juce::Colour> pink;
    std::unique_ptr<juce::Colour> yellow;
    std::unique_ptr<juce::Colour> babyBlue;
    std::unique_ptr<juce::Colour> blue;


    FlangerAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FlangerAudioProcessorEditor)
};
