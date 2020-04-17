#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "ZeroCrossingDetector.h"

using namespace std;

class ZeroCrossingDetectorTest : public testing::Test {
public:

    ZeroCrossingDetector objectUnderTest;
};

TEST_F(ZeroCrossingDetectorTest, detectsOneZeroCrossings)
{
	Samples testSamples;
	testSamples.push_back(-500);
	for(int i = 0; i < 100; i++) {
		testSamples.push_back(500);
	}

	Crossings crossings;
    objectUnderTest.detect(testSamples, back_inserter(crossings));

    EXPECT_EQ(1, crossings.size());
}

TEST_F(ZeroCrossingDetectorTest, worksWithEmptyInputData)
{
	Samples testSamples;
	Crossings crossings;
    objectUnderTest.detect(testSamples, back_inserter(crossings));

    EXPECT_EQ(0, crossings.size());
}

TEST_F(ZeroCrossingDetectorTest, detectsManyCrossings)
{
	Samples testSamples;
	Crossings crossings;

	for(int i = 0; i < 100; i++) {
		testSamples.push_back(500 * (i % 2) ? (-1) : (1));
	}
	objectUnderTest.detect(testSamples, back_inserter(crossings));

    EXPECT_EQ(99, crossings.size());
}