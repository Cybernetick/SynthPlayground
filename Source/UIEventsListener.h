/*
  ==============================================================================

    UIEventsListener.h
    Created: 27 May 2022 1:30:32pm
    Author:  Ivan Khulup

  ==============================================================================
*/

#pragma once

namespace GUI {
    class UIEventsListener {
    public:
        virtual void onWaveFormSelected(WaveForms form) = 0;
        virtual ~UIEventsListener() = default;
    };
}
