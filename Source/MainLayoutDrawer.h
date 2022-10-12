/*
  ==============================================================================

    MainLayoutDrawer.h
    Created: 9 May 2022 9:39:50pm
    Author:  Ivan Khulup

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "WaveSelectionButtonLaF.h"
#include "UIEventsListener.h"
#include "ADSRVisualiserComponent.h"

using namespace juce;
class MainLayoutDrawer : Button::Listener {
private:
    void drawBackground(juce::Graphics& graphicContext);
    void drawWaveSelectionContainer(juce::Graphics& graphicContext);
    void drawWaveformContainer(juce::Graphics& graphicContext);
    void drawWaveform(juce::Graphics& graphicContext, juce::Rectangle<float> drawingRect);
    void drawADSRComponent(juce::Graphics& graphicContext);
    void buttonClicked(juce::Button* clickedButton) override;
    
    int width { 0 };
    int height { 0 };
    
    juce::AudioVisualiserComponent audioVisualiserComponent;
    std::unique_ptr<WaveSelectionButtonLaF> sinWaveButtonLaF { new WaveSelectionButtonLaF(WaveForms::sin) };
    std::unique_ptr<WaveSelectionButtonLaF> squareWaveButtonLaF { new WaveSelectionButtonLaF(WaveForms::square) };
    std::unique_ptr<WaveSelectionButtonLaF> sawtoothWaveButtonLaF { new WaveSelectionButtonLaF(WaveForms::sawtooth) };
    std::unique_ptr<WaveSelectionButtonLaF> triangleWaveButtonLaF { new WaveSelectionButtonLaF(WaveForms::triangle) };

    ToggleButton sinWaveButton;
    ToggleButton squareWaveButton;
    ToggleButton sawToothWaveButton;
    ToggleButton triangleWaveButton;

    ADSRVisualiserComponent adsrVisualiserComponent;

    std::vector<juce::Component*> components;

    juce::Button* currentSelection = nullptr;
    GUI::UIEventsListener* uiEventsListener = nullptr;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainLayoutDrawer)
    
public:
    explicit MainLayoutDrawer(int numChannels);
    void redrawLayout(juce::Graphics& graphic);
    void onWindowResized(int newWidth, int newHeight);
    void pushBufferForVisualisation(juce::AudioBuffer<float>& buffer);
    void clean();
    
    void setUIEventsListener(GUI::UIEventsListener* listener)
    {
        uiEventsListener = listener;
    }
    
    const std::vector<juce::Component*> getComponents()
    {
        return components;
    }
    
    juce::AudioProcessorValueTreeState::Listener* getAudioParametersListener()
    {
        return dynamic_cast<ADSRVisualiserComponent*>(&adsrVisualiserComponent);
    }
};
