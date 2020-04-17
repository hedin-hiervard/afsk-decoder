#pragma once

#include <iterator>

#include "data_types.h"

class ZeroCrossingDetector {

public:
	ZeroCrossingDetector();

	void detect(
		const Samples& samples,
		std::back_insert_iterator<Crossings> inserter);
};

