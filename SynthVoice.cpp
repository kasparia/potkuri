#include "SynthVoice.h"

bool SynthVoice::canPlaySound (juce::SynthesiserSound* sound) {
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void SynthVoice::startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition) {
  baseNote = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
  // osc.setFrequency(baseNote);
  adsr.noteOn();
  std::cout << "Start pitch: " << baseNote << std::endl;

  runningTime = 0.0;
  runningNote = 8.5;
  currentPitch = baseNote * 4.0;
  
  if (currentPitch > 19500.0) {
    currentPitch = 19000.0;
  }
}

void SynthVoice::stopNote (float velocity, bool allowTailOff) {
  adsr.noteOff();
}

void SynthVoice::controllerMoved (int controllerNumber, int newControllerValue) {
    
}

void SynthVoice::pitchWheelMoved (int newPitchWheelValue) {
    
}

void SynthVoice::prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels) {
  adsr.setSampleRate(sampleRate);

  adsrParams.attack = 0.0f;
  adsrParams.decay = 0.5f;
  adsrParams.sustain = 0.5f;
  adsrParams.release = 0.2f;


  adsr.setParameters(adsrParams);

  juce::dsp::ProcessSpec spec;
  spec.maximumBlockSize = samplesPerBlock;
  spec.sampleRate = sampleRate;
  spec.numChannels = outputChannels;
  osc.prepare(spec);
  gain.prepare(spec);

  osc.setFrequency(440.0f);
  gain.setGainLinear(0.25f);

  isPrepared = true;
}

void SynthVoice::renderNextBlock (juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples) {
  jassert(isPrepared);

  setPitchValue();

  juce::dsp::AudioBlock<float> audioBlock { outputBuffer };
  osc.process( juce::dsp::ProcessContextReplacing<float> (audioBlock) );
  gain.process( juce::dsp::ProcessContextReplacing<float> (audioBlock) );

  adsr.applyEnvelopeToBuffer(outputBuffer, startSample, numSamples);
}

void SynthVoice::setADSRParameters(float attack, float release) {
  adsrParams.attack = attack;
  adsrParams.release = release;
  adsr.setParameters(adsrParams);
}

void SynthVoice::setPitchValue() {
  runningTime += 0.1;
std::cout << "time: " << runningTime << "    PITCH: " << currentPitch << std::endl;

  if (runningTime > 5.0) {

      if (runningNote > 0.4 && currentPitch > 40.0) {
        currentPitch = currentPitch - exp(runningNote);
        runningNote -= (0.8 * adsrParams.release);

        if (currentPitch < 20.0) {
          currentPitch = 20.0;
        }
      } else {
        currentPitch = 20.0;
      }

    
      std::cout << "-----------" << std::endl;
      osc.setFrequency(currentPitch);
      runningTime = 0.0;
  }
  
}
