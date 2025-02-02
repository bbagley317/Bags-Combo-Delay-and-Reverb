/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BagsComboAudioProcessor::BagsComboAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

BagsComboAudioProcessor::~BagsComboAudioProcessor()
{
}

//==============================================================================
const juce::String BagsComboAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BagsComboAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BagsComboAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BagsComboAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BagsComboAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BagsComboAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int BagsComboAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BagsComboAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String BagsComboAudioProcessor::getProgramName (int index)
{
    return {};
}

void BagsComboAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void BagsComboAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mDelayBuffer.setSize(2, 2*mSampleRate);
    mDelayBuffer.clear();
}

void BagsComboAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BagsComboAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void BagsComboAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDeNormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();


    // clear channels
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // Apply our delay effect to the new output..
    applyDelay(buffer, mDelayBuffer, delayLevel, delayTime);

    // Apply reverb effect 
    applyReverb(buffer, roomSize, width, damp, wetLevel, dryLevel);

    // Apply our gain change to the outgoing data..
    applyGain(buffer, mDelayBuffer, gainLevel);

    
}

void BagsComboAudioProcessor::applyGain(juce::AudioBuffer<float>& buffer, juce::AudioBuffer<float>& delayBuffer, float gain)
{
    ignoreUnused(delayBuffer);

    for (auto channel = 0; channel < getTotalNumOutputChannels(); ++channel)
        buffer.applyGain(channel, 0, buffer.getNumSamples(), gain);
}

void BagsComboAudioProcessor::applyDelay(juce::AudioBuffer<float>& buffer, juce::AudioBuffer<float>& delayBuffer, float delayLevel, float delayTime)
{
    auto numSamples = buffer.getNumSamples();
    auto sampleRate = getSampleRate(); 

    int delayWritePos = mDelayPosition; // Initialize delay write position

    for (auto channel = 0; channel < getTotalNumOutputChannels(); ++channel)
    {
        auto channelData = buffer.getWritePointer(channel);
        auto delayData = delayBuffer.getWritePointer(juce::jmin(channel, delayBuffer.getNumChannels() - 1));

        delayWritePos = mDelayPosition;

        for (auto sample = 0; sample < numSamples; ++sample)
        {
            // Convert delay time from milliseconds to samples
            int delaySamples = static_cast<int>(delayTime / 1000 * mSampleRate);

            // Calculate the position in the delay buffer considering the delay time
            int delayReadPos = (delayWritePos + delayBuffer.getNumSamples() - delaySamples) % delayBuffer.getNumSamples();


            auto in = channelData[sample];                               // get original sample
            channelData[sample] += delayData[delayReadPos] * delayLevel; // add effects to delay sample and add to main buffer
            delayData[delayWritePos] = (channelData[sample]);            // add main buffer sample to delay buffer

            if (++delayWritePos >= delayBuffer.getNumSamples())
                delayWritePos = 0;
        }
    }

    mDelayPosition = delayWritePos;
}


void BagsComboAudioProcessor::applyReverb(juce::AudioBuffer<float>& buffer, float roomSize, float width, float damp, float wetLevel, float dryLevel)
{
    // Set the reverb parameters
    juce::Reverb::Parameters reverbParameters;
    reverbParameters.roomSize = roomSize;
    reverbParameters.damping = damp;
    reverbParameters.width = width;
    reverbParameters.wetLevel = wetLevel;
    reverbParameters.dryLevel = dryLevel;
    reverb.setParameters(reverbParameters);


    // Apply reverb to the audio buffer
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
    {
        float* channelData = buffer.getWritePointer(channel);
        reverb.processMono(channelData, buffer.getNumSamples());
    }


}

//==============================================================================
bool BagsComboAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* BagsComboAudioProcessor::createEditor()
{
    return new BagsComboAudioProcessorEditor (*this);
}

//==============================================================================
void BagsComboAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void BagsComboAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BagsComboAudioProcessor();
}
