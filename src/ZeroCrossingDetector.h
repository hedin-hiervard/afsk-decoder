#pragma once

#include <vector>
#include <iterator>

class ZeroCrossingDetector {

public:
	using Samples = std::vector<double>;
	using Crossings = std::vector<size_t>;

	ZeroCrossingDetector();

	void detect(
		Samples::const_iterator begin,
		Samples::const_iterator end,
		std::back_insert_iterator<Crossings> inserter);
};

