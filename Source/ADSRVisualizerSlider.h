/*
  ==============================================================================

    ADSRVisualizerSlider.h
    Created: 6 Jun 2022 5:51:25pm
    Author:  Ivan Khulup

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class ADSRVisualizerSlider: public juce::Slider
{
public:
    ADSRVisualizerSlider();
    
    void mouseEnter(const juce::MouseEvent& event) override;
    void mouseExit(const juce::MouseEvent& event) override;
};
