/*
  ==============================================================================

    OscVoice.cpp
    Created: 24 May 2022 7:42:21pm
    Author:  Ivan Khulup

  ==============================================================================
*/

#include "OscVoice.h"

using namespace juce;

OscVoice::OscVoice()
{
    osc.initialise([](float x) { return std::sin(x); });
    adsr.setParameters(adsrParameters);
}

bool OscVoice::canPlaySound(SynthesiserSound *sound)
{
    return dynamic_cast<SynthesiserSound*>(sound) != nullptr;
}

void OscVoice::pitchWheelMoved(int newPitchWheelPosition)
{
    std::cout << "pitchWheelMoved: " << newPitchWheelPosition << std::endl;
}

void OscVoice::startNote(int midiNoteNumber, float noteVelocity, SynthesiserSound *sound, int currentPitchWheelPosition)
{
    std::cout << "start note " << juce::MidiMessage::getMidiNoteName(midiNoteNumber, true, true, 1) << std::endl;
    adsr.noteOn();
    osc.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
}

void OscVoice::stopNote(float noteVelocity, bool allowTailOff)
{
    adsr.noteOff();
    clearCurrentNote();
}

void OscVoice::renderNextBlock(AudioBuffer<float> &outputBuffer, int startSample, int numSamples)
{
    voiceBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
    voiceBuffer.clear();
    auto block = juce::dsp::AudioBlock<float> { voiceBuffer };
    osc.process(juce::dsp::ProcessContextReplacing<float>(block));
    adsr.applyEnvelopeToBuffer(voiceBuffer, 0, voiceBuffer.getNumSamples());
    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
    {
        outputBuffer.addFrom (channel, startSample, voiceBuffer, channel, 0, numSamples);
    }
}

void OscVoice::controllerMoved(int controllerNumber, int newControllerValue)
{
    std::cout << "controllerMoved: " << controllerNumber << " & " << newControllerValue << std::endl;
}

void OscVoice::updateWaveForm(WaveForms waveform)
{
    osc.reset();
    switch (waveform) {
        case WaveForms::sin:
            osc.initialise([] (float x ) { return sin(x);});
            break;
        case WaveForms::square:
            osc.initialise([] (float x ) { return x < 0.0f ? -1.0f : 1.0f; });
            break;
        case WaveForms::sawtooth:
            osc.initialise([] (float x ) { return x / juce::MathConstants<float>::pi; });
            break;
        case WaveForms::triangle:
            osc.initialise([] (float x){
                return (fabs(x / MathConstants<float>::pi) * -1) + 0.5f;
            });
            break;
        default:
            break;
    }
}

void OscVoice::initFilters(double sampleRate, int samplesPerBlock, int numChannels)
{
    initAdsrSpec(sampleRate);
    initOscilliatorSpec(sampleRate, samplesPerBlock, numChannels);
}


void OscVoice::updateAdsrParameters(float attack, float decay, float sustain, float release)
{
    adsrParameters.attack = attack;
    adsrParameters.decay = decay;
    adsrParameters.sustain = sustain;
    adsrParameters.release = release;
    adsr.setParameters(adsrParameters);
}
