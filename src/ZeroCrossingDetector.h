#pragma once

#include <vector>
#include <iterator>

class ZeroCrossingDetector {

public:
	using Samples = std::vector<double>;
	using Crossings = std::vector<Samples::size_type>;

	ZeroCrossingDetector();

	void detect(
		const Samples& samples,
		std::back_insert_iterator<Crossings> inserter);
};

