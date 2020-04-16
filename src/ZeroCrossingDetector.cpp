#include <stdio.h>

#include "ZeroCrossingDetector.h"

using namespace std;

ZeroCrossingDetector::ZeroCrossingDetector(AudioFile<double>&& audioFile)
: m_audioFile(audioFile)
{}

void ZeroCrossingDetector::detect() {
	for(auto channel = 0; channel < m_audioFile.getNumChannels(); channel++)
	{
		cout << "parsing channel " << channel << endl;
		double lastZeroCrossingTime = 0.0;
		for (int i = 1; i < m_audioFile.getNumSamplesPerChannel(); i++)
		{
			auto& previousSample = m_audioFile.samples[channel][i - 1];
			auto& currentSample = m_audioFile.samples[channel][i];
			if(currentSample * previousSample < 0) {
				cout << "ZC at sample " << i << endl;
			}

		}
	}
}
