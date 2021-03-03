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
    tags.dateTime(1614632629005001);
    ASSERT_EQ(tags.dateTime(), 1614632629005001);

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

    ASSERT_TRUE(tags.matrixNavToCamera().size() == 16);
    ASSERT_EQ(tags.matrixNavToCamera(), Constants::DEFAULT_TRANSFORM);
    const std::vector<double> nav_vec = {1.0, 2.0, 3.0, 4.0, 5.0, 1.0, 6.0, 7.0, 8.0, 9.0, 1.0, 11.0, 12.0, 13.0, 14.0, 1.0};
    tags.matrixNavToCamera(nav_vec);
    ASSERT_EQ(tags.matrixNavToCamera(), nav_vec);

    ASSERT_EQ(tags.imageNumber(), 0);
    tags.imageNumber(3);
    ASSERT_EQ(tags.imageNumber(), 3);

    ASSERT_DOUBLE_EQ(tags.waterDepth(), 0);
    tags.waterDepth(10.5);
    ASSERT_DOUBLE_EQ(tags.waterDepth(), 10.5);

    ASSERT_EQ(tags.bayerPattern(), Tags::BAYER_GRAYSCALE);
    tags.bayerPattern(Tags::BAYER_BG2BGR);
    ASSERT_EQ(tags.bayerPattern(), Tags::BAYER_BG2BGR);

    ASSERT_DOUBLE_EQ(tags.frameRate(), 0.0);
    tags.frameRate(45.0);
    ASSERT_DOUBLE_EQ(tags.frameRate(), 45.0);

    ASSERT_TRUE(tags.cameraMatrix().size() == 4);
    ASSERT_EQ(tags.cameraMatrix(), Constants::DEFAULT_CAM_MATRIX);
    const std::vector<double> cam_vec = {2000.0, 2001.0, 1036, 738.2};
    tags.cameraMatrix(cam_vec);
    ASSERT_EQ(tags.cameraMatrix(), cam_vec);

    ASSERT_TRUE(tags.distortion().size() == 5);
    ASSERT_EQ(tags.distortion(), Constants::DEFAULT_DISTORTION);
    const std::vector<double> dist_vec = {-0.1, 0.2, -0.3, 0.5, -0.5};
    tags.distortion(dist_vec);
    ASSERT_EQ(tags.distortion(), dist_vec);

    ASSERT_TRUE(tags.pose().size() == 3);
    ASSERT_EQ(tags.pose(), Constants::DEFAULT_POSE);
    const std::vector<double> pose_vec = {1.0, 2.3, 3.4};
    tags.pose(pose_vec);
    ASSERT_EQ(tags.pose(), pose_vec);

    ASSERT_DOUBLE_EQ(tags.targetRange(), 0.0);
    tags.targetRange(5.1);
    ASSERT_DOUBLE_EQ(tags.targetRange(), 5.1);

    ASSERT_EQ(tags.latitudeRef(), Tags::LATITUDEREF_NORTH);
    tags.latitudeRef(Tags::LATITUDEREF_SOUTH);
    ASSERT_EQ(tags.latitudeRef(), Tags::LATITUDEREF_SOUTH);

    ASSERT_DOUBLE_EQ(tags.latitude(), 0.0);
    tags.latitude(83.1245678);
    ASSERT_DOUBLE_EQ(tags.latitude(), 83.1245678);

    ASSERT_EQ(tags.longitudeRef(), Tags::LONGITUDEREF_EAST);
    tags.longitudeRef(Tags::LONGITUDEREF_WEST);
    ASSERT_EQ(tags.longitudeRef(), Tags::LONGITUDEREF_WEST);

    ASSERT_DOUBLE_EQ(tags.longitude(), 0.0);
    tags.longitude(43.67890);
    ASSERT_DOUBLE_EQ(tags.longitude(), 43.67890);

    ASSERT_EQ(tags.altitudeRef(), Tags::ALTITUDEREF_ABOVE_SEA_LEVEL);
    tags.altitudeRef(Tags::ALTITUDEREF_BELOW_SEA_LEVEL);
    ASSERT_EQ(tags.altitudeRef(), Tags::ALTITUDEREF_BELOW_SEA_LEVEL);

    ASSERT_DOUBLE_EQ(tags.altitude(), 0.0);
    tags.altitude(10.765);
    ASSERT_DOUBLE_EQ(tags.altitude(), 10.765);


} 

}
}
