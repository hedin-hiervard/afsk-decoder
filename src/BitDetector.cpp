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

auto BitDetector::detectSegment(std::string&& segmentId, Samples::size_type numSamplesInSegment) -> Result
{
	double segmentLengthInSeconds = static_cast<double>(numSamplesInSegment) / static_cast<double>(m_samplesPerSecond);
	auto segmentLengthInMicroseconds = segmentLengthInSeconds * 1e6;
	if(m_segmentInserter) {
		*m_segmentInserter = { segmentId, segmentLengthInMicroseconds };
	}

	if(withinVariation(segmentLengthInMicroseconds, ZeroBitLengthInMicroseconds, m_maxVariation)) {
		*m_inserter = 0;
		return {
			.zeroBits = 1,
			.oneBits = 0,
			};
	} else if(withinVariation(segmentLengthInMicroseconds, OneBitLengthInMicroseconds, m_maxVariation)) {
		*m_inserter = 1;
		return {
			.zeroBits = 0,
			.oneBits = 1,
			};
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
		return {
			.zeroBits = 0,
			.oneBits = 0,
			};
	}
}

auto BitDetector::detect(
		size_t totalSamples,
		const Crossings& crossings,
		int samplesPerSecond,
		double maxVariation,
		std::back_insert_iterator<Bits> inserter,
		std::back_insert_iterator<Errors> errorInserter,
		std::optional<std::back_insert_iterator<Segments>> segmentInserter
	) -> Result
{
	if(crossings.empty()) { return Result(); }

	m_inserter = inserter;
	m_errorInserter = errorInserter;
	m_segmentInserter = segmentInserter;
	m_samplesPerSecond = samplesPerSecond;
	m_maxVariation = maxVariation;

	Result result;

	/* from zero to first crossing */
	result += detectSegment("leading", crossings[0]);

	for(Crossings::size_type idx = 1; idx < crossings.size(); idx++) {
		if(crossings[idx] > totalSamples) {
			errorInserter = { to_string(idx), "segment outside totalSamples" };
			return result;
		}
		if(crossings[idx] >= crossings[idx - 1]) {
			Samples::size_type numSamplesInSegment = crossings[idx] - crossings[idx - 1];
			result += detectSegment(to_string(idx - 1) + " - " + to_string(idx), numSamplesInSegment);
		} else {
			errorInserter = { to_string(idx), "segment length negative" };
			continue;
		}
	}

	result += detectSegment("trailing", totalSamples - crossings.back());
	return result;
}