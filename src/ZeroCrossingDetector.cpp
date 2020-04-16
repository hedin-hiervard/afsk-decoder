#include <stdio.h>

#include "ZeroCrossingDetector.h"

using namespace std;

ZeroCrossingDetector::ZeroCrossingDetector()
{}

void ZeroCrossingDetector::detect(Samples::const_iterator begin, Samples::const_iterator end, back_insert_iterator<Crossings> inserter) {
	if(begin == end) { return; }

	Samples::size_type idx = 0;
	Samples::const_iterator prev = begin;
	Samples::const_iterator it = begin;
	do
	{
		it++;
		idx++;
		if(it == end) { return; }

		const auto& prevSample = *prev;
		const auto& curSample = *it;
 		if(curSample * prevSample < 0) {
 			inserter = idx;
 		}

		prev = it;
	} while(true);
}

