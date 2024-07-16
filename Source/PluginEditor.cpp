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

    // Define the parameters of delayLevelController slider
    delayLevelController.setRange(0.0, 1.0, 0.01);
    delayLevelController.setValue(0.25);

    delayTimeController.setRange(0.0, 1000.0, 1.0);
    delayTimeController.showTextBox();

    // Define the parameters of gainController slider
    gainController.setRange(0.0, 1.0, 0.01);
    gainController.setSliderStyle(juce::Slider::SliderStyle::Rotary);


   // Add controllers and make visible 
    addAndMakeVisible(gainController);

    addAndMakeVisible(delayLevelController);
    addAndMakeVisible(delayTimeController);
    addAndMakeVisible(d3);
    addAndMakeVisible(d4);
    addAndMakeVisible(d5);
    addAndMakeVisible(d6);

    addAndMakeVisible(r1);
    addAndMakeVisible(r2);
    addAndMakeVisible(r3);
    addAndMakeVisible(r4);
    addAndMakeVisible(r5);
    addAndMakeVisible(r6);
    
    // Add listeners to the sliders
    delayLevelController.addListener(this);
    delayTimeController.addListener(this);
    gainController.addListener(this);
}

BagsComboAudioProcessorEditor::~BagsComboAudioProcessorEditor()
{
    // Reset look and feel when plugin closes
    gainController.setLookAndFeel(nullptr); 
    delayLevelController.setLookAndFeel(nullptr);
    delayTimeController.setLookAndFeel(nullptr);
    d3.setLookAndFeel(nullptr);
    d4.setLookAndFeel(nullptr);
    d5.setLookAndFeel(nullptr);
    d6.setLookAndFeel(nullptr);
    r1.setLookAndFeel(nullptr);
    r2.setLookAndFeel(nullptr);
    r3.setLookAndFeel(nullptr);
    r4.setLookAndFeel(nullptr);
    r5.setLookAndFeel(nullptr);
    r6.setLookAndFeel(nullptr);
}

//==============================================================================
void BagsComboAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff313638)); // Dark grey
    g.setFont(15.0f);
    g.setColour(juce::Colours::white);
    g.drawFittedText("BAGS COMBO DELAY & REVERB", 0, 0, getWidth(), 30, juce::Justification::centred, 1);
    
    const int border = 20;
    const int padding = 5;

    // Proportional to plugin width = 400, height = 300
    const int dialWidth = (getWidth() / 8);
    const int dialHeight = (getHeight() / 6);

    // Draw boxes around the dials
    const int headerHeight = 50;
    const int boxWidth = 3 * dialWidth + 2 * padding;
    const int boxHeight = 2 * dialHeight + 7 * padding;

    g.drawRect(border - padding, border + headerHeight - 4*padding, boxWidth + 2 * padding, boxHeight + 2 * padding);
    g.drawRect(getWidth() / 2 + border - padding, border + headerHeight - 4*padding, boxWidth + 2 * padding, boxHeight + 2 * padding);
    
    // Draw labels above the boxes
    g.drawFittedText("DELAY", border, border + padding , boxWidth + 2 * padding, 30, juce::Justification::centred, 1);
    g.drawFittedText("REVERB", getWidth() / 2 + border, border + padding, boxWidth + 2 * padding, 30, juce::Justification::centred, 1);

}

void BagsComboAudioProcessorEditor::resized()
{
    const int border = 20;
    const int padding = 5;
    const int headerHeight = 50;

    // Proportional to plugin width = 400, height = 300
    const int dialWidth = (getWidth() / 8); 
    const int dialHeight = (getHeight() / 6); 

    // Arrange delay controllers in 3 by 2 grid on the left
    delayLevelController.setBounds(border, border + headerHeight, dialWidth, dialHeight);
    delayTimeController.setBounds(border + dialWidth + padding, border + headerHeight, dialWidth, dialHeight);
    d3.setBounds(border + 2 * (dialWidth + padding), border + headerHeight, dialWidth, dialHeight);
    d4.setBounds(border, border + headerHeight + dialHeight + 4*padding, dialWidth, dialHeight);
    d5.setBounds(border + dialWidth + padding, border + headerHeight + dialHeight + 4*padding, dialWidth, dialHeight);
    d6.setBounds(border + 2 * (dialWidth + padding), border + headerHeight + dialHeight + 4*padding, dialWidth, dialHeight);

    // Arrange reverb controllers in 3 by 2 grid on the right
    const int rightBorder = getWidth() / 2 + border;
    r1.setBounds(rightBorder, border + headerHeight, dialWidth, dialHeight);
    r2.setBounds(rightBorder + dialWidth + padding, border + headerHeight, dialWidth, dialHeight);
    r3.setBounds(rightBorder + 2 * (dialWidth + padding), border + headerHeight, dialWidth, dialHeight);
    r4.setBounds(rightBorder, border + headerHeight + dialHeight + 4*padding, dialWidth, dialHeight);
    r5.setBounds(rightBorder + dialWidth + padding, border + headerHeight + dialHeight + 4*padding, dialWidth, dialHeight);
    r6.setBounds(rightBorder + 2 * (dialWidth + padding), border + headerHeight + dialHeight + 4*padding, dialWidth, dialHeight);

    // Arrange gain controller below the grid in the center
    gainController.setBounds((getWidth() - dialWidth) / 2, getHeight() - border - dialHeight, dialWidth, dialHeight);
}


void BagsComboAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    audioProcessor.delayLevel = static_cast<float>(delayLevelController.getValue());
    audioProcessor.delayTime = static_cast<float>(delayTimeController.getValue());
    audioProcessor.gainLevel = static_cast<float>(gainController.getValue());
   

}


