/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class SynthframeworkAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SynthframeworkAudioProcessorEditor (SynthframeworkAudioProcessor&);
    ~SynthframeworkAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Slider _attackSlider;
    juce::Slider _decaySlider;
    juce::Slider _sustainSlider;
    juce::Slider _releaseSlider;
    juce::ComboBox _oscSelector;
    
    SynthframeworkAudioProcessor& audioProcessor;
    
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    
    std::unique_ptr<SliderAttachment> _attackAttachment;
    std::unique_ptr<SliderAttachment> _decayAttachment;
    std::unique_ptr<SliderAttachment> _sustainAttahment;
    std::unique_ptr<SliderAttachment> _releaseAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> _oscSelAttachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthframeworkAudioProcessorEditor)
};
