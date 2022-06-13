/*
  ==============================================================================

    ADSRComponent.h
    Created: 30 May 2022 10:11:32pm
    Author:  Ivan Khulup

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ADSRVisualizerSliderLaF.h"
#include "ADSRVisualizerSlider.h"

using namespace juce;

class ADSRVisualiserComponent: public juce::Component, public juce::AudioProcessorValueTreeState::Listener
{
    float attack_value { 0.1f };
    float decay_value { 2.0f };
    float sustain_value { 4.5f };
    float release_value { 1.1f };
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ADSRVisualiserComponent);
    
    ADSRVisualizerSlider attack_slider { };
    Slider decay_slider { };
    Slider sustain_slider { };
    Slider release_slider { };
    void drawADSRPoints(Graphics& graphics);
    
public:
    ADSRVisualiserComponent();
    void parameterChanged(const String& parameterID, float newValue) override;
    void resized() override;
    void paint(Graphics& graphics) override;
};
