#include <iostream>
#include "audiofile/AudioFile.h"

#include "ZeroCrossingDetector.h"
#include "BitDetector.h"
#include "ByteDetector.h"

using namespace std;

auto main(int argc, char** argv) -> int {
	if(argc < 3) {
		cout << "usage: decode <wav file> <rectangle width max variation>" << endl;
		return 0;
	}
	const std::string fileName = argv[1];
	const double maxWidthVariation = stod(argv[2]);
	if(maxWidthVariation < 0.0 || maxWidthVariation > 1.0) {
		cout << "max rectangle width variation must be >= 0.0 and <= 1.0" << endl;
		return 0;
	}

	cout << "loading " << fileName << endl;

	AudioFile<double> audioFile;
	audioFile.load(fileName);

	cout << "loaded file" << endl;
	audioFile.printSummary();

	for(int channel = 0; channel < audioFile.getNumChannels(); channel++)
	{
		cout << "parsing channel " << channel << endl;

		cout << "detecting zero crossings" << endl;
		ZeroCrossingDetector zcDetector;
		Crossings crossings;
		zcDetector.detect(audioFile.samples[channel], back_inserter(crossings));
		cout << "detected " << crossings.size() << " zero-crossings " << endl;

		cout << "detecting bits" << endl;
		BitDetector bitDetector;
		Bits bits;
		BitDetector::Errors bitErrors;

		bitDetector.detect(
			audioFile.samples[channel].size(),
			crossings,
			audioFile.getSampleRate(),
			maxWidthVariation,
			back_inserter(bits),
			back_inserter(bitErrors)
		);
		cout << "detected " << bits.size() << " bits, " << bitErrors.size() << " errors " << endl;
		for(const auto& e: bitErrors) {
			cout << "bit error: " << e.message << " at segment: " << e.segmentId << endl;
		}

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
		}
	}
	return 0;
}