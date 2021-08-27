/*
  ==============================================================================

    lfoControls.h
    Created: 31 Jul 2021 5:21:42pm
    Author:  ryand

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include <MixFeel.h>

//==============================================================================
/*
*/
class lfoControls  : public juce::Component
{
public:
    lfoControls();
    ~lfoControls() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (lfoControls)
};
