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

    ASSERT_EQ(tags.planarConfiguration(), Tags::PLANARCONFIG_CONTIG);

    ASSERT_EQ(tags.software(), "");
    tags.software("Test software!");
    ASSERT_EQ(tags.software(), "Test software!");

    ASSERT_DOUBLE_EQ(tags.exposureTime(), 0.0);
    tags.exposureTime(3.0);
    ASSERT_DOUBLE_EQ(tags.exposureTime(), 3.0);

    ASSERT_DOUBLE_EQ(tags.fNumber(), 0.0);
    tags.fNumber(5.0);
    ASSERT_DOUBLE_EQ(tags.fNumber(), 5.0);

    ASSERT_EQ(tags.dateTime(), 0);
    tags.dateTime(1614632629000000);
    //TODO ASSERT_EQ(tags.dateTime(), 1614632629000000);

    ASSERT_DOUBLE_EQ(tags.subjectDistance(), 0.0);
    tags.subjectDistance(6.0);
    ASSERT_DOUBLE_EQ(tags.subjectDistance(), 6.0);

    ASSERT_EQ(tags.lightSource(), Tags::LIGHTSOURCE_UNKNOWN);
    tags.lightSource(Tags::LIGHTSOURCE_BLUELED);
    ASSERT_EQ(tags.lightSource(), Tags::LIGHTSOURCE_BLUELED);

    ASSERT_EQ(tags.flash(), Tags::FLASH_NO_FLASH);
    tags.flash(Tags::FLASH_FIRED);
    ASSERT_EQ(tags.flash(), Tags::FLASH_FIRED);

    ASSERT_DOUBLE_EQ(tags.focalLength(), 0.0);
    tags.focalLength(1.2);
    ASSERT_DOUBLE_EQ(tags.focalLength(), 1.2);

    ASSERT_EQ(tags.colourSpace(), Tags::COLOURSPACE_sRGB);
    tags.colourSpace(Tags::COLOURSPACE_ADOBE_RGB);
    ASSERT_EQ(tags.colourSpace(), Tags::COLOURSPACE_ADOBE_RGB);

    ASSERT_DOUBLE_EQ(tags.flashEnergy(), 0.0);
    tags.flashEnergy(67.0);
    ASSERT_DOUBLE_EQ(tags.flashEnergy(), 67.0);

    ASSERT_EQ(tags.serialNumber(), "");
    tags.serialNumber("Test serial number");
    ASSERT_EQ(tags.serialNumber(), "Test serial number");

    ASSERT_EQ(tags.lensModel(), "");
    tags.lensModel("Test lens model");
    ASSERT_EQ(tags.lensModel(), "Test lens model");

    ASSERT_DOUBLE_EQ(tags.indexOfRefraction(), Constants::DEFAULT_INDEX);
    tags.indexOfRefraction(1.345);
    ASSERT_DOUBLE_EQ(tags.indexOfRefraction(), 1.345);

    ASSERT_DOUBLE_EQ(tags.viewportIndex(), Constants::DEFAULT_VIEWPORT_INDEX);
    tags.viewportIndex(1.4);
    ASSERT_DOUBLE_EQ(tags.viewportIndex(), 1.4);

    ASSERT_DOUBLE_EQ(tags.viewportThickness(), 0);
    tags.viewportThickness(0.25);
    ASSERT_DOUBLE_EQ(tags.viewportThickness(), 0.25);

    ASSERT_DOUBLE_EQ(tags.viewportDistance(), 0);
    tags.viewportDistance(0.26);
    ASSERT_DOUBLE_EQ(tags.viewportDistance(), 0.26);

    ASSERT_FALSE(tags.vignetting());
    tags.vignetting(true);
    ASSERT_TRUE(tags.vignetting());

    ASSERT_EQ(tags.viewportType(), Tags::VIEWPORT_FLAT);
    tags.viewportType(Tags::VIEWPORT_DOMED);
    ASSERT_EQ(tags.viewportType(), Tags::VIEWPORT_DOMED);

    ASSERT_EQ(tags.enhancement(), Tags::ENHANCEMENT_NONE);
    tags.enhancement(Tags::ENHANCEMENT_BASIC_COLOUR);
    ASSERT_EQ(tags.enhancement(), Tags::ENHANCEMENT_BASIC_COLOUR);

    ASSERT_TRUE(tags.pixelSize().empty());
    const std::vector<uint16_t> pixel_vec = {3600, 3700};
    tags.pixelSize(pixel_vec);
    ASSERT_EQ(tags.pixelSize(), pixel_vec);

} 

}
}
