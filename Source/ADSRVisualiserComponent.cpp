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
    attack_slider.setValue(attack_value);
    decay_slider.setValue(decay_value);
    sustain_slider.setValue(sustain_value);
    release_slider.setValue(release_value);
    
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
    graphics.setColour(Colours::darkblue.darker(0.3f));
    graphics.fillRoundedRectangle(bounds, 3.0f);
    drawVisualizerPath(graphics);
}

void ADSRVisualiserComponent::drawVisualizerPath(Graphics& graphics)
{
    auto bounds = getLocalBounds();
    envelopeVisualizerPath.clear();
    envelopeVisualizerPath.startNewSubPath(0.0, bounds.getHeight());
    auto attackPosition = attack_slider.getCurrentThumbPosition();
    envelopeVisualizerPath.lineTo(attack_slider.getBounds().getCentreX(), attackPosition);
    graphics.setColour(attack_slider.findColour(juce::Slider::thumbColourId));
    graphics.strokePath(envelopeVisualizerPath, PathStrokeType(1.0f));
    
    auto decay = decay_slider.getCurrentThumbPosition();
    envelopeVisualizerPath.lineTo(decay_slider.getBounds().getCentreX(), decay);
    auto sustain = sustain_slider.getCurrentThumbPosition();
    envelopeVisualizerPath.lineTo(sustain_slider.getBounds().getCentreX(), sustain);
    auto release = release_slider.getCurrentThumbPosition();
    envelopeVisualizerPath.lineTo(release_slider.getBounds().getCentreX(), release);
    envelopeVisualizerPath.lineTo(bounds.getWidth(), bounds.getHeight());
    
    graphics.strokePath(envelopeVisualizerPath, PathStrokeType(1.0f));
}

void ADSRVisualiserComponent::clean() {
    attack_slider.setLookAndFeel(nullptr);
    decay_slider.setLookAndFeel(nullptr);
    sustain_slider.setLookAndFeel(nullptr);
    release_slider.setLookAndFeel(nullptr);
}

void ADSRVisualiserComponent::attachAPVTS(juce::AudioProcessorValueTreeState& apvts)
{
    attack_slider.attachApvts(apvts, Parameters::Ids::envelope_attack);
    decay_slider.attachApvts(apvts, Parameters::Ids::envelope_decay);
    sustain_slider.attachApvts(apvts, Parameters::Ids::envelope_sustain);
    release_slider.attachApvts(apvts, Parameters::Ids::envelope_release);
}
