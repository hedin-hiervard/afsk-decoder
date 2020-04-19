#include <stdio.h>

#include "dj_fft/dj_fft.h"

#include "ZeroCrossingDetector.h"

using namespace std;

ZeroCrossingDetector::ZeroCrossingDetector()
{}

void ZeroCrossingDetector::detect(
	const Samples& samples,
	Samples::size_type resolutionInSamples,
	back_insert_iterator<Crossings> inserter
) {
	if(samples.empty()) { return; }

	for(auto idx = 1; idx < samples.size(); idx++)
	{
		const auto& prevSample = samples[idx - 1];
		const auto& curSample = samples[idx];
 		if(curSample * prevSample < 0) {
 			inserter = idx;
 		}
	}
}

