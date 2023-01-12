#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p),
      processorRef (p),
      keyboardComponent (keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard),
      startTime (juce::Time::getMillisecondCounterHiRes() * 0.001)
{
    juce::ignoreUnused (processorRef);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    addAndMakeVisible (keyboardComponent);
    keyboardState.addListener (this);


    attackSlider.setSliderStyle( juce::Slider::SliderStyle::LinearVertical );
    attackSlider.setRange(0.0f, 1.0f, 0.02f);
    attackSlider.setValue(0.01f);
    attackSlider.addListener(this);
    attackSlider.setTextBoxStyle (juce::Slider::TextBoxLeft, false, 40, attackSlider.getTextBoxHeight());
    addAndMakeVisible(attackSlider);

    addAndMakeVisible(attackLabel);
    attackLabel.setText("Attack", juce::dontSendNotification);
    attackLabel.attachToComponent(&attackSlider, false);


    decaySlider.setSliderStyle( juce::Slider::SliderStyle::LinearVertical );
    decaySlider.setRange(0.0f, 1.0f, 0.02f);
    decaySlider.setValue(0.2f);
    decaySlider.addListener(this);
    decaySlider.setTextBoxStyle (juce::Slider::TextBoxLeft, false, 40, decaySlider.getTextBoxHeight());
    addAndMakeVisible(decaySlider);

    addAndMakeVisible(decayLabel);
    decayLabel.setText("Decay", juce::dontSendNotification);
    decayLabel.attachToComponent(&decaySlider, false);
    

    releaseSlider.setSliderStyle( juce::Slider::SliderStyle::LinearVertical );
    releaseSlider.setRange(0.0f, 10.0f, 0.01f);
    releaseSlider.setValue(0.01f);
    releaseSlider.addListener(this);
    releaseSlider.setTextBoxStyle (juce::Slider::TextBoxLeft, false, 40, releaseSlider.getTextBoxHeight());
    addAndMakeVisible(releaseSlider);

    addAndMakeVisible(releaseLabel);
    releaseLabel.setText("Release", juce::dontSendNotification);
    releaseLabel.attachToComponent(&releaseSlider, false);

    // mod
    modulationSlider.setSliderStyle( juce::Slider::SliderStyle::LinearVertical );
    modulationSlider.setRange(0.0f, 1000.0f, 0.01f);
    modulationSlider.setValue(0.0f);
    modulationSlider.addListener(this);
    modulationSlider.setTextBoxStyle (juce::Slider::TextBoxLeft, false, 40, modulationSlider.getTextBoxHeight());
    addAndMakeVisible(modulationSlider);

    addAndMakeVisible(modulationLabel);
    modulationLabel.setText("Mod", juce::dontSendNotification);
    modulationLabel.attachToComponent(&modulationSlider, false);

    addAndMakeVisible (waveformSelect);
    waveformSelect.addItem ("SIN", 1);
    waveformSelect.addItem ("SAW", 2);
    waveformSelect.addItem ("SQU", 3);
    waveformSelect.setSelectedId(1);
    addAndMakeVisible(waveformLabel);
    waveformLabel.setText("WAVE", juce::dontSendNotification);
    waveformLabel.attachToComponent(&waveformSelect, false);
    waveformSelect.onChange = [this] { processorRef.waveformSelectValue = waveformSelect.getSelectedId(); };



    addAndMakeVisible (midiInputListLabel);
    midiInputListLabel.setText ("MIDI Input:", juce::dontSendNotification);
    midiInputListLabel.attachToComponent (&midiInputList, true);

    addAndMakeVisible (midiInputList);
    midiInputList.setTextWhenNoChoicesAvailable ("No MIDI Inputs Enabled");
    auto midiInputs = juce::MidiInput::getAvailableDevices();

    juce::StringArray midiInputNames;

    for (auto input : midiInputs)
        midiInputNames.add (input.name);

    midiInputList.addItemList (midiInputNames, 1);

    // find the first enabled device and use that by default
    for (auto input : midiInputs)
    {
        if (deviceManager.isMidiInputDeviceEnabled (input.identifier))
        {
            setMidiInput (midiInputs.indexOf (input));
            break;
        }
    }

    // if no enabled devices were found just use the first one in the list
    if (midiInputList.getSelectedId() == 0)
        setMidiInput (0);
    

    setSize (640, 480);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
}

void AudioPluginAudioProcessorEditor::resized()
{
    attackSlider.setBounds(30, 30, 100, 180);
    decaySlider.setBounds(120, 30, 100, 180);
    releaseSlider.setBounds(220, 30, 100, 180);
    modulationSlider.setBounds(320, 30, 100, 180);
    waveformSelect.setBounds(30, 250, 70, 25);
    
    keyboardComponent.setBounds(0, 400, 200, 80);
    
}

void AudioPluginAudioProcessorEditor::sliderValueChanged (juce::Slider *slider) {
    if (slider == &attackSlider) {
        processorRef.attackSliderValue = attackSlider.getValue();
    }
    if (slider == &decaySlider) {
        processorRef.decaySliderValue = decaySlider.getValue();
    }
    if (slider == &releaseSlider) {
        processorRef.releaseSliderValue = releaseSlider.getValue();
    }
    if (slider == &modulationSlider) {
        processorRef.modulationSliderValue = modulationSlider.getValue();
    }
}
