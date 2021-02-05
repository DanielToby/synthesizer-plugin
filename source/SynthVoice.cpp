/*
  ==============================================================================

    synth_voice.cpp
    Created: 26 Jan 2021 10:45:34am
    Author:  Daniel Toby

  ==============================================================================
*/

#include "SynthVoice.h"

bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    _osc.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
    _adsr.noteOn();
}

void SynthVoice::stopNote(float velocity, bool allowTailOff)
{
    _adsr.noteOff();
    
    if (!allowTailOff || _adsr.isActive())
        clearCurrentNote();
}

void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue)
{
    
}

void SynthVoice::pitchWheelMoved(int newPitchWheelValue)
{
    
}

void SynthVoice::prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels)
{
    _adsr.setSampleRate(sampleRate);
    
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;
    
    _osc.prepare(spec);
    _gain.prepare(spec);
    
    _gain.setGainLinear(0.1f);
    
    _adsrParams.attack = 0.8f;
    _adsrParams.decay = 0.8f;
    _adsrParams.sustain = 1.0f;
    _adsrParams.release = 1.5f;
    
    _adsr.setParameters(_adsrParams);
    
    isPrepared = true;
}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples)
{
    jassert(isPrepared);
    
    if (!isVoiceActive())
        return;
    
    _synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
    _synthBuffer.clear();
    
    juce::dsp::AudioBlock<float> audioBlock { _synthBuffer };
    _osc.process(juce::dsp::ProcessContextReplacing<float> (audioBlock));
    _gain.process(juce::dsp::ProcessContextReplacing<float> (audioBlock));
    
    _adsr.applyEnvelopeToBuffer(_synthBuffer, 0, _synthBuffer.getNumSamples());
    
    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
    {
        outputBuffer.addFrom(channel, startSample, _synthBuffer, channel, 0, numSamples);
        
        if (!_adsr.isActive())
            clearCurrentNote();
    }
}
