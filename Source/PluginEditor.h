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
struct customRotarySlider : juce::Slider
{
    customRotarySlider() : juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, 
                                        juce::Slider::TextEntryBoxPosition::NoTextBox) 
    {
    }
};

class SimpleEQAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SimpleEQAudioProcessorEditor (SimpleEQAudioProcessor&);
    ~SimpleEQAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SimpleEQAudioProcessor& audioProcessor;

	customRotarySlider lowCutFreqSlider, 
                 highCutFreqSlider, 
                 peakFreqSlider, 
                 peakGainSlider, 
                 peakQualitySlider,
                 lowCutSlopeSlider,
                 highCutSlopeSlider;

	std::vector<juce::Component*> getComps();

	using APVTS = juce::AudioProcessorValueTreeState;
	using Attachment = APVTS::SliderAttachment;
    Attachment lowCutFreqSliderAttachment, 
               highCutFreqSliderAttachment, 
               peakFreqSliderAttachment, 
               peakGainSliderAttachment, 
               peakQualitySliderAttachment,
               lowCutSlopeSliderAttachment,
		       highCutSlopeSliderAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleEQAudioProcessorEditor)
};
