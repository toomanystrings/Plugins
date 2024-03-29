//
// Created by Conor Foran on 29/01/2024.
//

#ifndef JUCECMAKEREPO_SYNTHVOICE_H
#define JUCECMAKEREPO_SYNTHVOICE_H

#include <JuceHeader.h>

class SynthVoice : public juce::SynthesiserVoice
{
public:
    bool canPlaySound (juce::SynthesiserSound *) override;
    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition) override;
    void stopNote (float velocity, bool allowTailOff) override;
    void pitchWheelMoved (int newPitchWheelValue) override;
    void controllerMoved (int controllerNumber, int newControllerValue) override;
    void renderNextBlock (juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples) override;

    void prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels);
};

class SynthSound : public juce::SynthesiserSound
{
    bool appliesToNote (int midiNoteNumber) override { return true; }
    bool appliesToChannel (int midiChannel) override { return true; }
};


#endif //JUCECMAKEREPO_SYNTHVOICE_H
