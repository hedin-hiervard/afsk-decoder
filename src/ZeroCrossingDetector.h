#pragma once

#include "../lib/audiofile/AudioFile.h"

class ZeroCrossingDetector {

public:
	ZeroCrossingDetector(AudioFile<double>&& audioFile);
	void detect();

private:
	AudioFile<double> m_audioFile;
};

