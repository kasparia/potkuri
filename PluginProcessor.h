#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_devices/juce_audio_devices.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include <juce_audio_utils/juce_audio_utils.h>

//==============================================================================
class AudioPluginAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    AudioPluginAudioProcessor();
    ~AudioPluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    using AudioProcessor::processBlock;

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

    float oscBasePitch = 2.0f; // x
    float oscPitchValue = 2.0f;
    float midiPitchValue = 0.0f;
    float pitchRunTimer = 200.0f;
    
    float runner = 14.20f; // potenssiin y

    float gainValue = 0.0f;
    //float oscRunner = 0.0f;
    
    juce::dsp::Oscillator<float> osc {
        [](float sinX) {
            return std::sin(sinX);
        }
    };

    // drum osc
    /* juce::dsp::Oscillator<float> osc {
        [](float sinX) {
            return std::sin(sinX);
        }
    }; */
    
    int lastPitch { 0 };
    int lastMidiNote { 0 };

    bool midiOscOn = false; // when midi note is one -> play

private:
    juce::dsp::Gain<float> gain;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessor)
};
