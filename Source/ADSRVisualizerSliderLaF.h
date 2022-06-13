/*
  ==============================================================================

    ADSRVisualizerSliderLaF.h
    Created: 3 Jun 2022 1:42:29pm
    Author:  Ivan Khulup

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

using namespace juce;

class ADSRVisualizerSliderLaF: public juce::LookAndFeel_V4 {
public:
  
    void drawLinearSlider(Graphics& graphics, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle sliderStyle, Slider& slider) override
    {
        auto radius = juce::LookAndFeel_V2::getSliderThumbRadius(slider);
        auto trackWidth = 6;
        auto cornerRadius = trackWidth / 2;
        auto trackCenter = x + (width / 2);
// draw track
//        graphics.setColour(slider.findColour(slider.trackColourId));
//        graphics.fillRoundedRectangle(trackCenter - trackWidth / 2, y - cornerRadius, trackWidth, height + cornerRadius * 2, cornerRadius);
        
        graphics.setColour(slider.findColour(slider.thumbColourId));
        graphics.fillEllipse(trackCenter - radius / 2, sliderPos - radius / 2, radius , radius);
    };
};
