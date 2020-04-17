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

	struct Segment {
		std::string id;
		double length;
	};

	struct Result {
		size_t oneBits = 0;
		size_t zeroBits = 0;

		Result operator+=(const Result& other) {
			oneBits += other.oneBits;
			zeroBits += other.zeroBits;
			return *this;
		}
	};

	using Errors = std::vector<Error>;
	using Segments = std::vector<Segment>;

	Result detect(
		size_t totalSamples,
		const Crossings& crossings,
		int samplesPerSecond,
		double maxVariation,
		std::back_insert_iterator<Bits> inserter,
		std::back_insert_iterator<Errors> errorInserter,
		std::optional<std::back_insert_iterator<Segments>> segmentInserter = {}
	);
private:
	static double variation(double value, double referenceValue);
	static bool withinVariation(double value, double referenceValue, double maxVariation);

	Result detectSegment(std::string&& segmentId, Samples::size_type numSamplesInSegment);

	static constexpr double ZeroBitLengthInMicroseconds = 640;
	static constexpr double OneBitLengthInMicroseconds = 320;

	std::optional<std::back_insert_iterator<Bits>> m_inserter;
	std::optional<std::back_insert_iterator<Errors>> m_errorInserter;
	std::optional<std::back_insert_iterator<Segments>> m_segmentInserter;

	int m_samplesPerSecond;
	double m_maxVariation;
};