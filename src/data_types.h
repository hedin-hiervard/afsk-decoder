#pragma once

#include <vector>

using Bit = short;
using Byte = std::uint8_t;
using DoubleByte = std::uint16_t;
using Samples = std::vector<double>;
using Crossings = std::vector<Samples::size_type>;
using Bits = std::vector<Bit>;
using Bytes = std::vector<Byte>;

struct Range {
	double min;
	double max;
};

struct BitRanges {
	Range zeroBitRangeInMicroseconds;
	Range oneBitRangeInMicroseconds;
};
