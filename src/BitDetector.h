#pragma once

#include <string>
#include <optional>

#include "data_types.h"

class BitDetector {
public:
	struct Error {
		std::string segmentId;
		std::string message;
	};

	using Errors = std::vector<Error>;

	void detect(
		size_t totalSamples,
		const Crossings& crossings,
		int samplesPerSecond,
		double maxVariation,
		std::back_insert_iterator<Bits> inserter,
		std::back_insert_iterator<Errors> errorInserter
	);
private:
	static double variation(double value, double referenceValue);
	static bool withinVariation(double value, double referenceValue, double maxVariation);

	void detectSegment(std::string&& segmentId, Samples::size_type numSamplesInSegment);

	static constexpr double ZeroBitLengthInMicroseconds = 640;
	static constexpr double OneBitLengthInMicroseconds = 320;

	std::optional<std::back_insert_iterator<Bits>> m_inserter;
	std::optional<std::back_insert_iterator<Errors>> m_errorInserter;
	int m_samplesPerSecond;
	double m_maxVariation;
};