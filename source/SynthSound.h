/*
  ==============================================================================

    synth_sound.h
    Created: 26 Jan 2021 10:45:47am
    Author:  Daniel Toby

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SynthSound : public juce::SynthesiserSound
{
public:
    bool appliesToNote(int midiNoteNumber) override { return true; }
    bool appliesToChannel(int midiChannel) override { return true; }
};
