#include <iostream>
#include <numeric>

#include "ZeroCrossingDetector.h"

using namespace std;

void ZeroCrossingDetector::detect(
	const Samples& samples,
	Samples::size_type resolutionInSamples,
	back_insert_iterator<Crossings> inserter
) {
	if(samples.empty()) { return; }

	for(Crossings::size_type windowBegin = 0; windowBegin + resolutionInSamples < samples.size(); windowBegin += 1) {
		auto windowEnd = windowBegin + resolutionInSamples;
		const auto windowMiddle = (windowBegin + resolutionInSamples / 2);

		double leftSum = accumulate(samples.begin() + windowBegin, samples.begin() +  + windowMiddle, 0.0);
		double rightSum = accumulate(samples.begin() + windowMiddle, samples.begin() +  + windowEnd, 0.0);

		if(leftSum * rightSum < 0) {
			inserter = windowMiddle;
		}
	}
}

