/*
  ==============================================================================

    ADSRComponent.cpp
    Created: 30 May 2022 10:11:32pm
    Author:  Ivan Khulup

  ==============================================================================
*/

#include "ADSRVisualiserComponent.h"

ADSRVisualiserComponent::ADSRVisualiserComponent()
{
    attack_slider.setRange(0.1, 5.0);
    attack_slider.setValue(attack_value);
    attack_slider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    attack_slider.setLookAndFeel(new ADSRVisualizerSliderLaF());
    
    decay_slider.setValue(decay_value);
    decay_slider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    
    addAndMakeVisible(&attack_slider);
    addAndMakeVisible(&decay_slider);
    addAndMakeVisible(&sustain_slider);
    addAndMakeVisible(&release_slider);
}

void ADSRVisualiserComponent::parameterChanged(const String &parameterID, float newValue)
{
    std::cout << "parameter " << parameterID << " changed to " << newValue;
}

void ADSRVisualiserComponent::resized()
{
    attack_slider.setBounds(getLocalBounds().getWidth() * 0.1f, 0, 20, getLocalBounds().getHeight());
    decay_slider.setBounds(getLocalBounds().getWidth() * 0.35f, 0, 20, getLocalBounds().getHeight());
    sustain_slider.setBounds(getLocalBounds().getWidth() * 0.6f, 0, 20, getLocalBounds().getHeight());
    release_slider.setBounds(getLocalBounds().getWidth() * 0.9f, 0, 20, getLocalBounds().getHeight());
}

void ADSRVisualiserComponent::paint(Graphics &graphics)
{
    auto bounds = getLocalBounds().toType<float>();
    graphics.setColour(Colours::dimgrey.darker(0.3f));
    graphics.fillRoundedRectangle(bounds, 3.0f);
    
    graphics.setColour(Colours::aquamarine.withAlpha(0.2f));
    for (int i = 1; i <= 10; ++i) {
        auto y = bounds.getHeight() * (0.1f * i);
        graphics.drawLine(0, y , bounds.getWidth(), y);
        
        auto x = bounds.getWidth() * (0.1f * i);
        graphics.drawLine(x, 0, x, bounds.getHeight());
    }
    drawADSRPoints(graphics);
}

void ADSRVisualiserComponent::drawADSRPoints(Graphics& graphics)
{
}
