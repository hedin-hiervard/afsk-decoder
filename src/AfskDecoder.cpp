#include <iostream>

#include "audiofile/AudioFile.h"

#include "AfskDecoder.h"
#include "ZeroCrossingDetector.h"
#include "BitDetector.h"
#include "ByteDetector.h"

using namespace std;

Crossings AfskDecoder::detectZeroCrossings(const Samples& samples, Samples::size_type zcDetectResolution) {
	info("detecting zero crossings");
	ZeroCrossingDetector zcDetector;
	Crossings crossings;
	zcDetector.detect(samples, zcDetectResolution, back_inserter(crossings));
	info("detected " + to_string(crossings.size()) + " zero-crossings");
	return crossings;
}

Bits AfskDecoder::detectBits(const Crossings& crossings, Samples::size_type totalSamples, int samplesPerSecond, BitRanges bitRanges) {
	info("detecting bits");
	Bits bits;
	BitDetector bitDetector;
	BitDetector::Errors bitErrors;
	BitDetector::Segments segments;

	auto result = bitDetector.detect(
		crossings,
		totalSamples,
		samplesPerSecond,
		move(bitRanges),
		back_inserter(bits),
		back_inserter(bitErrors),
		back_inserter(segments)
	);

	info("detected " + to_string(bits.size()) + " bits");
	info(to_string(result.zeroBits) + " zeros, " + to_string(result.oneBits) + " ones");
	info(to_string(bitErrors.size()) + " errors ");

	for(const auto& e: bitErrors) {
		error("bit error: " + e.message + " at segment: " + e.segmentId);
	}
	raw(string("bits: ") + "\n");
	for(const auto& b: bits) {
		raw(to_string(b));
	}
	raw("\n");
	return bits;
}

Bytes AfskDecoder::detectBytes(const Bits& bits) {
	info("detecting bytes");
	ByteDetector byteDetector;
	Bytes bytes;
	ByteDetector::Errors byteErrors;

	byteDetector.detect(
		bits,
		back_inserter(bytes),
		back_inserter(byteErrors)
	);

	info("detected " + to_string(bytes.size()) + " bytes, " + to_string(byteErrors.size()) + " errors ");
	for(const auto& e: byteErrors) {
		error(string("byte error: ") +e.message + " at bit: " + to_string(e.position));
		error("bit context: " + e.bitContext);
	}

	raw(string("bytes: ") + "\n");
		for(const auto& b : bytes) {
			raw(to_string(static_cast<int>(b)) + " ");
		}
	raw("\n");
	return bytes;
}

MessageDecoder::Messages AfskDecoder::decodeMessages(const Bytes& bytes) {
	MessageDecoder messageDecoder;
	MessageDecoder::Messages messages;
	MessageDecoder::Errors messageErrors;

	messageDecoder.decode(
		bytes,
		back_inserter(messages),
		back_inserter(messageErrors)
	);

	info(string("decoded ") + to_string(messages.size()) + " messages, " + to_string(messageErrors.size()) + " errors");
	for(const auto& e: messageErrors) {
		error("message error: " + e.message + " at byte: " + to_string(e.position));
	}

	message("messages:\n");
	message("=========\n\n");
	for(const auto& m : messages) {
		for(const auto& byte : m) {
			message(string(1, static_cast<char>(byte)));
		}
	}
	message("\n\n");
	return messages;
}

void AfskDecoder::info(std::string msg) {
	if(!m_infoWriter) { return; }
	m_infoWriter(msg + "\n");
}

void AfskDecoder::error(std::string msg) {
	if(!m_errorWriter) { return; }
	m_errorWriter(msg + "\n");
}
void AfskDecoder::message(std::string msg) {
	if(!m_messageWriter) { return; }
	m_messageWriter(msg);
}

void AfskDecoder::raw(std::string msg) {
	if(!m_rawWriter) { return; }
	m_rawWriter(msg);
}

void AfskDecoder::decode(
		std::string&& filename,
		BitRanges bitRanges,
		Samples::size_type zeroCrossingResolution,
		Writer infoWriter,
		Writer errorWriter,
		Writer messageWriter,
		Writer rawWriter
	)
{
	m_infoWriter = infoWriter;
	m_errorWriter = errorWriter;
	m_messageWriter = messageWriter;
	m_rawWriter = rawWriter;

	info("loading");

	AudioFile<double> audioFile;
	auto fileLoaded = audioFile.load(filename);

	if(!fileLoaded) {
		error("failed to load audio file: " + filename);
		return;
	}

	info("loaded file: ");
	info("  channels: " + to_string(audioFile.getNumChannels()));
	info("  samples: " + to_string(audioFile.getNumSamplesPerChannel()));
	info("  sampleRate: " + to_string(audioFile.getSampleRate()));

	for(int channel = 0; channel < audioFile.getNumChannels(); channel++)
	{
		info("parsing channel " + to_string(channel));

		auto crossings = detectZeroCrossings(audioFile.samples[channel], zeroCrossingResolution);
		auto bits = detectBits(
			crossings,
			audioFile.samples[channel].size(),
			audioFile.getSampleRate(),
			bitRanges
		);
		auto bytes = detectBytes(bits);
		decodeMessages(bytes);
	}
}