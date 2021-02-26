//TestTags.cpp
//Copyright 2G Robotics Inc., 2021

#include <gtest/gtest.h>
#include "EXIFTags/Tags.h"
#include <cstdint>
#include <string>

namespace tg {
namespace tags {

TEST ( TagsTest, GenerateEmptyTagsClass_CheckDefaultBehaviour) {
    Tags tags;

    ASSERT_EQ(tags.subfileType(), Tags::FULL_RESOLUTION_IMAGE);

    ASSERT_EQ(tags.imageHeight(), 0);
    tags.imageHeight(1024);
    ASSERT_EQ(tags.imageHeight(), 1024);

    ASSERT_EQ(tags.imageWidth(), 0);
    tags.imageWidth(2048);
    ASSERT_EQ(tags.imageWidth(), 2048);

    ASSERT_EQ(tags.compression(), Tags::COMPRESSION_NONE);
    tags.compression(Tags::COMPRESSION_JPEG);
    ASSERT_EQ(tags.compression(), Tags::COMPRESSION_JPEG);

    ASSERT_EQ(tags.photometricInterpolation(), Tags::PHOTOMETRIC_MINISBLACK);
    tags.photometricInterpolation(Tags::PHOTOMETRIC_RGB);
    ASSERT_EQ(tags.photometricInterpolation(), Tags::PHOTOMETRIC_RGB);

    ASSERT_EQ(tags.imageDescription(), "");
    tags.imageDescription("Test description!");
    ASSERT_EQ(tags.imageDescription(), "Test description!");

    ASSERT_EQ(tags.make(), Constants::DEFAULT_MAKE);

    ASSERT_EQ(tags.model(), "");
    tags.model("Test model!");
    ASSERT_EQ(tags.model(), "Test model!");

    ASSERT_EQ(tags.orientation(), Tags::ORIENTATION_TOPLEFT);

    ASSERT_EQ(tags.samplesPerPixel(), 1);
    tags.samplesPerPixel(3);
    ASSERT_EQ(tags.samplesPerPixel(), 3);
} 


}
}
