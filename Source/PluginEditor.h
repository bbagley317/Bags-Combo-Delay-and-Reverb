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


/*
 Color Scheme Ref... 
 0xff313638  dark grey (dial fill)
 0xff93b7be  light blue (reverb controls) 
 0xff35ff69  bright green (delay controls)
 0xfff06543  tomato 
 */


// LookAndFeel class for delay controls
class DelayLookAndFeel : public juce::LookAndFeel_V4 {
public:
    DelayLookAndFeel() 
    {
        setColour(juce::Slider::thumbColourId, juce::Colours::limegreen);
    };

};

// LookAndFeel class for reverb controls
class ReverbLookAndFeel : public juce::LookAndFeel_V4 {
public:
    ReverbLookAndFeel()
    {
        setColour(juce::Slider::thumbColourId, juce::Colour(0xff23B5D3)); // Bright Blue
    };

};




class CustomController : public juce::Slider {
public:
    CustomController(const juce::String& labelText, juce::LookAndFeel_V4* lookAndFeel)
      : rotaryStartAngle(juce::MathConstants<float>::pi * 1.2f),
        rotaryEndAngle(juce::MathConstants<float>::pi * 2.8f)
    {
        setSliderStyle(juce::Slider::SliderStyle::Rotary);
        setLookAndFeel(lookAndFeel);
        setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

        label.setText(labelText, juce::dontSendNotification);
        label.setJustificationType(juce::Justification::centredBottom);
        label.attachToComponent(this,false); 

    }

    ~CustomController()
    {
        setLookAndFeel(nullptr);
    }

    void paint(juce::Graphics& g) override {
        auto radius = (float)juce::jmin(getWidth() / 2, getHeight() / 2) - 8.0f;
        auto centreX = (float)getWidth() * 0.5f;
        auto centreY = (float)getHeight() * 0.5f;
        auto rx = centreX - radius;
        auto ry = centreY - radius;
        auto rw = radius * 2.0f;
        float sliderPos = (float)(getValue() - getMinimum()) / (float)(getMaximum() - getMinimum());
        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

        g.setColour(juce::Colour(0xff313638)); // Dark Grey
        g.fillEllipse(rx, ry, rw, rw);

        // outline
        g.setColour(findColour(juce::Slider::thumbColourId)); // Use thumb colour
        g.drawEllipse(rx, ry, rw, rw, 4.0f);

        juce::Path p;
        auto pointerLength = radius * 0.6f;
        auto pointerThickness = 8.0f;

        p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
        p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));

        // pointer
        g.fillPath(p);
    }

private:
    float rotaryStartAngle;
    float rotaryEndAngle;
    juce::Label label;
};

class BagsComboAudioProcessorEditor  : public juce::AudioProcessorEditor, private juce::Slider::Listener // [2]
{
public:
    BagsComboAudioProcessorEditor (BagsComboAudioProcessor&);
    ~BagsComboAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
private:
    void sliderValueChanged(juce::Slider* slider) override;

    BagsComboAudioProcessor& audioProcessor;
    DelayLookAndFeel delayLookAndFeel;
    ReverbLookAndFeel reverbLookAndFeel;

    CustomController delayTimeController{"time", &delayLookAndFeel};
    CustomController d2 {"d2", & delayLookAndFeel};
    CustomController d3 {"d3", &delayLookAndFeel };
    CustomController d4 {"d4", &delayLookAndFeel };
    CustomController d5 {"d5", &delayLookAndFeel };
    CustomController d6 {"d6", &delayLookAndFeel };
                         
    CustomController r1 {"r1", &reverbLookAndFeel };
    CustomController r2 {"r2", &reverbLookAndFeel };
    CustomController r3 {"r3", &reverbLookAndFeel };
    CustomController r4 {"r4", &reverbLookAndFeel };
    CustomController r5 {"r5", &reverbLookAndFeel };
    CustomController r6 {"r6", &reverbLookAndFeel };
                        
    CustomController gainController {"Gain", &reverbLookAndFeel};


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BagsComboAudioProcessorEditor)
};