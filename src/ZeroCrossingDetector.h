#pragma once

#include <iterator>

#include "data_types.h"

class ZeroCrossingDetector {

public:
	void detect(
		const Samples& samples,
		Samples::size_type resolutionInSamples,
		std::back_insert_iterator<Crossings> inserter);
};

