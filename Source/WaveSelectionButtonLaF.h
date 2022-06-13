/*
  ==============================================================================

    WaveSelectionButtonLaF.h
    Created: 12 May 2022 10:38:57pm
    Author:  Ivan Khulup

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "Waveforms.h"

using namespace juce;
struct WaveSelectionButtonLaF: public juce::LookAndFeel_V4 {
    
public:
    WaveSelectionButtonLaF(WaveForms form) : waveform { form }
    {
    };
    
    const WaveForms waveform;
    
    void drawToggleButton (Graphics& graphics, ToggleButton& button,
                                   bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        
        auto defaultColor = juce::Colours::darkblue;
        auto baseColour = defaultColor.withMultipliedSaturation (button.hasKeyboardFocus (true) ? 1.3f : 0.9f)
                                                  .withMultipliedAlpha      (button.isEnabled() ? 1.0f : 0.5f);
        
        if (shouldDrawButtonAsDown || shouldDrawButtonAsHighlighted)
            baseColour = baseColour.darker (shouldDrawButtonAsDown ? 0.3f : 0.0f);
        auto inset = 16.0f;
        
        auto width  = (float) button.getWidth()  - inset;
        auto height = (float) button.getHeight() - inset;
        
        if (width > 0 && height > 0)
        {
        
            shadow.colour = Colours::dimgrey;
            if (shouldDrawButtonAsDown || button.getToggleState())
            {
                shadow.radius = inset / 3;
            } else
            {
                shadow.radius = inset / 2;
            }
            
            shadow.drawForRectangle(graphics, button.getLocalBounds().reduced(shadow.radius, shadow.radius));
            graphics.setGradientFill (ColourGradient::vertical (baseColour, 0.0f,
                                                         baseColour.darker (0.1f), height));
            
            Rectangle<float> buttonBody = button.getLocalBounds().reduced(inset / 2, inset / 2).toType<float>();
            graphics.fillRoundedRectangle(buttonBody, 4.0f);
            buttonBody = button.getLocalBounds().reduced(inset / 2, inset * 0.85).toType<float>();
            drawWaveFormIndicator(graphics, buttonBody);
        }
    }
    
private:
    DropShadow shadow { DropShadow() };
    Path indicatorPath { Path() };
    
    bool isPathConstructed { false };
    
    void drawWaveFormIndicator(Graphics& graphics, Rectangle<float> bounds)
    {
        graphics.setColour(Colours::aquamarine);
        indicatorPath.clear();
        
        switch (waveform) {
            case WaveForms::sin:
                for (int i = 0; i <= bounds.getWidth() ; ++i)
                {
                    auto x = MathConstants<float>::twoPi * (i / (bounds.getWidth()));
                    auto y = std::abs((std::sin(x) * (bounds.getHeight() / 2)) + (bounds.getHeight() / 2 + bounds.getY()));
                    if (indicatorPath.isEmpty())
                    {
                        indicatorPath.startNewSubPath(i + bounds.getX(), y);
                    }
                    indicatorPath.lineTo(i + bounds.getX(), y);
                    
                };
                break;
            case WaveForms::square:
                indicatorPath.startNewSubPath(bounds.getX(), bounds.getY() + bounds.getHeight() / 6);
                indicatorPath.lineTo(bounds.getX() + bounds.getWidth() / 2, bounds.getY() + bounds.getHeight() / 6);
                indicatorPath.lineTo(bounds.getX() + bounds.getWidth() / 2, bounds.getY() + bounds.getHeight() * 0.75);
                indicatorPath.lineTo(bounds.getX() + bounds.getWidth(), bounds.getY() + bounds.getHeight() * 0.75);
                break;
            case WaveForms::sawtooth:
                indicatorPath.startNewSubPath(bounds.getX(), bounds.getY() + bounds.getHeight() * 0.9);
                indicatorPath.lineTo(bounds.getX() + bounds.getWidth() / 2, bounds.getY());
                indicatorPath.lineTo(bounds.getX() + bounds.getWidth() / 2, bounds.getY() + bounds.getHeight() * 0.9);
                indicatorPath.lineTo(bounds.getX() + bounds.getWidth(), bounds.getHeight());
                break;
            case WaveForms::triangle:
                indicatorPath.startNewSubPath(bounds.getX(), bounds.getY() + bounds.getHeight() * 0.9);
                indicatorPath.lineTo(bounds.getX() + bounds.getWidth() / 3, bounds.getY());
                indicatorPath.lineTo(bounds.getX() + bounds.getWidth() * 0.66, bounds.getY() + bounds.getHeight() * 0.9);
                indicatorPath.lineTo(bounds.getX() + bounds.getWidth(), bounds.getY());
                break;
            default:
                break;
        }
        if (!isPathConstructed)
        {
            isPathConstructed = true;
        }
        graphics.strokePath(indicatorPath, PathStrokeType(1.0f));
    }
};
