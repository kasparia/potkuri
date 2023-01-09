#pragma once

#include <JuceHeader.h>

#include "SynthSound.h"

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

  private:

    juce::ADSR adsr;
    juce::ADSR::Parameters adsrParams;

    juce::dsp::Oscillator<float> osc {
      // sine
      /*[](float sinX) {
        return std::sin(sinX);
      }*/

      // saw
      /*[](float sinX) {
        return sinX / juce::MathConstants<float>::pi;
      }*/

      //
      [](float sinX) {
        return sinX / juce::MathConstants<float>::pi * std::sin(sinX);
      }
    };

    juce::AudioBuffer<float> synthBuffer;

    double baseNote;
    double runningNote = 440.0;
    double runningTime = 0;
    double currentPitch = 18000.0;
    double modulationMultiply = 0.0;

    juce::dsp::Gain<float> gain;

    bool isPrepared { false };
};