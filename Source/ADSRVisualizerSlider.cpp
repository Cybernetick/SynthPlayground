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
    setColour(juce::Slider::thumbColourId, juce::Colours::aquamarine);
    setSliderSnapsToMousePosition(false);
    setLookAndFeel(new ADSRVisualizerSliderLaF());
    setSliderStyle(SliderStyle::LinearVertical);
    setRange(0.1, 5.0);
}

void ADSRVisualizerSlider::mouseExit(const juce::MouseEvent &event)
{
}

void ADSRVisualizerSlider::mouseEnter(const juce::MouseEvent &event)
{
}

float ADSRVisualizerSlider::getCurrentThumbPosition()
{
    auto position = getPositionOfValue(getValue());
    return position;
}

void ADSRVisualizerSlider::attachApvts(juce::AudioProcessorValueTreeState &apvts, const juce::String &parameterId)
{
    sliderAttachment = std::make_unique<SliderAttachement>(apvts, parameterId, *this);
}

