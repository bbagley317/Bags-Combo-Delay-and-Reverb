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
    setSize(400,300);

    // Define the parameters of delayTimeController slider
    delayTimeController.setRange(0.0, 1.0, 0.01);
    delayTimeController.setValue(0.25);

    // Define the parameters of gainController slider
    gainController.setRange(0.0, 1.0, 0.01);


    //Set slier styles 
    gainController.setSliderStyle(juce::Slider::SliderStyle::Rotary);

    delayTimeController.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    d2.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    d3.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    d4.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    d5.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    d6.setSliderStyle(juce::Slider::SliderStyle::Rotary);

    r1.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    r2.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    r3.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    r4.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    r5.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    r6.setSliderStyle(juce::Slider::SliderStyle::Rotary);

    // Set look and feel for controllers 
    gainController.setLookAndFeel(&reverbLookAndFeel); // temp

    delayTimeController.setLookAndFeel(&delayLookAndFeel);
    d2.setLookAndFeel(&delayLookAndFeel);
    d3.setLookAndFeel(&delayLookAndFeel);
    d4.setLookAndFeel(&delayLookAndFeel);
    d5.setLookAndFeel(&delayLookAndFeel);
    d6.setLookAndFeel(&delayLookAndFeel);

    r1.setLookAndFeel(&reverbLookAndFeel);
    r2.setLookAndFeel(&reverbLookAndFeel);
    r3.setLookAndFeel(&reverbLookAndFeel);
    r4.setLookAndFeel(&reverbLookAndFeel);
    r5.setLookAndFeel(&reverbLookAndFeel);
    r6.setLookAndFeel(&reverbLookAndFeel);


   // Add controllers and make visible 
    addAndMakeVisible(d2);
    addAndMakeVisible(d3);
    addAndMakeVisible(d4);
    addAndMakeVisible(d5);
    addAndMakeVisible(d6);
    addAndMakeVisible(delayTimeController);

    addAndMakeVisible(gainController);

    addAndMakeVisible(r1);
    addAndMakeVisible(r2);
    addAndMakeVisible(r3);
    addAndMakeVisible(r4);
    addAndMakeVisible(r5);
    addAndMakeVisible(r6);
    

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
    

    // Draw boxes around the dials
    const int border = 20;
    const int padding = 10;
    const int headerHeight = 50;
    const int dialWidth = (getWidth() / 2 - 2 * border - 2 * padding) / 3;
    const int dialHeight = 22;
    const int boxWidth = 3 * dialWidth + 2 * padding;
    const int boxHeight = 2 * dialHeight + 2 * padding;

    g.drawRect(border - padding, border + headerHeight - padding, boxWidth + 2 * padding, boxHeight + 2 * padding);
    g.drawRect(getWidth() / 2 + border - padding, border + headerHeight - padding, boxWidth + 2 * padding, boxHeight + 2 * padding);

    // Draw labels above the boxes
    g.drawFittedText("DELAY", border - padding, border - padding - 30, boxWidth + 2 * padding, 30, juce::Justification::centred, 1);
    g.drawFittedText("REVERB", getWidth() / 2 + border - padding, border - padding - 30, boxWidth + 2 * padding, 30, juce::Justification::centred, 1);

}

void BagsComboAudioProcessorEditor::resized()
{
    const int border = 20;
    const int padding = 10;
    const int dialWidth = (getWidth() / 2 - 2 * border - 2 * padding) / 3;
    const int dialHeight = (getHeight() - 2 * border - padding) / 2;

    // Arrange delay controllers in 3 by 2 grid on the left
    delayTimeController.setBounds(border, border, dialWidth, dialHeight);
    d2.setBounds(border + dialWidth + padding, border, dialWidth, dialHeight);
    d3.setBounds(border + 2 * (dialWidth + padding), border, dialWidth, dialHeight);
    d4.setBounds(border, border + 4*padding, dialWidth, dialHeight);
    d5.setBounds(border + dialWidth + padding, border + 4*padding, dialWidth, dialHeight);
    d6.setBounds(border + 2 * (dialWidth + padding), border + 4*padding, dialWidth, dialHeight);

    // Arrange reverb controllers in 3 by 2 grid on the right
    const int rightBorder = getWidth() / 2 + border;
    r1.setBounds(rightBorder, border, dialWidth, dialHeight);
    r2.setBounds(rightBorder + dialWidth + padding, border, dialWidth, dialHeight);
    r3.setBounds(rightBorder + 2 * (dialWidth + padding), border, dialWidth, dialHeight);
    r4.setBounds(rightBorder, border + 4*padding, dialWidth, dialHeight);
    r5.setBounds(rightBorder + dialWidth + padding, border + 4*padding, dialWidth, dialHeight);
    r6.setBounds(rightBorder + 2 * (dialWidth + padding), border + 4*padding, dialWidth, dialHeight);

    // Arrange gain controller below the grid in the center
    gainController.setBounds((getWidth() - dialWidth) / 2, getHeight() - border - dialHeight, dialWidth, dialHeight);
}

void BagsComboAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    audioProcessor.delayTime = static_cast<float>(delayTimeController.getValue());
    audioProcessor.gainLevel = static_cast<float>(gainController.getValue());
}
