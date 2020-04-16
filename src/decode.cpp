#include <iostream>
#include "audiofile/AudioFile.h"
#include "ZeroCrossingDetector.h"

using namespace std;

auto main(int argc, char** argv) -> int {
	if(argc < 2) {
		cout << "usage: decode <wav-file>" << endl;
		return 0;
	}
	const std::string fileName = argv[1];
	cout << "loading " << fileName << endl;

	AudioFile<double> audioFile;
	audioFile.load(fileName);

	cout << "loaded file" << endl;
	audioFile.printSummary();

	for(int channel = 0; channel < audioFile.getNumChannels(); channel++)
	{
		cout << "parsing channel " << channel << endl;
		// ZeroCrossingDetector zcDetector;
		// zcDetector.parse(audioFile.sample)
	}
	//zcDetector.detect();

	return 0;
}