#pragma once

#include "SynthSound.h"
#include "SynthVoice.h"

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

    void synthPlayNote(int midiNoteNumber, float velocity);
    void synthStopNote(int midiNoteNumber, float velocity);
    
    int lastPitch { 0 };
    int lastMidiNote { 0 };

    bool midiOscOn = false; // when midi note is one -> play

    juce::Synthesiser synth;

    void setSynthParameters();
    float attackSliderValue = 0.01f;
    float decaySliderValue = 0.2f;
    float sustainSliderValue = 0.0;
    float releaseSliderValue = 0.1;
    float modulationSliderValue = 0.0f;

    int waveformSelectValue = 1; // default = sine
    
private:
    //==============================================================================

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessor)
};
