/*
  ==============================================================================

    DelayBase.h
    Created: 29 Jul 2021 11:38:49am
    Author:  ryand

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <./pedal/TrivialSine.hpp>

class DelayBase
{
public:
    DelayBase() : delayLine(24000) { lfo.setAmplitude(0.f); lfo.setFrequency(0.f); }
    ~DelayBase() {}

    void prepare(double sampleRate, int samplesPerBlock)
    {
        mSampleRate = sampleRate;
        delayLine.prepare({ sampleRate, (juce::uint32)samplesPerBlock, 2 });
        smoothDelay.reset(sampleRate, 0.5);
        lfo.setAmplitude(0.f);
      //  DBG(mSampleRate);
    }

    float getDelayTime() { return delayTimeMS.get(); }

    float getFeedback() { return feedback.get(); }

    void increment() 
    {
        //auto range = juce::NormalisableRange<float>(-1.f, 1.f, 0.0001f, 1.0);
        //float modSample = range.convertTo0to1(lfo.generateSample());
        
        float modSample = sqrt(pow(lfo.generateSample(), 2)) * maxDelayMS;
        float smoothSample = smoothDelay.getNextValue();
        delayTimeMS = juce::jlimit<float>(0.f, maxDelayMS, modSample + smoothSample);
    }

    void pushSample(int channel, float sample)
    {
        delayLine.pushSample(channel, sample);
    }
    float popSample(int channel)
    {
        auto delayTimeSamples = (delayTimeMS.get() * (float) mSampleRate) / 1000.f;
        auto wetSample = delayLine.popSample(channel, delayTimeSamples, true);
        auto sample = wetSample + (lastOutput[channel] * feedback.get());
        lastOutput[channel] = wetSample;
        return sample;
    }

    void setDelayTime(float delayTime) { smoothDelay.setTargetValue(delayTime);  }
    void setFeedback(float fb)      { feedback = fb; }
    void setLFO(float lfoAmount)    { lfo.setAmplitude(lfoAmount); }

    void setLFOFreq(float lfoFreq)  
    { 
        if (fmToggle)
            lfo.setFrequency(lfoFreq * 10.f);
        else if (lowToggle)
            lfo.setFrequency(lfoFreq * 0.1);
        else 
            lfo.setFrequency(lfoFreq);
    }

    void setLFOPhase(float lfoPhase) { lfo.setPhase(lfoPhase); }

    void setLFOMode(int mode)
    {
        switch (mode)
        {
        case 0: fmToggle = true; lowToggle = false;
            break;
        case 1: fmToggle = false; lowToggle = false;
            break;
        case 2: fmToggle = false; lowToggle = true;
            break;
        }
 
    }
    void setFM(bool toggle) { lowToggle = false;  fmToggle = toggle; }
    void setLow(bool toggle) { fmToggle = false; lowToggle = toggle; }
    //void debug() { DBG(delayTimeMS.get()); }


private:
    juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Lagrange3rd> delayLine;
    juce::SmoothedValue<float> smoothDelay;
    juce::Atomic<float> delayTimeMS { 0.f };
    juce::Atomic<float> feedback { 0.f };
    std::vector<float> lastOutput{ 0.f, 0.f };

    pedal::TrivialSine lfo;

    const float maxDelayMS = 100;
    double mSampleRate = -1;
    bool fmToggle = false;
    bool lowToggle = false;
};