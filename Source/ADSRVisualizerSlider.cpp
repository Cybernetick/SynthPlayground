/*
  ==============================================================================

    ADSRVisualizerSlider.cpp
    Created: 6 Jun 2022 5:51:25pm
    Author:  Ivan Khulup

  ==============================================================================
*/

#include "ADSRVisualizerSlider.h"

ADSRVisualizerSlider::ADSRVisualizerSlider()
{
    setColour(juce::Slider::trackColourId, juce::Colours::green);
    setColour(juce::Slider::thumbColourId, juce::Colours::red);
    setSliderSnapsToMousePosition(false);
    onValueChange = [&](){
        std::cout << "value changed: " << getValue() << std::endl;
    };
}

void ADSRVisualizerSlider::mouseExit(const juce::MouseEvent &event)
{
}

void ADSRVisualizerSlider::mouseEnter(const juce::MouseEvent &event)
{
}

