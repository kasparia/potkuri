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
      
  private:
    juce::ADSR adsr;
    juce::ADSR::Parameters adsrParams;

    juce::dsp::Oscillator<float> osc {
        [](float sinX) {
            return std::sin(sinX);
        }
    };

    juce::dsp::Gain<float> gain;

    bool isPrepared { false };
};