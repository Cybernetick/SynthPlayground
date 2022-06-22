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

class ADSRVisualizerSliderLaF: public LookAndFeel_V4 {
public:

    void drawLinearSlider(Graphics& graphics, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle sliderStyle, Slider& slider) override
    {
        auto radius = (slider.isMouseOverOrDragging()) ? LookAndFeel_V4::getSliderThumbRadius(slider) * 1.4: LookAndFeel_V4::getSliderThumbRadius(slider) ;
        auto trackWidth = 6;
        auto cornerRadius = trackWidth / 2;
        auto trackCenter = x + (width / 2);
// draw track
//        graphics.setColour(slider.findColour(slider.trackColourId));
//        graphics.fillRoundedRectangle(trackCenter - trackWidth / 2, y - cornerRadius, trackWidth, height + cornerRadius * 2, cornerRadius);
        graphics.setColour(slider.findColour(slider.thumbColourId));
        graphics.fillEllipse(static_cast<float>(trackCenter - radius / 2.0), static_cast<float>(sliderPos - radius / 2),
                             static_cast<float>(radius), static_cast<float>(radius));
    }
};
