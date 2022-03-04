#ifndef dBFS_Analysis_h
#define dBFS_Analysis_h

#include <cmath>

class DBFSAnalysis
{
public:

    DBFSAnalysis()
    {
        for (int i = 0; i < rmsArraySize; ++i)
            rmsArray[i] = 0.0f;
    }
    ~DBFSAnalysis() {}

    float processSample(float inputSample, int channel)
    {
        rmsArray[index] = powf(inputSample, 2);
        if (index < rmsArraySize - 1)
            index++;

        if (index == rmsArraySize - 1)
            index = 0;

        //sampleCount++;
    	
        if (true)
        {
            float total = 0.0f;
            for (int i = 0; i < rmsArraySize; ++i)
                total += rmsArray[i];

            float rms = sqrtf(total / rmsArraySize);

            float outputSample = rms;


            //inputSample = fabs(inputSample);

            if (previousSample[channel] < rms) {
                g = ga; // Meter rising
            }
            else {
                g = gr; // Meter falling
            }

            outputSample = (1.0f - g) * rms + g * previousSample[channel];
            previousSample[channel] = outputSample;

            // Convert to decibel scale
            outputSample = 20.0f * log10(outputSample / rootTwo);

            // Set a floor, -75 was picked to closely match the meter in Logic
            if (outputSample < -75.0f) { outputSample = -75.0f; }

            // Convert from scale: -75 dB to 0 db, over to the scale: 0 to 1 for the meter
            outputSample = (outputSample / 75.0f) + 1;

            //sampleCount = 0;

            return outputSample;
        }
    }

    float processComp(float inputSample, int channel)
    {
        rmsArray[index] = powf(inputSample, 2);
        if (index < rmsArraySize - 1)
            index++;

        if (index == rmsArraySize - 1)
            index = 0;

        //sampleCount++;

        if (true)
        {
            float total = 0.0f;
            for (int i = 0; i < rmsArraySize; ++i)
                total += rmsArray[i];

            float rms = sqrtf(total / rmsArraySize);

            float outputSample = rms;


            //inputSample = fabs(inputSample);

            if (previousSample[channel] < rms) {
                g = ga; // Meter rising
            }
            else {
                g = gr; // Meter falling
            }

            outputSample = (1.0f - g) * rms + g * previousSample[channel];
            previousSample[channel] = outputSample;

            // Convert to decibel scale
            outputSample = 20.0f * log10(outputSample);

            // Set a floor, -75 was picked to closely match the meter in Logic
            if (outputSample < -75.0f) { outputSample = -75.0f; }

            // Convert from scale: -75 dB to 0 db, over to the scale: 0 to 1 for the meter
            outputSample = (outputSample / 75.0f) + 1;

            //sampleCount = 0;

            return outputSample;
        }
    }

    float processArray(std::vector<float> meterArray)
    {
        float total = 0.0f;
        for (int i = 0; i < meterArray.size(); ++i)
            total += meterArray[i];

        float rms = sqrtf(total / meterArray.size());

        float outputSample = rms;


        //inputSample = fabs(inputSample);

        if (previousSample[0] < rms) {
            g = ga; // Meter rising
        }
        else {
            g = gr; // Meter falling
        }

        outputSample = (1.0f - g) * rms + g * previousSample[0];
        previousSample[0] = outputSample;

        // Convert to decibel scale
        outputSample = 20.0f * log10(outputSample);

        // Set a floor, -75 was picked to closely match the meter in Logic
        if (outputSample < -75.0f) { outputSample = -75.0f; }

        // Convert from scale: -75 dB to 0 db, over to the scale: 0 to 1 for the meter
        outputSample = (outputSample / 75.0f) + 1;

        //sampleCount = 0;

        return outputSample;
    }

    void setSampleRate(int sampleRate)
    {
        if (this->sampleRate != sampleRate) {
            this->sampleRate = sampleRate;
            // Update Rise and Fall Time, these values were picked to match Logic's meter
            ga = exp(-log(9) / ((float)sampleRate * 0.05));
            gr = exp(-log(9) / ((float)sampleRate * 0.85));

            previousSample[0] = 0.0f;
            previousSample[1] = 0.0f;

            //maxSamples = static_cast<int>(sampleRate/6000);
        }
    }
private:

    float sampleRate = 1.0;

    float rootTwo = 1.0 / sqrt(2);

    float ga;
    float gr;
    float g;

    float rmsArray[512] = { 0.0f };
    int rmsArraySize = 512;
    int index = 0;

    float previousSample[2] = { 0.0f };

    int sampleCount = 0;
    int maxSamples = 0;
};

#endif