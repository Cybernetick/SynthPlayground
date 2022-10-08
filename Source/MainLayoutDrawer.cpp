/*
  ==============================================================================

    MainLayoutDrawer.cpp
    Created: 9 May 2022 9:39:50pm
    Author:  Ivan Khulup

  ==============================================================================
*/

#include "MainLayoutDrawer.h"

MainLayoutDrawer::MainLayoutDrawer(int numChannels): audioVisualiserComponent(numChannels) {
    audioVisualiserComponent.setColours(juce::Colours::darkblue, juce::Colours::aquamarine);
    audioVisualiserComponent.setRepaintRate(24);
    audioVisualiserComponent.setSamplesPerBlock(16);
    components.insert(components.begin(), &audioVisualiserComponent);
    
    auto sinWaveButton = new juce::ToggleButton();
    sinWaveButton->setLookAndFeel(sinWaveButtonLaF.get());
    sinWaveButton->addListener(this);
    auto squareWaveButton = new juce::ToggleButton();
    squareWaveButton->setLookAndFeel(squareWaveButtonLaF.get());
    squareWaveButton->addListener(this);
    auto sawToothWaveButton = new juce::ToggleButton();
    sawToothWaveButton->setLookAndFeel(sawtoothWaveButtonLaF.get());
    sawToothWaveButton->addListener(this);
    auto triangleWaveButton = new juce::ToggleButton();
    triangleWaveButton->setLookAndFeel(triangleWaveButtonLaF.get());
    triangleWaveButton->addListener(this);
    
    components.insert(components.end(), sinWaveButton);
    components.insert(components.end(), squareWaveButton);
    components.insert(components.end(), sawToothWaveButton);
    components.insert(components.end(), triangleWaveButton);
    components.insert(components.end(), &adsrVisualiserComponent);
}

void MainLayoutDrawer::redrawLayout(juce::Graphics &graphic)
{
    drawBackground(graphic);
    drawWaveSelectionContainer(graphic);
    drawWaveformContainer(graphic);
    drawADSRComponent(graphic);
}

void MainLayoutDrawer::onWindowResized(int newWidth, int newHeight)
{
    width = newWidth;
    height = newHeight;
}

void MainLayoutDrawer::pushBufferForVisualisation(juce::AudioBuffer<float>& buffer)
{
    audioVisualiserComponent.pushBuffer(buffer);
}

// Private functions ==========================================================================================================================================================================================================
void MainLayoutDrawer::drawBackground(juce::Graphics &graphicContext)
{
    auto gradient = juce::ColourGradient(juce::Colours::black, 0.0, 0.0, juce::Colours::darkblue, (float) width, (float) height, true);
    graphicContext.setGradientFill(gradient);
    graphicContext.fillAll();
}

void MainLayoutDrawer::drawWaveformContainer(juce::Graphics &graphicContext)
{
    float waveformAreaHeight = height * 0.5;
    audioVisualiserComponent.setBounds(0, waveformAreaHeight, width, height);
//    graphicContext.setColour(juce::Colours::black);
    auto rect = juce::Rectangle<float>(0.0, waveformAreaHeight, width, height);
    drawWaveform(graphicContext, rect);
}

void MainLayoutDrawer::drawWaveform(juce::Graphics &graphicContext, juce::Rectangle<float> drawingRect)
{
    audioVisualiserComponent.paintChannel(graphicContext, drawingRect, new juce::Range<float>(-1.0, 1.0), 2, 16);
}

void MainLayoutDrawer::drawWaveSelectionContainer(juce::Graphics &graphicContext)
{
//    sinWaveButton.setBounds(10, 10, 40, 40);
//    squareWaveButton.setBounds(50, 10, 40, 40);
//    sawToothWaveButton.setBounds(90, 10, 40, 40);
//    triangleWaveButton.setBounds(130, 10, 40, 40);
    for (Component* component : components) {
        auto laf = dynamic_cast<WaveSelectionButtonLaF*>(&component->getLookAndFeel());
        if (laf) {
            if (laf->waveform == WaveForms::sin) {
                component->setBounds(10, 10, 40, 40);
            } else if (laf->waveform == WaveForms::square) {
                component->setBounds(50, 10, 40, 40);
            } else if (laf->waveform == WaveForms::sawtooth) {
                component->setBounds(90, 10, 40, 40);
            } else if (laf->waveform == WaveForms::triangle) {
                component->setBounds(130, 10, 40, 40);
            }
        }
    }
}

void MainLayoutDrawer::buttonClicked(juce::Button *clickedButton)
{
    auto current = dynamic_cast<juce::ToggleButton*>(currentSelection);
    if (current) {
        current->setToggleState(false, juce::NotificationType::dontSendNotification);
    }
    currentSelection = clickedButton;
    currentSelection->setToggleState(true, juce::NotificationType::dontSendNotification);
    if (uiEventsListener)
    {
        auto currentLaF = dynamic_cast<WaveSelectionButtonLaF*>(&currentSelection->getLookAndFeel());
        if (currentLaF)
        {
            uiEventsListener->onWaveFormSelected(currentLaF->waveform);
        }
    }
}

void MainLayoutDrawer::clean()
{
    components.clear();
    uiEventsListener = nullptr;
}

void MainLayoutDrawer::drawADSRComponent(juce::Graphics &graphicContext)
{
    adsrVisualiserComponent.setBounds(200, 10, 200, 100);
}
