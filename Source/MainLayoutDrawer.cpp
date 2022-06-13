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
    
    sinWaveButton.setLookAndFeel(new WaveSelectionButtonLaF(WaveForms::sin));
    sinWaveButton.addListener(this);
    squareWaveButton.setLookAndFeel(new WaveSelectionButtonLaF(WaveForms::square));
    squareWaveButton.addListener(this);
    
    sawToothWaveButton.setLookAndFeel(new WaveSelectionButtonLaF(WaveForms::sawtooth));
    sawToothWaveButton.addListener(this);
    
    triangleWaveButton.setLookAndFeel(new WaveSelectionButtonLaF(WaveForms::triangle));
    triangleWaveButton.addListener(this);
    
    components.insert(components.end(), &sinWaveButton);
    components.insert(components.end(), &squareWaveButton);
    components.insert(components.end(), &sawToothWaveButton);
    components.insert(components.end(), &triangleWaveButton);
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
    sinWaveButton.setBounds(10, 10, 40, 40);
    squareWaveButton.setBounds(50, 10, 40, 40);
    sawToothWaveButton.setBounds(90, 10, 40, 40);
    triangleWaveButton.setBounds(130, 10, 40, 40);
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
