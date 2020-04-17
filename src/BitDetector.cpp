#include <cmath>

#include "BitDetector.h"

bool BitDetector::withinVariation(double value, double referenceValue, double maxVariation) {
	return std::abs(referenceValue - value) / referenceValue <= maxVariation;
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

	double samplesPerMicrosecond = static_cast<double>(samplesPerSecond) * 1e-6;

	for(Crossings::size_type idx = 0; idx < crossings.size(); idx++) {
		Samples::size_type numSamplesInSegment;

		if(idx > 0) {
			numSamplesInSegment = crossings[idx] - crossings[idx - 1];
		} else {
			numSamplesInSegment = crossings[idx];
		}
		double segmentLengthInMicroseconds = numSamplesInSegment / samplesPerMicrosecond;;

		if(withinVariation(segmentLengthInMicroseconds, ZeroBitLengthInMicroseconds, maxVariation)) {
			inserter = false;
		} else if(withinVariation(segmentLengthInMicroseconds, OneBitLengthInMicroseconds, maxVariation)) {
			inserter = true;
		} else {
			errorInserter = { idx - 1, idx };
		}
	}
}