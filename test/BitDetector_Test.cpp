#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "BitDetector.h"

using namespace std;

class BitDetectorTest : public testing::Test {
public:

    BitDetector objectUnderTest;
    static constexpr int samplesPerSecond = 44100;
    static constexpr double oneBitTimeInSeconds = 320e-6;
    static constexpr double zeroBitTimeInSeconds = 2 * oneBitTimeInSeconds;
    static constexpr size_t samplesPerOneBit = samplesPerSecond * oneBitTimeInSeconds;
    static constexpr size_t samplesPerZeroBit = samplesPerSecond * zeroBitTimeInSeconds;
};

TEST_F(BitDetectorTest, detectsZeroOneZero)
{
	Crossings testCrossings;
	BitDetector::Errors errors;

	size_t currentSample = 0;

	currentSample += samplesPerOneBit;
	testCrossings.push_back(currentSample);

	currentSample += samplesPerZeroBit;
	testCrossings.push_back(currentSample);

	currentSample += samplesPerOneBit;
	testCrossings.push_back(currentSample);

	Bits bits;
    objectUnderTest.detect(
    	testCrossings,
    	samplesPerSecond,
    	0.1,
    	back_inserter(bits),
    	back_inserter(errors)
    );

    EXPECT_EQ(0, errors.size());
    EXPECT_EQ(3, bits.size());
}
