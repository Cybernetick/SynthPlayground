/*
  ==============================================================================

    SynthParameters.h
    Created: 29 May 2022 1:52:15pm
    Author:  Ivan Khulup

  ==============================================================================
*/

#pragma once

namespace Parameters
{
    namespace Ids
    {
        static const juce::String envelope_attack { "envelope_attack" };
        static const juce::String envelope_decay { "envelope_decay" };
        static const juce::String envelope_sustain {"envelope_sustain"};
        static const juce::String envelope_release {"envelope_release"};
    };

    namespace Names
    {
        static const juce::String envelope_attack { "Attack" };
        static const juce::String envelope_decay { "Decay" };
        static const juce::String envelope_sustain { "Sustain" };
        static const juce::String envelope_release { "Release" };
    };
}

