/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "MainLayoutDrawer.h"
#include "SynthParameters.h"

//==============================================================================
/**
*/
class NewProjectAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Timer, public GUI::UIEventsListener
{
public:
    explicit NewProjectAudioProcessorEditor (NewProjectAudioProcessor&, juce::AudioProcessorValueTreeState&);
    ~NewProjectAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;

    void onWaveFormSelected(WaveForms form) override;
    void onMidiDeviceSelected(int midiDeviceId) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    NewProjectAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessorEditor)

    MainLayoutDrawer drawingDelegate;
};
