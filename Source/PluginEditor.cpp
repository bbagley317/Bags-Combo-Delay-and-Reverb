/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BagsComboAudioProcessorEditor::BagsComboAudioProcessorEditor (BagsComboAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    setSize(200, 200);

    // these define the parameters of our slider object
    //delayTimeController.setSliderStyle (juce::Slider::RotaryHorizontalDrag);
    delayTimeController.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    delayTimeController.setTextBoxStyle(juce::Slider::TextBoxAbove,false, 50, 20);
    delayTimeController.setSize(100,100);
    delayTimeController.setRange (0.0, 1000.0, 50.0);
    delayTimeController.setValue (400.0);

    delayTimeController.setTooltip("Change Delay Time");
    delayTimeController.setColour(juce::Slider::thumbColourId, juce::Colours::darkblue);
    delayTimeController.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::darkmagenta);
    delayTimeController.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::black);
    

    addAndMakeVisible(delayTimeController);

    // add the listener to the slider
    delayTimeController.addListener(this);
}

BagsComboAudioProcessorEditor::~BagsComboAudioProcessorEditor()
{
}

//==============================================================================
void BagsComboAudioProcessorEditor::paint (juce::Graphics& g)
{
  
    g.fillAll(juce::Colours::grey);
    g.setColour(juce::Colours::black);
    g.setFont(15.0f);
    g.drawFittedText("Delay", 0, 0, getWidth(), 30, juce::Justification::centred, 1);
}

void BagsComboAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    // sets the position and size of the slider with arguments (x, y, width, height)
    delayTimeController.setBounds(40, 30, 20, getHeight() - 60);
}

void BagsComboAudioProcessorEditor::sliderValueChanged(juce::Slider* slider) 
{
    audioProcessor.delayTime = static_cast<int>(delayTimeController.getValue());
}
