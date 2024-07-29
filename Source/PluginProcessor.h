/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class BagsComboAudioProcessor : public juce::AudioProcessor
{

public:
    float delayLevel { 0.8 };
    float delayTime { 10.0 };

    float roomSize { 0.5 };
    float width { 0.5 };
    float damp { 0.5 };
    float wetLevel { 0.33 };
    float dryLevel{ 0.4 };

    float gainLevel{ 0.8 };

public:
    //==============================================================================
    BagsComboAudioProcessor();
    ~BagsComboAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;


    void applyDelay(juce::AudioBuffer<float>& buffer, juce::AudioBuffer<float>& delayBuffer, float delayLevel, float delayTime);
    void applyGain(juce::AudioBuffer<float>& buffer, juce::AudioBuffer<float>& delayBuffer, float gainLevel);
    void applyReverb(juce::AudioBuffer<float>& buffer, float roomSize, float damping, float width, float wetLevel, float dryLevel);



private:
    juce::AudioBuffer<float> mDelayBuffer;
    int mDelayPosition{ 0 };
    juce::Reverb reverb;
    int mSampleRate{ 44100 };

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BagsComboAudioProcessor)
};
