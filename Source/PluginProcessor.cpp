/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NewProjectAudioProcessor::NewProjectAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "Parameters", createApvtsParameters())
#endif
{
}

NewProjectAudioProcessor::~NewProjectAudioProcessor()
{
}

//==============================================================================
const juce::String NewProjectAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool NewProjectAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool NewProjectAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool NewProjectAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double NewProjectAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int NewProjectAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int NewProjectAudioProcessor::getCurrentProgram()
{
    return 0;
}

void NewProjectAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String NewProjectAudioProcessor::getProgramName (int index)
{
    return {};
}

void NewProjectAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void NewProjectAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    mySynthesiser.setCurrentPlaybackSampleRate(sampleRate);
    mySynthesiser.clearVoices();
    for (int i = 0; i < 1; i++) {
        auto voice = new OscVoice();
        voice->initFilters(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
        voice->updateWaveForm(mSelectedWaveform);
        mySynthesiser.addVoice(voice);
    }
    mySynthesiser.addSound(new OscSound());
}

void NewProjectAudioProcessor::releaseResources()
{
    mySynthesiser.clearVoices();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool NewProjectAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void NewProjectAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    auto attackParam = apvts.getRawParameterValue(Parameters::Ids::envelope_attack);
    auto decayParam = apvts.getRawParameterValue(Parameters::Ids::envelope_decay);
    auto sustainParam = apvts.getRawParameterValue(Parameters::Ids::envelope_sustain);
    auto releaseParam = apvts.getRawParameterValue(Parameters::Ids::envelope_release);
    
    for (int i = 0; i < mySynthesiser.getNumVoices(); ++i)
    {
        auto voice = dynamic_cast<OscVoice*>(mySynthesiser.getVoice(i));
        if (voice) {
            voice->updateADSRParameters(attackParam->load(), decayParam->load(), sustainParam->load(),
                                        releaseParam->load());
        }
        
    }
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    mySynthesiser.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    
    latestBlock.setSize(buffer.getNumChannels(), buffer.getNumSamples());
    latestBlock = juce::AudioBuffer<float>(buffer);
}

//==============================================================================
bool NewProjectAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* NewProjectAudioProcessor::createEditor()
{
    return new NewProjectAudioProcessorEditor (*this, apvts);
}

//==============================================================================
void NewProjectAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    xml -> setAttribute("waveForm", static_cast<int>(mSelectedWaveform));
    copyXmlToBinary(*xml, destData);
}

void NewProjectAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml (getXmlFromBinary(data, sizeInBytes));
    if (xml != nullptr) {
        if (xml->hasTagName(apvts.state.getType())) {
            apvts.replaceState(ValueTree::fromXml(*xml));
        }
    }
    mSelectedWaveform = static_cast<WaveForms>(xml->getIntAttribute("waveForm", -999));
    updateWaveForm(mSelectedWaveform);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NewProjectAudioProcessor();
}

juce::AudioBuffer<float>& NewProjectAudioProcessor::getLatestAudioBlock()
{
    return latestBlock;
}

void NewProjectAudioProcessor::updateWaveForm(WaveForms waveform)
{
    mSelectedWaveform = waveform;
    for (int i = 0; i < mySynthesiser.getNumVoices(); ++i)
    {
        auto voice = dynamic_cast<OscVoice*>(mySynthesiser.getVoice(i));
        if (voice)
        {
            voice->updateWaveForm(waveform);
        }
    }
}

APVTS_Parameters NewProjectAudioProcessor::createApvtsParameters()
{
    auto layout = APVTS_Parameters {};
    layout.add(std::make_unique<AudioParameterFloat>(Parameters::Ids::envelope_attack, Parameters::Names::envelope_attack, 0.0f, 5.0f, 0.1f));
    layout.add(std::make_unique<AudioParameterFloat>(Parameters::Ids::envelope_decay, Parameters::Names::envelope_decay, 0.0f, 5.0f, 0.1f));
    layout.add(std::make_unique<AudioParameterFloat>(Parameters::Ids::envelope_sustain, Parameters::Names::envelope_sustain, 0.0f, 5.0f, 0.1f));
    layout.add(std::make_unique<AudioParameterFloat>(Parameters::Ids::envelope_release, Parameters::Names::envelope_release, 0.0f, 5.0f, 0.1f));
    return layout;
}

void NewProjectAudioProcessor::handleIncomingMidiMessage(juce::MidiInput *source, const MidiMessage &message) {
    std::cout << "input from " << source->getName() << ", message: " << message.getDescription();
}

juce::StringArray NewProjectAudioProcessor::getAvailableMidiDevicesNames() {
    auto devices = juce::MidiInput::getAvailableDevices();
    juce::StringArray names;
    names.add("On-Screen keyboard");
    for (const auto& device : devices) {
        names.add(device.name);
    }
    return names;
}

void NewProjectAudioProcessor::setActiveMidiDeviceId(int deviceIndex) {
    auto deviceList = juce::MidiInput::getAvailableDevices();
    if (mSelectedMidiDeviceIdentifier.isNotEmpty()){
        mAudioDeviceManager.removeMidiInputDeviceCallback(mSelectedMidiDeviceIdentifier, this);
    }
    mSelectedMidiDeviceIdentifier = deviceList[deviceIndex].identifier;
    if (!mAudioDeviceManager.isMidiInputDeviceEnabled(mSelectedMidiDeviceIdentifier)) {
        mAudioDeviceManager.setMidiInputDeviceEnabled(mSelectedMidiDeviceIdentifier, true);
    }
    mAudioDeviceManager.addMidiInputDeviceCallback(mSelectedMidiDeviceIdentifier, this);
}

WaveForms NewProjectAudioProcessor::getCurrentSelectedWaveform() {
    return mSelectedWaveform;
}
