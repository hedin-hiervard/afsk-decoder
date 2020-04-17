#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "ByteDetector.h"

using namespace std;

class ByteDetectorTest : public testing::Test {
public:
    ByteDetector objectUnderTest;

    void printErrors(const ByteDetector::Errors& errors) {
        for(const auto& e: errors) {
            cout << "byte error: " << e.message << " at bit: " << e.position << endl;
            cout << "bit context: " << e.bitContext << endl;
        }
    }
};

TEST_F(ByteDetectorTest, detectsOneByte)
{
	ByteDetector::Errors errors;
	Bytes bytes;
    objectUnderTest.detect(
    	{ 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1 },
    	back_inserter(bytes),
    	back_inserter(errors)
    );

    EXPECT_EQ(0, errors.size());
    EXPECT_EQ(1, bytes.size());
    EXPECT_EQ(0x7f, bytes[0]);
    printErrors(errors);
}

TEST_F(ByteDetectorTest, detectsThreeByte)
{
    ByteDetector::Errors errors;
    Bytes bytes;
    objectUnderTest.detect(
        {
            0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1,
            0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1,
            0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1,
        },
        back_inserter(bytes),
        back_inserter(errors)
    );

    EXPECT_EQ(0, errors.size());
    EXPECT_EQ(3, bytes.size());
    EXPECT_EQ(0x7f, bytes[0]);
    EXPECT_EQ(0x7f, bytes[1]);
    EXPECT_EQ(0xbf, bytes[2]);
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

TEST_F(ByteDetectorTest, shouldNotEndTrailingZeroBytes)
{
    ByteDetector::Errors errors;
    Bytes bytes;
    objectUnderTest.detect(
        {
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        },
        back_inserter(bytes),
        back_inserter(errors)
    );

    EXPECT_EQ(1, bytes.size());
    EXPECT_EQ(0xaa, bytes[0]);
    EXPECT_EQ(1, errors.size()); // unexpected bitstream termination
}

TEST_F(ByteDetectorTest, shouldFindTwoBytesAfterMalformedLeader)
{
    ByteDetector::Errors errors;
    Bytes bytes;
    objectUnderTest.detect(
        {
            1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,
            0,0,1,0,0,0,0,1,0,1,1,
            0,1,1,0,0,0,0,0,0,1,1
        },
        back_inserter(bytes),
        back_inserter(errors)
    );

    EXPECT_EQ(2, bytes.size());
    EXPECT_EQ(0x42, bytes[0]);
    EXPECT_EQ(0x03, bytes[1]);
    printErrors(errors);
}
