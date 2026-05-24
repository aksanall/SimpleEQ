/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
enum Slope
{
	slope_12,
	slope_24,
	slope_36,
	slope_48
};

struct ChainSettings
{
    float lowCutFreq{0}, highCutFreq{0}, peakFreq{0}, peakGainInDecibels{0}, peakQuality{ 1.f };
    int lowCutSlope{ 0 }, highCutSlope{ 0 };
	Slope lowCutSlopeEnum{ Slope::slope_12 }, highCutSlopeEnum{ Slope::slope_12 };
};  

ChainSettings getChainSettings(juce::AudioProcessorValueTreeState& apvts);

class SimpleEQAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    SimpleEQAudioProcessor();
    ~SimpleEQAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //ChainSettings getChainSettings(juce::AudioProcessorValueTreeState& apvts);

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    juce::AudioProcessorValueTreeState apvts{*this,nullptr,"parameter",createParameterLayout()};

private:
	using Filter = juce::dsp::IIR::Filter<float>;
	using CutFilter = juce::dsp::ProcessorChain<Filter, Filter, Filter, Filter>;
    using MonoChain = juce::dsp::ProcessorChain<CutFilter, Filter, CutFilter > ;
	MonoChain leftChain, rightChain;

    enum chainPositions
    {
        LowCut,
        Peak,
        HighCut
    };

	void updatePeakFilter(const ChainSettings& chainSettings);
    using coefficients = Filter::CoefficientsPtr;
    static void updateCoefficients(coefficients& old, const coefficients& replacements);
    
    template<typename ChainType,typename CoefficientType> 
    void updateCutFilter(ChainType& leftLowCut, 
        const CoefficientType& cutcoefficients,
        /*const ChainSettings& chainSettings*/
        const Slope& lowCutSlope) {

        leftLowCut.template setBypassed<0>(true);
        leftLowCut.template setBypassed<1>(true);
        leftLowCut.template setBypassed<2>(true);
        leftLowCut.template setBypassed<3>(true);

        //switch (chainSettings.lowCutSlope)
		switch (lowCutSlope)
        {
        case slope_12:
            *leftLowCut.template get<0>().coefficients = *cutcoefficients[0];
            leftLowCut.template setBypassed<0>(false);
            break;
        case slope_24:
            *leftLowCut.template get<0>().coefficients = *cutcoefficients[0];
            leftLowCut.template setBypassed<0>(false);
            *leftLowCut.template get<1>().coefficients = *cutcoefficients[1];
            leftLowCut.template setBypassed<1>(false);
            break;
        case slope_36:
            *leftLowCut.template get<0>().coefficients = *cutcoefficients[0];
            leftLowCut.template setBypassed<0>(false);
            *leftLowCut.template get<1>().coefficients = *cutcoefficients[1];
            leftLowCut.template setBypassed<1>(false);
            *leftLowCut.template get<2>().coefficients = *cutcoefficients[2];
            leftLowCut.template setBypassed<2>(false);
            break;
        case slope_48:
            *leftLowCut.template get<0>().coefficients = *cutcoefficients[0];
            leftLowCut.template setBypassed<0>(false);
            *leftLowCut.template get<1>().coefficients = *cutcoefficients[1];
            leftLowCut.template setBypassed<1>(false);
            *leftLowCut.template get<2>().coefficients = *cutcoefficients[2];
            leftLowCut.template setBypassed<2>(false);
            *leftLowCut.template get<3>().coefficients = *cutcoefficients[3];
            leftLowCut.template setBypassed<3>(false);
            break;
        default:
            break;
        }
    }

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleEQAudioProcessor)
};
