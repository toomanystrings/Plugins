/*
  ==============================================================================

    FIFO.cpp
    Created: 19 May 2021 3:18:05pm
    Author:  Olorin

  ==============================================================================
*/

#include "FIFO.h"

DivisionVoid::FIFO::FIFO(int size, bool isRMS = false) : fifoSize(size), isRMS(isRMS)
{
    fifo.clear();
    fifo.resize(fifoSize);
}

DivisionVoid::FIFO::~FIFO()
{
}

// Add a single value to the FIFO.
void DivisionVoid::FIFO::addValueToFifo(float value)
{
	if (isRMS)
		fifo[index] = powf(value, 2);
	else
        fifo[index] = value;
	
    advanceIndex();
}

// Add a buffer/array to the FIFO.
void DivisionVoid::FIFO::addValuesToFifo(const float* values, int size)
{
	for (int i = 0; i < size; ++i)
	{
        if (isRMS)
            fifo[index] = powf(values[i], 2);
        else
            fifo[index] = values[i];
		
        advanceIndex();
	}
}

// Return the FIFO as a vector.
std::vector<float>& DivisionVoid::FIFO::getFifo()
{
    return fifo;
}

// Return a pointer to the vectors data.
float* DivisionVoid::FIFO::getFifoData()
{
    return fifo.data();
}

// Return the size of the FIFO.
int DivisionVoid::FIFO::getFifoSize()
{
    return fifoSize;
}

float DivisionVoid::FIFO::getValueAt(int index)
{
    jassert(index < fifoSize);

    return fifo[index];
}

void DivisionVoid::FIFO::setFifoSize(int newSize)
{
    if (fifoSize != newSize || fifo.size() != newSize)
    {
        fifo.resize(newSize);
        this->fifoSize = newSize;
    }
}

void DivisionVoid::FIFO::advanceIndex()
{
    if (++index >= fifoSize)
        index = 0;
}
