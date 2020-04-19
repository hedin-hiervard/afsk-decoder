#include <iostream>

#include "audiofile/AudioFile.h"
#include "argparse.hpp"

#include "ZeroCrossingDetector.h"
#include "BitDetector.h"
#include "ByteDetector.h"
#include "MessageDecoder.h"

using namespace std;
using namespace argparse;

bool printRaw;

double zeroMinWidth;
double zeroMaxWidth;

double oneMinWidth;
double oneMaxWidth;

Crossings detectZeroCrossings(const Samples& samples) {
	cout << "detecting zero crossings" << endl;
	ZeroCrossingDetector zcDetector;
	Crossings crossings;
	zcDetector.detect(samples, back_inserter(crossings));
	cout << "detected " << crossings.size() << " zero-crossings " << endl;
	return crossings;
}

Bits detectBits(const Crossings& crossings, Samples::size_type totalSamples, int samplesPerSecond) {
	cout << "detecting bits" << endl;
	BitDetector bitDetector;
	Bits bits;
	BitDetector::Errors bitErrors;
	BitDetector::Segments segments;

	auto result = bitDetector.detect(
		crossings,
		totalSamples,
		samplesPerSecond,
		{
			.zeroBitWidthInMicroseconds = { zeroMinWidth, zeroMaxWidth },
			.oneBitWidthInMicroseconds = { oneMinWidth, oneMaxWidth },
		},
		back_inserter(bits),
		back_inserter(bitErrors),
		back_inserter(segments)
	);

	cout << "detected " << bits.size() << " bits" << endl;
	cout << result.zeroBits << " zeros, " << result.oneBits << " ones" << endl;
	cout << bitErrors.size() << " errors " << endl;
	for(const auto& e: bitErrors) {
		cout << "bit error: " << e.message << " at segment: " << e.segmentId << endl;
	}
	if(printRaw) {
		cout << "bits: " << endl;
		for(const auto& b: bits) {
			cout << to_string(b);
		}
		cout << endl;
	}
	return bits;
}

Bytes detectBytes(const Bits& bits) {
	cout << "detecting bytes" << endl;
	ByteDetector byteDetector;
	Bytes bytes;
	ByteDetector::Errors byteErrors;

	byteDetector.detect(
		bits,
		back_inserter(bytes),
		back_inserter(byteErrors)
	);

	cout << "detected " << bytes.size() << " bytes, " << byteErrors.size() << " errors " << endl;
	for(const auto& e: byteErrors) {
		cout << "byte error: " << e.message << " at bit: " << e.position << endl;
		cout << "bit context: " << e.bitContext << endl;
	}

	if(printRaw) {
		cout << "bytes: " << endl;
		for(const auto& b : bytes) {
			cout << hex << static_cast<int>(b) << dec << " ";
		}
		cout << endl;
	}
	return bytes;
}

MessageDecoder::Messages decodeMessages(const Bytes& bytes) {
	MessageDecoder messageDecoder;
	MessageDecoder::Messages messages;
	MessageDecoder::Errors messageErrors;

	messageDecoder.decode(
		bytes,
		back_inserter(messages),
		back_inserter(messageErrors)
	);

	cout << "decoded " << messages.size() << " messages, " << messageErrors.size() << " errors " << endl;
	for(const auto& e: messageErrors) {
		cout << "message error: " << e.message << " at byte: " << e.position << endl;
	}

	cout << "messages: " << endl;
	for(const auto& message : messages) {
		for(const auto& byte : message) {
			cout << byte;
		}
	}
	cout << endl;
	return messages;
}

auto main(int argc, char** argv) -> int {
	ArgumentParser program("afsk decoder");

	program.add_argument("-f", "--file")
		.help("input wav file to decode")
		.required();

	program.add_argument("--min-zero")
		.help("min width of zero rectangle in microseconds")
		.action([](const std::string& value) { return std::stod(value); })
		.default_value(600.0);

	program.add_argument("--max-zero")
		.help("max width of zero rectangle in microseconds")
		.action([](const std::string& value) { return std::stod(value); })
		.default_value(680.0);

	program.add_argument("--min-one")
		.help("min width of one rectangle in microseconds")
		.action([](const std::string& value) { return std::stod(value); })
		.default_value(300.0);

	program.add_argument("--max-one")
		.help("max width of one rectangle in microseconds")
		.action([](const std::string& value) { return std::stod(value); })
		.default_value(360.0);

	program.add_argument("--raw")
		.help("print raw bits and bytes")
		.default_value(false)
		.implicit_value(true);

	try {
	    program.parse_args(argc, argv);
	 }
	 catch (const std::runtime_error& err) {
		std::cout << err.what() << std::endl;
		std::cout << program;
		exit(1);
	 }

	auto filename = program.get<string>("--file");
	zeroMinWidth = program.get<double>("--min-zero");
	zeroMaxWidth = program.get<double>("--max-zero");
	oneMinWidth = program.get<double>("--min-one");
	oneMaxWidth = program.get<double>("--max-one");
	printRaw = program.get<bool>("--raw");

	cout << "loading " << filename << endl;

	AudioFile<double> audioFile;
	auto fileLoaded = audioFile.load(filename);

	if(!fileLoaded) {
		cout << "failed to load audio file " << filename << endl;
		return 0;
	}

	cout << "loaded file" << endl;
	audioFile.printSummary();


	for(int channel = 0; channel < audioFile.getNumChannels(); channel++)
	{
		cout << "parsing channel " << channel << endl;

		auto crossings = detectZeroCrossings(audioFile.samples[channel]);
		auto bits = detectBits(
			crossings,
			audioFile.samples[channel].size(),
			audioFile.getSampleRate()
		);
		auto bytes = detectBytes(bits);
		decodeMessages(bytes);
	}
	return 0;
}