#include <iostream>

#include "audiofile/AudioFile.h"

#include "ZeroCrossingDetector.h"
#include "BitDetector.h"
#include "ByteDetector.h"
#include "MessageDecoder.h"

using namespace std;


bool printRaw = false;
double maxWidthVariation = 0.0;

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
		totalSamples,
		crossings,
		samplesPerSecond,
		maxWidthVariation,
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
	if(argc < 3) {
		cout << "usage: decode <wav file> <rectangle width max variation> [--raw]" << endl;
		return 0;
	}
	const std::string fileName = argv[1];
	maxWidthVariation = stod(argv[2]);
	if(maxWidthVariation < 0.0 || maxWidthVariation > 1.0) {
		cout << "max rectangle width variation must be >= 0.0 and <= 1.0" << endl;
		return 0;
	}

	if(argc >= 4 && string(argv[3]) == "--raw") {
		printRaw = true;
	}

	cout << "loading " << fileName << endl;

	AudioFile<double> audioFile;
	audioFile.load(fileName);

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