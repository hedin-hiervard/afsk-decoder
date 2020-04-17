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
    	{
            0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1
        },
    	back_inserter(bytes),
    	back_inserter(errors)
    );

    EXPECT_EQ(0, errors.size());
    EXPECT_EQ(2, bytes.size());
    EXPECT_EQ(0x7f, bytes[1]);
    printErrors(errors);
}

TEST_F(ByteDetectorTest, detectsThreeByte)
{
    ByteDetector::Errors errors;
    Bytes bytes;
    objectUnderTest.detect(
        {
            0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1,
            0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1,
            0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1,
        },
        back_inserter(bytes),
        back_inserter(errors)
    );

    EXPECT_EQ(0, errors.size());
    EXPECT_EQ(4, bytes.size());
    EXPECT_EQ(0x7f, bytes[1]);
    EXPECT_EQ(0x7f, bytes[2]);
    EXPECT_EQ(0xbf, bytes[3]);
}

TEST_F(ByteDetectorTest, shouldFindTwoBytesAfterLeader)
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

    EXPECT_EQ(0x42, bytes[bytes.size() - 2]);
    EXPECT_EQ(0x03, bytes[bytes.size() - 1]);
    printErrors(errors);
}

TEST_F(ByteDetectorTest, shouldFindTwoBytesAfterLeader2)
{
    ByteDetector::Errors errors;
    Bytes bytes;
    objectUnderTest.detect(
        {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,
            0,0,1,0,0,0,0,1,0,1,1,
            0,1,1,0,0,0,0,0,0,1,1

        },
        back_inserter(bytes),
        back_inserter(errors)
    );

    EXPECT_EQ(0xff, bytes.front());
    EXPECT_EQ(0x42, bytes[bytes.size() - 2]);
    EXPECT_EQ(0x42, bytes[bytes.size() - 2]);
    EXPECT_EQ(0x03, bytes[bytes.size() - 1]);

    printErrors(errors);
}

