#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "ByteDetector.h"

using namespace std;

class ByteDetectorTest : public testing::Test {
public:
    ByteDetector objectUnderTest;
};

TEST_F(ByteDetectorTest, detectsOneByte)
{
	ByteDetector::Errors errors;
	Bytes bytes;
    objectUnderTest.detect(
    	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
    	back_inserter(bytes),
    	back_inserter(errors)
    );

    EXPECT_EQ(0, errors.size());
    EXPECT_EQ(1, bytes.size());
    EXPECT_EQ(0xff, bytes[0]);
}

TEST_F(ByteDetectorTest, detectsThreeByte)
{
    ByteDetector::Errors errors;
    Bytes bytes;
    objectUnderTest.detect(
        {
            0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        },
        back_inserter(bytes),
        back_inserter(errors)
    );

    EXPECT_EQ(0, errors.size());
    EXPECT_EQ(3, bytes.size());
    EXPECT_EQ(0xff, bytes[0]);
    EXPECT_EQ(0xff, bytes[1]);
    EXPECT_EQ(0xff, bytes[2]);
}

TEST_F(ByteDetectorTest, skipsLeadingTone)
{
    ByteDetector::Errors errors;
    Bytes bytes;
    objectUnderTest.detect(
        {
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1,
        },
        back_inserter(bytes),
        back_inserter(errors)
    );

    EXPECT_EQ(0, errors.size());
    EXPECT_EQ(1, bytes.size());
    EXPECT_EQ(0xaa, bytes[0]);
}

TEST_F(ByteDetectorTest, skipsLeadinAndTrailingTones)
{
    ByteDetector::Errors errors;
    Bytes bytes;
    objectUnderTest.detect(
        {
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1,
        },
        back_inserter(bytes),
        back_inserter(errors)
    );

    EXPECT_EQ(0, errors.size());
    EXPECT_EQ(1, bytes.size());
    EXPECT_EQ(0xaa, bytes[0]);
}
