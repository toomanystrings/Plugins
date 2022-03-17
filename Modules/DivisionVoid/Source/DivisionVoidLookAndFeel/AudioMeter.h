//
// Created by Conor Foran on 10/03/2022.
//

#pragma once

inline float clip(float x)
{
    return std::max(0.0f, std::min(1.0f, x));
}
namespace DivisionVoid {
    class AudioMeter : public juce::Component, private juce::Timer {
    public:
        enum MeterStyle {
            Vertical,
            Horizontal
        };

        enum MeterCalibration {
            Peak,
            RMS,
            VU,
            Custom
        };

        AudioMeter() {
            meterStyle = Vertical;
        }

        ~AudioMeter() = default;

        void setMeterStyle(MeterStyle style) {
            meterStyle = style;
            resized();
            repaint();
        }

        MeterStyle getMeterStyle() const {
            return meterStyle;
        }

        void setMeterCalibration(MeterCalibration calibration) {
            meterCalibration = calibration;

            // There is a load of switchy things to do here, depending on rise and fall, stuff like that...
        }

        MeterCalibration getMeterCalibration() const {
            return meterCalibration;
        }

        void setMeterSource(int channel, float *const source) {
            meterSources.set(channel, source);
            meterBuffers.set(channel, 0.0f);

            // Apparently we can set the old timer running here, instead of the constructor, but
            // we shall see!

            repaint();
        }

        void paint(juce::Graphics &g) override {
            int width = getWidth();
            int height = getHeight();

            auto backgroundColour = DivisionVoidColours::black;
            auto highlightColour = DivisionVoidColours::white;
            auto foregroundColour = DivisionVoidColours::red;

            juce::Path b;
            b.addRoundedRectangle(0, 0, width, height, CORNER_CONFIG);
            g.setColour(backgroundColour);
            g.fillPath(b);

            g.setColour(backgroundColour);
            g.fillRect(indicatorArea);

            if (meterSources.size()) {
                if (meterStyle == Vertical) {
                    int channelWidth = indicatorArea.getWidth() / meterSources.size();

                    for (int channel = 0; channel < meterSources.size(); ++channel) {
                        float output = meterBuffers[channel];
                        float peak = meterPeaks[channel];

                        g.setColour(highlightColour);

                        g.fillRect
                                (
                                        indicatorArea.getX() + channelWidth * channel,
                                        indicatorArea.getBottom() - (int) (indicatorArea.getHeight() * output),
                                        channelWidth,
                                        (int) (indicatorArea.getHeight() * output)
                                );

                        if (meterPeakStatus) {
                            g.setColour(foregroundColour);

                            g.drawLine
                                    (
                                            indicatorArea.getX() + channelWidth * channel,
                                            indicatorArea.getBottom() - (indicatorArea.getHeight() * peak),
                                            (indicatorArea.getX() + (channelWidth * channel)) + channelWidth,
                                            indicatorArea.getBottom() - (indicatorArea.getHeight() * peak),
                                            2
                                    );
                        }

                        g.setColour(backgroundColour);
                        g.drawLine
                                (
                                        (indicatorArea.getX() + channelWidth * channel) + channelWidth,
                                        indicatorArea.getY(),
                                        (indicatorArea.getX() + channelWidth * channel) + channelWidth,
                                        indicatorArea.getBottom(),
                                        1
                                );
                    }
                } else if (meterStyle == Horizontal) {
                    int channelHeight = indicatorArea.getHeight() / meterSources.size();

                    for (int channel = 0; channel < meterSources.size(); ++channel) {
                        float output = meterBuffers[channel];
                        float peak = meterPeaks[channel];

                        g.setColour(highlightColour);

                        g.fillRect
                                (
                                        indicatorArea.getX(),
                                        indicatorArea.getY() + channelHeight * channel,
                                        (int) (indicatorArea.getWidth() * output),
                                        channelHeight
                                );

                        if (meterPeakStatus) {
                            g.setColour(foregroundColour);

                            g.drawLine(indicatorArea.getX() + indicatorArea.getWidth() * peak,
                                       indicatorArea.getY() + channelHeight * channel,
                                       indicatorArea.getX() + indicatorArea.getWidth() * peak,
                                       (indicatorArea.getY() + channelHeight * channel) + channelHeight,
                                       2);
                        }

                        g.setColour(backgroundColour);
                        g.drawLine(
                                indicatorArea.getX(),
                                (indicatorArea.getY() + channelHeight * channel) + channelHeight,
                                indicatorArea.getRight(),
                                (indicatorArea.getY() + channelHeight * channel) + channelHeight,
                                1);
                    }
                }
            }

//        g.setColour(findColour(DivisionVoid::midgroundColourId));
//        for (int i = 0; i < pipLocations.size(); ++i)
//        {
//            juce::Point<int>& pip = pipLocations.getReference(i);
//            g.fillEllipse(pip.x - pipSize/2, pip.y - pipSize/2, pipSize, pipSize);
//        }

            juce::Path p;
            p.addRoundedRectangle(4, 4, width - 8, height - 8, CORNER_CONFIG);
            g.setColour(foregroundColour);
            g.strokePath(p, juce::PathStrokeType(8));
        }

