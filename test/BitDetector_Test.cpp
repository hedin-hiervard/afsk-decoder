#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "BitDetector.h"

using namespace std;

class BitDetectorTest : public testing::Test {
public:

    BitDetector objectUnderTest;
    static constexpr int samplesPerSecond = 44100;

    static constexpr double oneBitTimeInMicroseconds = 320;
    static constexpr double zeroBitTimeInMicroseconds = 640;

    static constexpr double oneBitTimeInSeconds = oneBitTimeInMicroseconds * 1e-6;
    static constexpr double zeroBitTimeInSeconds = zeroBitTimeInMicroseconds * 1e-6;

    static constexpr size_t samplesPerOneBit = samplesPerSecond * oneBitTimeInSeconds;
    static constexpr size_t samplesPerZeroBit = samplesPerSecond * zeroBitTimeInSeconds;
};

TEST_F(BitDetectorTest, detectsZeroOneZero)
{
	BitDetector::Errors errors;
	size_t currentSample = 0;

	Bits bits;
    objectUnderTest.detect(
    	10000,
    	{
    		(currentSample += samplesPerOneBit),
    		(currentSample += samplesPerZeroBit),
    		(currentSample += samplesPerOneBit),
    	},
    	samplesPerSecond,
    	0.1,
        0.1,
    	back_inserter(bits),
    	back_inserter(errors)
    );

    EXPECT_EQ(1, errors.size());
    EXPECT_EQ(3, bits.size());
    EXPECT_EQ(1, bits[0]);
    EXPECT_EQ(0, bits[1]);
    EXPECT_EQ(1, bits[2]);
}

TEST_F(BitDetectorTest, detectsZeroZeroZero)
{
	BitDetector::Errors errors;
	size_t currentSample = 0;

	Bits bits;
    objectUnderTest.detect(
    	10000,
    	{
    		(currentSample += samplesPerZeroBit),
    		(currentSample += samplesPerZeroBit),
    		(currentSample += samplesPerZeroBit),
    	},
    	samplesPerSecond,
    	0.1,
        0.1,
    	back_inserter(bits),
    	back_inserter(errors)
    );

    EXPECT_EQ(1, errors.size());
    EXPECT_EQ(3, bits.size());
    EXPECT_EQ(0, bits[0]);
    EXPECT_EQ(0, bits[1]);
    EXPECT_EQ(0, bits[2]);
}



TEST_F(BitDetectorTest, detectsFuzzyOneZeroOne)
{
	BitDetector::Errors errors;
	size_t currentSample = 0;

	Bits bits;
    objectUnderTest.detect(
    	10000,
    	{
    		(currentSample += (samplesPerOneBit - 1)),
    		(currentSample += (samplesPerZeroBit + 1)),
    		(currentSample += (samplesPerOneBit - 2))
    	},
    	samplesPerSecond,
    	0.2,
        0.2,
    	back_inserter(bits),
    	back_inserter(errors)
    );

    EXPECT_EQ(1, errors.size());
    EXPECT_EQ(3, bits.size());
    EXPECT_EQ(1, bits[0]);
    EXPECT_EQ(0, bits[1]);
    EXPECT_EQ(1, bits[2]);
}

TEST_F(BitDetectorTest, shouldNotDetectFuzzyWithZeroVariation)
{
	BitDetector::Errors errors;
	size_t currentSample = 0;

	Bits bits;
    objectUnderTest.detect(
    	10000,
    	{
    		(currentSample += (samplesPerOneBit - 1)),
    		(currentSample += (samplesPerZeroBit + 1)),
    		(currentSample += (samplesPerOneBit - 2))
    	},
    	samplesPerSecond,
    	0.0,
        0.0,
    	back_inserter(bits),
    	back_inserter(errors)
    );

    EXPECT_EQ(4, errors.size());
}
