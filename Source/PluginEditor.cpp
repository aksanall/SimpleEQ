/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleEQAudioProcessorEditor::SimpleEQAudioProcessorEditor (SimpleEQAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p),
      lowCutFreqSliderAttachment(audioProcessor.apvts, "LowCut Freq", lowCutFreqSlider),
      highCutFreqSliderAttachment(audioProcessor.apvts, "HighCut Freq", highCutFreqSlider),
      peakFreqSliderAttachment(audioProcessor.apvts, "Peak Freq", peakFreqSlider),
      peakGainSliderAttachment(audioProcessor.apvts, "Peak Gain", peakGainSlider),
	  peakQualitySliderAttachment(audioProcessor.apvts, "Peak Quality", peakQualitySlider),
	  highCutSlopeSliderAttachment(audioProcessor.apvts, "HighCut Slope", highCutSlopeSlider),
	  lowCutSlopeSliderAttachment(audioProcessor.apvts, "LowCut Slope", lowCutSlopeSlider)
{

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    for(auto* comp : getComps())
    {
        addAndMakeVisible(comp);
	}

    setSize (600, 400);
}

SimpleEQAudioProcessorEditor::~SimpleEQAudioProcessorEditor()
{
}

//==============================================================================
void SimpleEQAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (15.0f));
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void SimpleEQAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    auto bounds = getLocalBounds().reduced(20);

    // Top third for response curve
    auto responseArea = bounds.removeFromTop(bounds.getHeight() / 3);

    // Split remaining area into 3 equal columns
    auto columnWidth = bounds.getWidth();

    auto lowCutArea = bounds.removeFromLeft(columnWidth/3);
    auto highCutArea = bounds.removeFromRight(columnWidth/2);

   /* auto peakArea = bounds.removeFromLeft(columnWidth/3);*/

    // Low‑cut column
    lowCutFreqSlider.setBounds(lowCutArea.removeFromTop(lowCutArea.getHeight() / 2));
    lowCutSlopeSlider.setBounds(lowCutArea);

    // High‑cut column
    highCutFreqSlider.setBounds(highCutArea.removeFromTop(highCutArea.getHeight() / 2));
    highCutSlopeSlider.setBounds(highCutArea);

    // Peak column
    peakFreqSlider.setBounds(bounds.removeFromTop(bounds.getHeight() / 3));
    peakGainSlider.setBounds(bounds.removeFromTop(bounds.getHeight() / 2));
    peakQualitySlider.setBounds(bounds);



}

std::vector<juce::Component*> SimpleEQAudioProcessorEditor::getComps()
{
    return { 
        &lowCutFreqSlider, 
        &highCutFreqSlider, 
        &peakFreqSlider, 
        &peakGainSlider, 
        &peakQualitySlider,
		&lowCutSlopeSlider,
		&highCutSlopeSlider
    };
}
