#include "SynthVoice.h"

bool SynthVoice::canPlaySound (juce::SynthesiserSound* sound) {
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void SynthVoice::startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition) {
  baseNote = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
  osc.setFrequency(baseNote);
  adsr.noteOn();

  //currentPitch = baseNote;  

  runningTime = 0.0;
  /*runningNote = 8.5;
  currentPitch = baseNote * 4.0;
  
  if (currentPitch > 19500.0) {
    currentPitch = 19000.0;
  }*/
}

void SynthVoice::stopNote (float velocity, bool allowTailOff) {
  adsr.noteOff();

  if ( !allowTailOff || !adsr.isActive() ) {
    clearCurrentNote();
  }
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

  if (!isVoiceActive()) {
    return;
  }

  synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
  synthBuffer.clear();

  setPitchValue();

  juce::dsp::AudioBlock<float> audioBlock { synthBuffer };
  osc.process( juce::dsp::ProcessContextReplacing<float> (audioBlock) );
  gain.process( juce::dsp::ProcessContextReplacing<float> (audioBlock) );

  adsr.applyEnvelopeToBuffer(synthBuffer, startSample, numSamples);

  for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel) {
    outputBuffer.addFrom(
        channel,
        startSample,
        synthBuffer,
        channel,
        0,
        numSamples
    );

    if ( !adsr.isActive() ) {
      clearCurrentNote();
    }
  }
}

void SynthVoice::setADSRParameters(float attack, float decay, float release, float modulationValue) {
  adsrParams.attack = attack;
  adsrParams.decay = decay;
  adsrParams.release = release;
  modulationMultiply = modulationValue;
  adsr.setParameters(adsrParams);
}

void SynthVoice::setPitchValue() {
  runningTime += 0.5;

  if (runningTime > 100.0) {
    runningTime = 0.0;
  }

  osc.setFrequency(osc.getFrequency() + sin(modulationMultiply));
  
}
