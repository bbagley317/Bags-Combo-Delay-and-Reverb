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

    roomSizeController.setRange(0.0, 1.0, 0.05);
    roomSizeController.setValue(0.5);

    widthController.setRange(0.0, 1.0, 0.05);
    widthController.setValue(0.5);

    dampController.setRange(0.0, 1.0, 0.05);
    dampController.setValue(0.25);

    wetLevelController.setRange(0.0, 1.0, 0.05);
    wetLevelController.setValue(0.33);

    dryLevelController.setRange(0.0, 1.0, 0.05);
    dryLevelController.setValue(0.4);

    // Define the parameters of gainController slider
    gainController.setRange(0.0, 1.0, 0.01);
    gainController.setSliderStyle(juce::Slider::SliderStyle::Rotary);


   // Add controllers and make visible 
    addAndMakeVisible(gainController);

    addAndMakeVisible(delayLevelController);
    addAndMakeVisible(delayTimeController);
    //addAndMakeVisible(d3);
    //addAndMakeVisible(d4);
    //addAndMakeVisible(d5);
    //addAndMakeVisible(d6);

    addAndMakeVisible(roomSizeController);
    addAndMakeVisible(widthController);
    addAndMakeVisible(dampController);
    addAndMakeVisible(wetLevelController);
    addAndMakeVisible(dryLevelController);
    //addAndMakeVisible(r6);
    
    // Add listeners to the sliders
    delayLevelController.addListener(this);
    delayTimeController.addListener(this);
    
    roomSizeController.addListener(this);
    widthController.addListener(this);
    dampController.addListener(this);
    wetLevelController.addListener(this);
    dryLevelController.addListener(this);


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

    roomSizeController.setLookAndFeel(nullptr);
    widthController.setLookAndFeel(nullptr);
    dampController.setLookAndFeel(nullptr);
    wetLevelController.setLookAndFeel(nullptr);
    dryLevelController.setLookAndFeel(nullptr);
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
    roomSizeController.setBounds(rightBorder, border + headerHeight, dialWidth, dialHeight);
    widthController.setBounds(rightBorder + dialWidth + padding, border + headerHeight, dialWidth, dialHeight);
    dampController.setBounds(rightBorder + 2 * (dialWidth + padding), border + headerHeight, dialWidth, dialHeight);
    wetLevelController.setBounds(rightBorder, border + headerHeight + dialHeight + 4*padding, dialWidth, dialHeight);
    dryLevelController.setBounds(rightBorder + dialWidth + padding, border + headerHeight + dialHeight + 4*padding, dialWidth, dialHeight);
    r6.setBounds(rightBorder + 2 * (dialWidth + padding), border + headerHeight + dialHeight + 4*padding, dialWidth, dialHeight);

    // Arrange gain controller below the grid in the center
    gainController.setBounds((getWidth() - dialWidth) / 2, getHeight() - border - dialHeight, dialWidth, dialHeight);
}


void BagsComboAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    audioProcessor.delayLevel = static_cast<float>(delayLevelController.getValue());
    audioProcessor.delayTime = static_cast<float>(delayTimeController.getValue());
    
    audioProcessor.roomSize = static_cast<float>(roomSizeController.getValue());
    audioProcessor.width = static_cast<float>(widthController.getValue());
    audioProcessor.damp = static_cast<float>(dampController.getValue());
    audioProcessor.wetLevel = static_cast<float>(wetLevelController.getValue());
    audioProcessor.dryLevel = static_cast<float>(dryLevelController.getValue());

    audioProcessor.gainLevel = static_cast<float>(gainController.getValue());
    
}


