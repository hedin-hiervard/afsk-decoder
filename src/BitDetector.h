#pragma once

#include "data_types.h"

class BitDetector {
public:
	struct Segment {
		Crossings::size_type from;
		Crossings::size_type to;
	};

	using Errors = std::vector<Segment>;

	void detect(
		const Crossings& crossings,
		int samplesPerSecond,
		double maxVariation,
		std::back_insert_iterator<Bits> inserter,
		std::back_insert_iterator<Errors> errorInserter
	);
private:
	static bool withinVariation(double value, double referenceValue, double maxVariation);

	static constexpr double ZeroBitLengthInMicroseconds = 640;
	static constexpr double OneBitLengthInMicroseconds = 320;
};