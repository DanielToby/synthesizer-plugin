/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "SynthVoice.h"

//==============================================================================
SynthframeworkAudioProcessor::SynthframeworkAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "Parameters", createParams())
#endif
{
    _synth.addSound(new SynthSound());
    _synth.addVoice(new SynthVoice());
}

SynthframeworkAudioProcessor::~SynthframeworkAudioProcessor()
{
}

//==============================================================================
const juce::String SynthframeworkAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SynthframeworkAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SynthframeworkAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SynthframeworkAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SynthframeworkAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SynthframeworkAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SynthframeworkAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SynthframeworkAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SynthframeworkAudioProcessor::getProgramName (int index)
{
    return {};
}

void SynthframeworkAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SynthframeworkAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    _synth.setCurrentPlaybackSampleRate(sampleRate);
    
    for (int i = 0; i < _synth.getNumVoices(); i++)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(_synth.getVoice(i)))
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
    }
}

void SynthframeworkAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SynthframeworkAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void SynthframeworkAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int i = 0; i < _synth.getNumVoices(); ++i)
    {
        if (auto voice = dynamic_cast<juce::SynthesiserVoice*>(_synth.getVoice(i)))
        {
            // Osc controls
            // ADSR
            // LFO
        }
    }
    
    _synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool SynthframeworkAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SynthframeworkAudioProcessor::createEditor()
{
    return new SynthframeworkAudioProcessorEditor (*this);
}

//==============================================================================
void SynthframeworkAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SynthframeworkAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SynthframeworkAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout SynthframeworkAudioProcessor::createParams()
{
    // ComboBox: switch oscillators
    // Attack - float
    // Decay - float
    // Sustain - float
    // Release - float
    
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> _params;
    
    _params.push_back(std::make_unique<juce::AudioParameterChoice> ("OSC", "Oscillator", juce::StringArray{ "Sine", "Square" }, 0));
    
    _params.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", juce::NormalisableRange<float> { 0.1f, 1.0f, }, 0.1f));
    _params.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", juce::NormalisableRange<float> { 0.1f, 1.0f, }, 0.1f));
    _params.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "Sustain", juce::NormalisableRange<float> { 0.1f, 1.0f, }, 1.0f));
    _params.push_back(std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", juce::NormalisableRange<float> { 0.1f, 3.0f, }, 0.4f));
    
    
    return { _params.begin(), _params.end() };
}
