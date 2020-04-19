#pragma once

#include <functional>

#include "data_types.h"
#include "MessageDecoder.h"

class AfskDecoder {
public:
	using Writer = std::function<void(std::string)>;

	void decode(
		std::string&& filename,
		BitRanges bitRanges,
		Samples::size_type zeroCrossingResolution,
		Writer infoWriter,
		Writer errorWriter,
		Writer messageWriter,
		Writer rawWriter
	);
private:
	Crossings detectZeroCrossings(const Samples& samples, Samples::size_type zcDetectResolution);
	Bits detectBits(const Crossings& crossings, Samples::size_type totalSamples, int samplesPerSecond, BitRanges bitRanges);
	Bytes detectBytes(const Bits& bits);
	MessageDecoder::Messages decodeMessages(const Bytes& bytes);

	void info(std::string msg);
	void error(std::string msg);
	void message(std::string msg);
	void raw(std::string msg);

	Writer m_infoWriter;
	Writer m_errorWriter;
	Writer m_messageWriter;
	Writer m_rawWriter;
};