        void clearSource(int channel) {
            meterSources.remove(channel);
            meterBuffers.remove(channel);

            repaint();
        }

        void clearSources() {
            meterSources.clear();
            meterBuffers.clear();

            // Apparently we can also stop the timer here, but I will have to look into how these interact
            // with the whole interface. Maybe within the parent component destructor?
        }


    private:

        void timerCallback() override {
            if (meterSources.size() == 0) {
                stopTimer();
            }

            float rise = (float) meterRise / 1000.0f;
            float fall = (float) meterFall / 1000.0f;

            for (int i = 0; i < meterSources.size(); ++i) {
                float in = std::abs(*meterSources[i]);
                float last = meterBuffers[i];
                float peak = meterPeaks[i];
                float out;

                float ga = exp(-1.0f / (float) (ANIMATION_FPS * 0.01f));
                float gr = exp(-1.0f / (float) (ANIMATION_FPS * 1.5f));

                float g;

                if (peak < in) {
                    g = ga;
                } else {
                    g = gr;
                }

                peak = (1.0f - g) * in + g * peak;
                meterPeaks.set(i, clip(peak));

                if (meterCalibration == Peak || meterCalibration == Custom) {
                    in = std::abs(*meterSources[i]);
                    last = meterBuffers[i];

                    ga = exp(-1.0f / (float) (ANIMATION_FPS * rise));
                    gr = exp(-1.0f / (float) (ANIMATION_FPS * fall));

                    if (last < in) {
                        g = ga;
                    } else {
                        g = gr;
                    }

                    out = (1.0f - g) * in + g * last;
                } else if (meterCalibration == VU) {
                    in = fmax(0.0f, *meterSources[i]);
                    last = meterBuffers[i];
                    g = exp(-1.0f / (float) (ANIMATION_FPS * rise));

                    out = (1.0f - g) * in + g * last;
                } else if (meterCalibration == RMS) {
                    in = (*meterSources[i]) * (*meterSources[i]);
                    last = meterBuffers[i];
                    g = exp(-1.0f / (float) (ANIMATION_FPS * rise));

                    out = (1.0f - g) * in + g * last;
                }

                meterBuffers.set(i, clip(out));
                repaint(indicatorArea);
            }
        }

        void resized() override {}

        juce::Array<float *> meterSources;
        juce::Array<float> meterBuffers;
        juce::Array<float> meterPeaks;

        juce::Rectangle<int> indicatorArea;

        bool meterPeakStatus;
        double currentPeakPos;

        int meterRise;
        float meterOvershoot;
        int meterFall;

        MeterCalibration meterCalibration;
        MeterStyle meterStyle;
    };
}