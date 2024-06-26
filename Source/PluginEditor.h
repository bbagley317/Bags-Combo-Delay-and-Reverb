/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class BagsComboAudioProcessorEditor  : public juce::AudioProcessorEditor, private juce::Slider::Listener // [2]
{
public:
    BagsComboAudioProcessorEditor (BagsComboAudioProcessor&);
    ~BagsComboAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void sliderValueChanged(juce::Slider* slider) override; // [3]

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    BagsComboAudioProcessor& audioProcessor;

    juce::Slider delayTimeController;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BagsComboAudioProcessorEditor)
};
