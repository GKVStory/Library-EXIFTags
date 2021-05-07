/**
 * TestConstant.h
 * 
 * Copyright 2G Robotics Inc. 2021
 * 
 * This class is generated by cmake and allows the test code to find the test data.
 */

#pragma once

#include "EXIFTags/Tags.h"
#include <string>

namespace tg {
namespace tags {

class TagsTestCommon {
public:

    static std::string testDataDir() {
        return std::string("${PROJECT_SOURCE_DIR}/test_data/");
    };

    static std::string testJpgNon2g() {
        return testDataDir() + "gps_info.jpg";
    };

    static std::string testTifNon2g() {
        return testDataDir() + "exif.tif";
    };

    static std::string testTifOld2g() {
        return testDataDir() + "old2gdata.tif";
    };

    static std::string jpegOutputFile() {
        return testDataDir() + "testout.jpg";
    };

    static std::string tiffOutputFile() {
        return testDataDir() + "testout.tif";
    };

    static std::string OpenCVTiffInputFile() {
        return testDataDir() + "opencv_test.tif";
    };

    static std::string OpenCVTiffOutputFile() {
        return testDataDir() + "opencv_test_output.tif";
    };

    static void setTags (Tags & tags) {

    tags.imageHeight(1024);
    tags.imageWidth(2048);
    tags.compression(Tags::COMPRESSION_EXIF_NONE);
    tags.photometricInterpolation(Tags::PHOTOMETRIC_EXIF_RGB);
    tags.imageDescription("Test description!");
    tags.model("Test model!");
    tags.samplesPerPixel(3);
    tags.software("Test software!");
    tags.exposureTime(3.0);
    tags.fNumber(5.0);
    tags.dateTime(1614632629005001);
    tags.subjectDistance(6.0);
    tags.lightSource(Tags::LIGHTSOURCE_BLUELED);
    tags.flash(Tags::FLASH_FIRED);
    tags.focalLength(1.2);
    tags.colourSpace(Tags::COLOURSPACE_ADOBE_RGB);
    tags.flashEnergy(67.0);
    tags.serialNumber("Test serial number");
    tags.lensModel("Test lens model");
    tags.indexOfRefraction(1.345);
    tags.viewportIndex(1.4);
    tags.viewportThickness(0.25);
    tags.viewportDistance(0.26);
    tags.vignetting(true);
    tags.viewportType(Tags::VIEWPORT_DOMED);
    tags.enhancement(Tags::ENHANCEMENT_BASIC_COLOUR);
    const std::vector<uint16_t> pixel_vec = {3600, 3700};
    tags.pixelSize(pixel_vec);
    const std::vector<double> nav_vec = {1.0, 2.0, 3.0, 4.0, 5.0, 1.0, 6.0, 7.0, 8.0, 9.0, 1.0, 11.0, 12.0, 13.0, 14.0, 1.0};
    tags.matrixNavToCamera(nav_vec);
    tags.imageNumber(3);;
    tags.waterDepth(10.5);
    tags.bayerPattern(Tags::BAYER_BG2BGR);
    tags.frameRate(45.0);
    const std::vector<double> cam_vec = {2000.0, 2001.0, 1036, 738.2};
    tags.cameraMatrix(cam_vec);
    const std::vector<double> dist_vec = {-0.1, 0.2, -0.3, 0.5, -0.5};
    tags.distortion(dist_vec);
    const std::vector<double> pose_vec = {1.0, 2.3, 3.4};
    tags.pose(pose_vec);
    tags.vehicleAltitude(10.87);
    tags.latitudeRef(Tags::LATITUDEREF_SOUTH);;
    tags.latitude(83.1245678);
    tags.longitudeRef(Tags::LONGITUDEREF_WEST);
    tags.longitude(43.67890);
    tags.altitudeRef(Tags::ALTITUDEREF_BELOW_SEA_LEVEL);
    tags.altitude(10.765);  
    tags.ppsTime(1614632629005011);
};

