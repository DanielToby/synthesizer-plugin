/*
  ==============================================================================

    synth_voice.h
    Created: 26 Jan 2021 10:45:34am
    Author:  Daniel Toby

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "SynthSound.h"

class SynthVoice : public juce::SynthesiserVoice
{
public:
    bool canPlaySound(juce::SynthesiserSound* sound) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;
    void pitchWheelMoved(int newPitchWheelValue) override;
    void prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels);
    void renderNextBlock(juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples) override;

private:
    juce::ADSR _adsr;
    juce::ADSR::Parameters _adsrParams;
    juce::AudioBuffer<float> _synthBuffer;
    
    juce::dsp::Oscillator<float> _osc { [] (float x) { return std::sin (x); }};
    juce::dsp::Gain<float> _gain;
    bool isPrepared { false };
};
