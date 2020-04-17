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

void BitDetector::detectSegment(std::string&& segmentId, Samples::size_type numSamplesInSegment)
{
	double segmentLengthInSeconds = static_cast<double>(numSamplesInSegment) / static_cast<double>(m_samplesPerSecond);
	auto segmentLengthInMicroseconds = segmentLengthInSeconds * 1e6;

	if(withinVariation(segmentLengthInMicroseconds, ZeroBitLengthInMicroseconds, m_maxVariation)) {
		*m_inserter = false;
	} else if(withinVariation(segmentLengthInMicroseconds, OneBitLengthInMicroseconds, m_maxVariation)) {
		*m_inserter = true;
	} else {
		*m_errorInserter = {
			segmentId,
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

void BitDetector::detect(
		size_t totalSamples,
		const Crossings& crossings,
		int samplesPerSecond,
		double maxVariation,
		std::back_insert_iterator<Bits> inserter,
		std::back_insert_iterator<Errors> errorInserter
	)
{
	if(crossings.empty()) { return; }

	m_inserter = inserter;
	m_errorInserter = errorInserter;
	m_samplesPerSecond = samplesPerSecond;
	m_maxVariation = maxVariation;

	/* from zero to first crossing */
	detectSegment("leading", crossings[0]);

	for(Crossings::size_type idx = 1; idx < crossings.size(); idx++) {
		if(crossings[idx] > totalSamples) {
			errorInserter = { to_string(idx), "segment outside totalSamples" };
			return;
		}
		if(crossings[idx] >= crossings[idx - 1]) {
			Samples::size_type numSamplesInSegment = crossings[idx] - crossings[idx - 1];
			detectSegment(to_string(idx - 1) + " - " + to_string(idx), numSamplesInSegment);
		} else {
			errorInserter = { to_string(idx), "segment length negative" };
			continue;
		}
	}

	detectSegment("trailing", totalSamples - crossings.back());
}