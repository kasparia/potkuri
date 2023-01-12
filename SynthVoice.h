#pragma once

#include <JuceHeader.h>

#include "SynthSound.h"
#include "SynthOscillator.h"

class SynthVoice : public juce::SynthesiserVoice {
  public:
    bool canPlaySound (juce::SynthesiserSound* sound) override;
    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition) override;
    void stopNote (float velocity, bool allowTailOff) override;
    void pitchWheelMoved (int newPitchWheelValue) override;
    void controllerMoved (int controllerNumber, int newControllerValue) override;
    void renderNextBlock (juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples) override;

    void prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels);

    void setADSRParameters(float attack, float decay, float release, float modulationValue);
    juce::ADSR& getADSR() { return adsr; }

    void setPitchValue();

    SynthOscillator& getOscillator() { return osc; }

  private:

    juce::ADSR adsr;
    juce::ADSR::Parameters adsrParams;

    SynthOscillator osc;

    juce::AudioBuffer<float> synthBuffer;

    double baseNote;
    double modulationMultiply = 0.0;

    juce::dsp::Gain<float> gain;

    bool isPrepared { false };
};