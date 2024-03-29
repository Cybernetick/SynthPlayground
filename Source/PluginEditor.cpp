/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NewProjectAudioProcessorEditor::NewProjectAudioProcessorEditor (NewProjectAudioProcessor& p, juce::AudioProcessorValueTreeState& apvts)
    : AudioProcessorEditor (&p), audioProcessor (p), drawingDelegate(audioProcessor.getTotalNumOutputChannels())
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 400);
    setResizable(true, true);
    drawingDelegate.setUIEventsListener(this);
    
    for (juce::Component* component: drawingDelegate.getComponents())
    {
        addAndMakeVisible(component);
        auto adsrComponent = dynamic_cast<ADSRVisualiserComponent*>(component);
        if (adsrComponent) {
            adsrComponent->attachAPVTS(apvts);
        }
    }
    startTimerHz(60);

    audioProcessor.apvts.addParameterListener(Parameters::Ids::envelope_attack, drawingDelegate.getAudioParametersListener());
    drawingDelegate.appendMidiDevicesList(audioProcessor.getAvailableMidiDevicesNames(), *this);
    drawingDelegate.setWaveformButtonSelected(audioProcessor.getCurrentSelectedWaveform());
}

NewProjectAudioProcessorEditor::~NewProjectAudioProcessorEditor()
{
    stopTimer();
    drawingDelegate.clean();
}

//==============================================================================
void NewProjectAudioProcessorEditor::paint (juce::Graphics& g)
{
    drawingDelegate.redrawLayout(g);
}

void NewProjectAudioProcessorEditor::resized()
{
    drawingDelegate.onWindowResized(getLocalBounds().getWidth(), getLocalBounds().getHeight());
}

void NewProjectAudioProcessorEditor::timerCallback()
{
    drawingDelegate.pushBufferForVisualisation(audioProcessor.getLatestAudioBlock());
    repaint();
}

void NewProjectAudioProcessorEditor::onWaveFormSelected(WaveForms form)
{
    std::cout << "notified waveform change" << std::endl;
    audioProcessor.updateWaveForm(form);
}

void NewProjectAudioProcessorEditor::onMidiDeviceSelected(int midiDeviceId) {
    std::cout << "selected midi: "<< midiDeviceId;
}
