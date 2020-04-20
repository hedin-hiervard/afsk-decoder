#include <fstream>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "AfskDecoder.h"

using namespace std;

class AfskDecoderTest : public testing::Test {
public:
    AfskDecoder objectUnderTest;
    std::string decodedMessages;
private:
    void SetUp() {
        std::ifstream t("data/decoded.txt");
        std::stringstream buffer;
        buffer << t.rdbuf();
        decodedMessages = buffer.str();
    }
};


TEST_F(AfskDecoderTest, decodesFile1)
{
    string output;
    objectUnderTest.decode(
        move("data/file_1.wav"),
        {
            .zeroBitRangeInMicroseconds = { 600, 680 },
            .oneBitRangeInMicroseconds = { 220, 360 },
        },
        3,
        nullptr,
        nullptr,
        [&output](std::string msg) { output += msg; },
        nullptr
    );

    EXPECT_EQ(decodedMessages, output);
}

TEST_F(AfskDecoderTest, decodesFile2)
{
    string output;
    objectUnderTest.decode(
        move("data/file_2.wav"),
        {
            .zeroBitRangeInMicroseconds = { 600, 680 },
            .oneBitRangeInMicroseconds = { 220, 360 },
        },
        3,
        nullptr,
        nullptr,
        [&output](std::string msg) { output += msg; },
        nullptr
    );

    EXPECT_EQ(decodedMessages, output);
}

TEST_F(AfskDecoderTest, decodesFile3)
{
    string output;
    objectUnderTest.decode(
        move("data/file_2.wav"),
        {
            .zeroBitRangeInMicroseconds = { 600, 680 },
            .oneBitRangeInMicroseconds = { 220, 360 },
        },
        3,
        nullptr,
        nullptr,
        [&output](std::string msg) { output += msg; },
        nullptr
    );

    EXPECT_EQ(decodedMessages, output);
}
