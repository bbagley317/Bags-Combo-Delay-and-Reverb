/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BagsComboAudioProcessorEditor::BagsComboAudioProcessorEditor(BagsComboAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Set pluggin size
    setSize(400, 200);

    // Define the parameters of delayTimeController slider
    delayTimeController.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    delayTimeController.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 50, 20);
    delayTimeController.setRange(0.0, 1.0, 0.01);
    delayTimeController.setLookAndFeel(&delayLookAndFeel);
    delayTimeController.setValue(0.25);

    // Define the parameters of gainController slider
    gainController.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 50, 20);
    gainController.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    gainController.setRange(0.0, 1.0, 0.01);
    gainController.setLookAndFeel(&reverbLookAndFeel); // temp


   // Add and make visible on plugin
    addAndMakeVisible(delayTimeController);
    addAndMakeVisible(gainController);

    // Add listeners to the sliders
    delayTimeController.addListener(this);
    gainController.addListener(this);
}

BagsComboAudioProcessorEditor::~BagsComboAudioProcessorEditor()
{
}

//==============================================================================
void BagsComboAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff313638)); // Dark grey
    g.setFont(15.0f);
    g.setColour(juce::Colours::white);
    g.drawFittedText("BAGS COMBO DELAY & REVERB", 0, 0, getWidth(), 30, juce::Justification::centred, 1);
}

void BagsComboAudioProcessorEditor::resized()
{
    const int border = 20; 
    const int dialWidth = getWidth() / 2 - border;
    const int dialHeight = getHeight() - border - 100;

    delayTimeController.setBounds(border, border + 30, dialWidth, dialHeight);
    gainController.setBounds(border + dialWidth, border + 30, dialWidth, dialHeight);

}

void BagsComboAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    audioProcessor.delayTime = static_cast<float>(delayTimeController.getValue());
    audioProcessor.gainLevel = static_cast<float>(gainController.getValue());
}
