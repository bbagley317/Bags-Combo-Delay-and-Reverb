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
    mDelayBuffer.setSize(2, 12000);
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
    applyDelay(buffer, mDelayBuffer, delayTime);

    // Apply our gain change to the outgoing data..
    applyGain(buffer, mDelayBuffer, gainLevel);
}

void BagsComboAudioProcessor::applyGain(juce::AudioBuffer<float>& buffer, juce::AudioBuffer<float>& delayBuffer, float gain)
{
    ignoreUnused(delayBuffer);

    for (auto channel = 0; channel < getTotalNumOutputChannels(); ++channel)
        buffer.applyGain(channel, 0, buffer.getNumSamples(), gain);
}

void BagsComboAudioProcessor::applyDelay(juce::AudioBuffer<float>& buffer, juce::AudioBuffer<float>& delayBuffer, float delayLevel)
{
    auto numSamples = buffer.getNumSamples();

    auto delayPos = 0;

    for (auto channel = 0; channel < getTotalNumOutputChannels(); ++channel)
    {
        // get write points to main and delay buffers
        auto channelData = buffer.getWritePointer(channel);
        auto delayData = delayBuffer.getWritePointer(juce::jmin(channel, delayBuffer.getNumChannels() - 1));

        delayPos = delayPosition;

        for (auto sample = 0; sample < numSamples; ++sample)
        {
            auto in = channelData[sample];
            channelData[sample] += delayData[delayPos];
            delayData[delayPos] = (delayData[delayPos] + in) * delayLevel;

            if (++delayPos >= delayBuffer.getNumSamples())
                delayPos = 0;
        }
    }

    delayPosition = delayPos;
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
