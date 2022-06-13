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

class MainLayoutDrawer : juce::Button::Listener {
private:
    void drawBackground(juce::Graphics& graphicContext);
    void drawWaveSelectionContainer(juce::Graphics& graphicContext);
    void drawWaveformContainer(juce::Graphics& graphicContext);
    void drawWaveform(juce::Graphics& graphicContext, juce::Rectangle<float> drawingRect);
    void drawADSRComponent(juce::Graphics& graphicContext);
    void buttonClicked(juce::Button* clickedButton) override;
    
    int width { 0 };
    int height { 0 };
    
    std::vector<juce::Component*> components;
    
    juce::AudioVisualiserComponent audioVisualiserComponent;
    juce::ToggleButton sinWaveButton;
    juce::ToggleButton squareWaveButton;
    juce::ToggleButton sawToothWaveButton;
    juce::ToggleButton triangleWaveButton;
    ADSRVisualiserComponent adsrVisualiserComponent;
    
    juce::Button* currentSelection = nullptr;
    GUI::UIEventsListener* uiEventsListener = nullptr;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainLayoutDrawer);
    
public:
    MainLayoutDrawer(int numChannels);
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
