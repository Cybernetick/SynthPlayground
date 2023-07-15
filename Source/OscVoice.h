/*
  ==============================================================================

    OscVoice.h
    Created: 24 May 2022 7:42:21pm
    Author:  Ivan Khulup

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Waveforms.h"
#include "OscSound.h"

class OscVoice: public juce::SynthesiserVoice {
public:
    OscVoice();
    bool canPlaySound(juce::SynthesiserSound* sound) override;
    void startNote(int midiNoteNumber, float noteVelocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float noteVelocity, bool allowTailOff) override;
    void pitchWheelMoved(int newPitchWheelPosition) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;
    void renderNextBlock(juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples) override;
    void initFilters(double sampleRate, int samplesPerBlock, int numChannels);
    void updateWaveForm(WaveForms waveform);
    void updateADSRParameters(float attack, float decay, float sustain, float release);
private:
    juce::dsp::Oscillator<float> osc {};
    juce::AudioBuffer<float> voiceBuffer;
    juce::ADSR adsr {};
    juce::ADSR::Parameters adsrParameters {};
    
    void initOscillatorSpec(double sampleRate, int samplesPerBlock, int numChannels)
    {
        juce::dsp::ProcessSpec spec;
        spec.maximumBlockSize = samplesPerBlock;
        spec.sampleRate = sampleRate;
        spec.numChannels = numChannels;
        
        osc.prepare(spec);
    }
    
    void initADSRSpec(double sampleRate)
    {
        adsr.setSampleRate(sampleRate);
    }

    float SawSample(float phase);
};
