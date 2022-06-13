/*
  ==============================================================================

    OscSound.h
    Created: 28 May 2022 9:32:26am
    Author:  Ivan Khulup

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class OscSound: public juce::SynthesiserSound {
public:
    bool appliesToNote(int midiNote) override { return true; }
    bool appliesToChannel (int midiChannel) override { return true; };
};