    static void testTags (const Tags & tags) {
    const std::vector<uint16_t> pixel_vec = {3600, 3700};
    const std::vector<double> nav_vec = {1.0, 2.0, 3.0, 4.0, 5.0, 1.0, 6.0, 7.0, 8.0, 9.0, 1.0, 11.0, 12.0, 13.0, 14.0, 1.0};
    const std::vector<double> cam_vec = {2000.0, 2001.0, 1036, 738.2};
    const std::vector<double> dist_vec = {-0.1, 0.2, -0.3, 0.5, -0.5};
    const std::vector<double> pose_vec = {1.0, 2.3, 3.4};
    ASSERT_EQ(tags.subfileType(), Tags::FULL_RESOLUTION_IMAGE);
    ASSERT_EQ(tags.imageDescription(), "Test description!");
    ASSERT_EQ(tags.make(), Constants::DEFAULT_MAKE);
    ASSERT_EQ(tags.model(), "Test model!");
    ASSERT_EQ(tags.orientation(), Tags::ORIENTATION_EXIF_TOPLEFT);
    ASSERT_EQ(tags.planarConfiguration(), Tags::PLANARCONFIG_EXIF_CONTIG);
    ASSERT_EQ(tags.software(), "Test software!");
    ASSERT_DOUBLE_EQ(tags.exposureTime(), 3.0);
    ASSERT_DOUBLE_EQ(tags.fNumber(), 5.0);
    ASSERT_EQ(tags.dateTime(), 1614632629005001);
    ASSERT_DOUBLE_EQ(tags.subjectDistance(), 6.0);
    ASSERT_EQ(tags.lightSource(), Tags::LIGHTSOURCE_BLUELED);
    ASSERT_EQ(tags.flash(), Tags::FLASH_FIRED);
    ASSERT_DOUBLE_EQ(tags.focalLength(), 1.2);
    ASSERT_DOUBLE_EQ(tags.flashEnergy(), 67.0);
    ASSERT_EQ(tags.serialNumber(), "Test serial number");
    ASSERT_EQ(tags.lensModel(), "Test lens model");
     
    ASSERT_DOUBLE_EQ(tags.indexOfRefraction(), 1.345);
    ASSERT_DOUBLE_EQ(tags.viewportIndex(), 1.4);
    ASSERT_DOUBLE_EQ(tags.viewportThickness(), 0.25);
    ASSERT_DOUBLE_EQ(tags.viewportDistance(), 0.26);
    ASSERT_TRUE(tags.vignetting());
    ASSERT_EQ(tags.viewportType(), Tags::VIEWPORT_DOMED);
    ASSERT_EQ(tags.enhancement(), Tags::ENHANCEMENT_BASIC_COLOUR);
    ASSERT_EQ(tags.pixelSize(), pixel_vec);
    ASSERT_EQ(tags.matrixNavToCamera(), nav_vec);
    ASSERT_EQ(tags.imageNumber(), 3);
    ASSERT_DOUBLE_EQ(tags.waterDepth(), 10.5);
    ASSERT_EQ(tags.bayerPattern(), Tags::BAYER_BG2BGR);
    ASSERT_DOUBLE_EQ(tags.frameRate(), 45.0);
    ASSERT_EQ(tags.cameraMatrix(), cam_vec);
    ASSERT_EQ(tags.distortion(), dist_vec);
    ASSERT_EQ(tags.pose(), pose_vec);
    ASSERT_DOUBLE_EQ(tags.vehicleAltitude(), 10.87);
    
    ASSERT_EQ(tags.latitudeRef(), Tags::LATITUDEREF_SOUTH);
    ASSERT_NEAR(tags.latitude(), 83.1245678, 0.000001);
    ASSERT_EQ(tags.longitudeRef(), Tags::LONGITUDEREF_WEST);
    ASSERT_NEAR(tags.longitude(), 43.67890, 0.0000001);
    ASSERT_EQ(tags.altitudeRef(), Tags::ALTITUDEREF_BELOW_SEA_LEVEL);
    ASSERT_NEAR(tags.altitude(), 10.765, 0.0000001);
    ASSERT_EQ(tags.ppsTime(), 1614632629005011); //This does not work with standard tags. Has to be done with arrays and buffers. See reasoning behind the altitude gps data.
};

};


} //tags
} //tg