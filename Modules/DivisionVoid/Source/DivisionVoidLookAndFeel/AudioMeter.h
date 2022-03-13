//
// Created by Conor Foran on 10/03/2022.
//

#pragma once

class AudioMeter : public juce::Component, private juce::Timer
{
public:
    enum MeterStyle
    {
        Vertical,
        Horizontal
    };

    enum MeterCalibration
    {
        Peak,
        RMS,
        VU
    };

    AudioMeter()
    {
        meterStyle = Vertical;
    }

    ~AudioMeter() = default;

    void setMeterStyle(MeterStyle style)
    {
        meterStyle = style;
        resized();
        repaint();
    }

    MeterStyle getMeterStyle() const
    {
        return meterStyle;
    }

    void setMeterCalibration(MeterCalibration calibration)
    {
        meterCalibration = calibration;

        // There is a load of switchy things to do here, depending on rise and fall, stuff like that...
    }

    MeterCalibration getMeterCalibration() const
    {
        return meterCalibration;
    }

    void setMeterSource(int channel, float* const source)
    {
        meterSources.set(channel, source);
        meterBuffers.set(channel, 0.0f);

        // Apparently we can set the old timer running here, instead of the constructor, but
        // we shall see!

        repaint();
    }

    void clearSource(int channel)
    {
        meterSources.remove(channel);
        meterBuffers.remove(channel);

        repaint();
    }

    void clearSources()
    {
        meterSources.clear();
        meterBuffers.clear();

        // Apparently we can also stop the timer here, but I will have to look into how these interact
        // with the whole interface. Maybe within the parent component destructor?
    }


private:

    void timerCallback() override {}

    void paint(juce::Graphics& g) override {}
    void resized() override {}

    juce::Array<float*> meterSources;
    juce::Array<float>  meterBuffers;
    juce::Array<float>  meterPeaks;

    MeterCalibration meterCalibration;
    MeterStyle meterStyle;
};