/*
  ==============================================================================

    FIFO.h
    Created: 19 May 2021 3:18:05pm
    Author:  Olorin

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <vector>

namespace DivisionVoid
{
	class FIFO
	{
	public:
		FIFO(int size, bool isRMS);
		~FIFO();

		
		void addValueToFifo(float value);
		void addValuesToFifo(const float* values, int size);

		std::vector<float>& getFifo();
		float* getFifoData();
		
		int getFifoSize();
		float getValueAt(int index);

		void setFifoSize(int newSize);

	private:

		void advanceIndex();

		std::vector<float> fifo;

		int index = 0;
		int fifoSize;

		// bool to tell us if we need to square values on the way in, in order to save processing power.
		// This was concieved as a way to save power before I decided to move the processing to another thread. I have
		// yet to decide on whether saving a few thousand square operations will save me all that much.
		bool isRMS = false;
	};
}
