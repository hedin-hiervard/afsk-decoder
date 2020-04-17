#include <iostream>
#include <cmath>

#include "BitDetector.h"

using namespace std;

double BitDetector::variation(double value, double referenceValue) {
	return std::abs(referenceValue - value) / referenceValue;
}

bool BitDetector::withinVariation(double value, double referenceValue, double maxVariation) {
	return variation(value, referenceValue) <= maxVariation;
}

void BitDetector::detect(
		const Crossings& crossings,
		int samplesPerSecond,
		double maxVariation,
		std::back_insert_iterator<Bits> inserter,
		std::back_insert_iterator<Errors> errorInserter
	)
{
	if(crossings.empty()) { return; }

	for(Crossings::size_type idx = 0; idx < crossings.size(); idx++) {
		Samples::size_type numSamplesInSegment;

		if(idx > 0) {
			if(crossings[idx] > crossings[idx - 1]) {
				numSamplesInSegment = crossings[idx] - crossings[idx - 1];
			} else {
				errorInserter = { { idx - 1, idx }, "segment length negative" };
				continue;
			}
		} else {
			numSamplesInSegment = crossings[idx];
		}
		double segmentLengthInSeconds = static_cast<double>(numSamplesInSegment) / static_cast<double>(samplesPerSecond);
		auto segmentLengthInMicroseconds = segmentLengthInSeconds * 1e6;

		if(withinVariation(segmentLengthInMicroseconds, ZeroBitLengthInMicroseconds, maxVariation)) {
			inserter = false;
		} else if(withinVariation(segmentLengthInMicroseconds, OneBitLengthInMicroseconds, maxVariation)) {
			inserter = true;
		} else {
			errorInserter = {
				{ idx - 1, idx },
				"can't recognize segment length "
					+ to_string(segmentLengthInMicroseconds)
					+ " microseconds, "
					+ " variation from zero: "
					+ to_string(variation(segmentLengthInMicroseconds, ZeroBitLengthInMicroseconds))
					+ " variation from one: "
					+ to_string(variation(segmentLengthInMicroseconds, OneBitLengthInMicroseconds))
			};
		}
	}
}