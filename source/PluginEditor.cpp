/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynthframeworkAudioProcessorEditor::SynthframeworkAudioProcessorEditor (SynthframeworkAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    _attackSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    _attackSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 80, 50);
    addAndMakeVisible(_attackSlider);
    
    _decaySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    _decaySlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 80, 50);
    addAndMakeVisible(_decaySlider);
    
    _sustainSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    _sustainSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 80, 50);
    addAndMakeVisible(_sustainSlider);
    
    _releaseSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    _releaseSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 80, 50);
    addAndMakeVisible(_releaseSlider);
    
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    
    _attackAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "ATTACK", _attackSlider);
    _decayAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "DECAY", _decaySlider);
    _sustainAttahment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "SUSTAIN", _sustainSlider);
    _releaseAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "RELEASE", _releaseSlider);
    
    _oscSelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, "OSC", _oscSelector);
}

SynthframeworkAudioProcessorEditor::~SynthframeworkAudioProcessorEditor()
{
}

//==============================================================================
void SynthframeworkAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
}

void SynthframeworkAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    _attackSlider.setBounds(0, 50, 80, 50);
    _decaySlider.setBounds(80, 50, 80, 50);
    _sustainSlider.setBounds(160, 50, 80, 50);
    _releaseSlider.setBounds(240, 50, 80, 50);
}
