/*
  ==============================================================================

    ADSRVisualizerSlider.h
    Created: 6 Jun 2022 5:51:25pm
    Author:  Ivan Khulup

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ADSRVisualizerSliderLaF.h"

using SliderAttachement = juce::AudioProcessorValueTreeState::SliderAttachment;

class ADSRVisualizerSlider: public juce::Slider
{
    std::unique_ptr<SliderAttachement> sliderAttachment;
    std::unique_ptr<ADSRVisualizerSliderLaF> lookAndFeel { new ADSRVisualizerSliderLaF() };
public:
    ADSRVisualizerSlider();
    
    void mouseEnter(const juce::MouseEvent& event) override;
    void mouseExit(const juce::MouseEvent& event) override;
    
    float getCurrentThumbPosition();
    void attachApvts(juce::AudioProcessorValueTreeState& apvts, const juce::String& parameterId);
};
