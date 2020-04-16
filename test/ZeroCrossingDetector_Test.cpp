#include <vector>
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "ZeroCrossingDetector.cpp"

using namespace std;
using ::testing::StrictMock;

class ZeroCrossingDetectorTest : public testing::Test {
public:

    ZeroCrossingDetector objectUnderTest;
};

TEST_F(ZeroCrossingDetectorTest, detectsOneZeroCrossings)
{
	vector<double> testSamples;
	testSamples.push_back(-500);
	for(int i = 0; i < 100; i++) {
		testSamples.push_back(500);
	}

	vector<size_t> zeroCrosses;
    objectUnderTest.detect(testSamples.begin(), testSamples.end(), back_inserter(zeroCrosses));

    EXPECT_EQ(1, zeroCrosses.size());
}

TEST_F(ZeroCrossingDetectorTest, worksWithEmptyInputData)
{
	vector<double> testSamples;
	vector<size_t> zeroCrosses;
    objectUnderTest.detect(testSamples.begin(), testSamples.end(), back_inserter(zeroCrosses));

    EXPECT_EQ(0, zeroCrosses.size());
}

TEST_F(ZeroCrossingDetectorTest, detectsManyCrossings)
{
	vector<double> testSamples;
	vector<size_t> zeroCrosses;

	for(int i = 0; i < 100; i++) {
		testSamples.push_back(500 * (i % 2) ? (-1) : (1));
	}
	objectUnderTest.detect(testSamples.begin(), testSamples.end(), back_inserter(zeroCrosses));

    EXPECT_EQ(99, zeroCrosses.size());
}