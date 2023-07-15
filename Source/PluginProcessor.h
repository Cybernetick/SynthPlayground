/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "OscVoice.h"
#include "Waveforms.h"

//==============================================================================
/**
*/
using APVTS = juce::AudioProcessorValueTreeState;
using APVTS_Parameters = juce::AudioProcessorValueTreeState::ParameterLayout;

class NewProjectAudioProcessor  : public juce::AudioProcessor, private juce::MidiInputCallback
{
public:
    //==============================================================================
    NewProjectAudioProcessor();
    ~NewProjectAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    juce::AudioBuffer<float>& getLatestAudioBlock();
    void updateWaveForm(WaveForms waveform);
    WaveForms getCurrentSelectedWaveform();
    juce::StringArray getAvailableMidiDevicesNames();
    void setActiveMidiDeviceId(int deviceIndex);

    APVTS apvts;
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessor)

    juce::Synthesiser mySynthesiser;
    juce::AudioBuffer<float> latestBlock { juce::AudioBuffer<float>() };
    juce::AudioDeviceManager mAudioDeviceManager;
    String mSelectedMidiDeviceIdentifier;
    WaveForms mSelectedWaveform { WaveForms::sin };
    static APVTS_Parameters createApvtsParameters();

    void handleIncomingMidiMessage(juce::MidiInput *source, const juce::MidiMessage &message) override;
};
