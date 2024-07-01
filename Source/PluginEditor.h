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

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&) override
    {
        auto radius = (float)juce::jmin(width / 2, height / 2) - 4.0f;
        auto centreX = (float)x + (float)width * 0.5f;
        auto centreY = (float)y + (float)height * 0.5f;
        auto rx = centreX - radius;
        auto ry = centreY - radius;
        auto rw = radius * 2.0f;
        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

        g.setColour(juce::Colour(0xff313638)); // Dark Grey
        g.fillEllipse(rx, ry, rw, rw);

        // outline
        g.setColour(juce::Colour(0xff35ff69)); // Bright Green
        g.drawEllipse(rx, ry, rw, rw, 4.0f);

        juce::Path p;
        auto pointerLength = radius * 0.5f;
        auto pointerThickness = 8.0f;
        p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
        p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));

        // pointer
        g.setColour(juce::Colour(0xff35ff69)); // Bright Green
        g.fillPath(p);
    }

};

// LookAndFeel class for reverb controls
class ReverbLookAndFeel : public juce::LookAndFeel_V4 {
public:
    ReverbLookAndFeel()
    {
        setColour(juce::Slider::thumbColourId, juce::Colour(0xff23B5D3)); // Bright Blue
    };

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&) override
    {
        auto radius = (float)juce::jmin(width / 2, height / 2) - 4.0f;
        auto centreX = (float)x + (float)width * 0.5f;
        auto centreY = (float)y + (float)height * 0.5f;
        auto rx = centreX - radius;
        auto ry = centreY - radius;
        auto rw = radius * 2.0f;
        auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

        g.setColour(juce::Colour(0xff313638)); // Dark Grey
        g.fillEllipse(rx, ry, rw, rw);

        // outline
        g.setColour(juce::Colour(0xff23B5D3)); // Bright Blue
        g.drawEllipse(rx, ry, rw, rw, 4.0f);

        juce::Path p;
        auto pointerLength = radius * 0.5f;
        auto pointerThickness = 8.0f;
        p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
        p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));

        // pointer
        g.setColour(juce::Colour(0xff23B5D3)); // Bright Blue
        g.fillPath(p);
    }

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

    juce::Slider delayTimeController;
    juce::Slider gainController;

    DelayLookAndFeel delayLookAndFeel;
    ReverbLookAndFeel reverbLookAndFeel;



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BagsComboAudioProcessorEditor)
};