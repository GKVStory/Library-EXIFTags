#pragma once
/**
 * TagConstants.h
 * 
 * Copyright 2G Robotics Inc., 2021
 * 
 * This file contains a bunch of constants associated with the 2G Exif tag library.
 */
#include <libexif/exif-data.h>
#include <vector>

namespace tg {
namespace tags {

class Constants {
public:

enum DataType {
    UINT32,
    UINT16,
    UINT8,
    DOUBLE,
    STRING,
    UINT16_ARRAY,
    DOUBLE_ARRAY,
};

enum SupportedTags {
        SUBFILE_TYPE,
        IMAGE_WIDTH,
        IMAGE_HEIGHT,
        COMPRESSION,
        PHOTOMETRIC_INTERPOLATION,
        IMAGE_DESCRIPTION,
        MAKE,
        MODEL,
        ORIENTATION,
        SAMPLES_PER_PIXEL,
        PLANAER_CONFIGURATION,
        SOFTWARE,
        EXPOSURE_TIME,
        F_NUMBER,
        //GPS_INFO,
        DATE_TIME_ORIGINAL,
        SUB_SEC_ORIGINAL,
        SUBJECT_DISTANCE,
        LIGHT_SOURCE,
        FLASH,
        FOCAL_LENGTH,
        MAKER_NOTE_2GR,
        COLOR_SPACE,
        FLASH_ENERGY,
        SERIAL_NUMBER,
        LENS_MODEL,
    
        //MakerNote Tags
        INDEX_OF_REFRACTION,
        VIEWPORT_INDEX,
        VIEWPORT_THICKNESS,
        VIEWPORT_DISTANCE,
        VIGNETTING,
        VIEWPORT_TYPE,
        ENAHNCEMENT_TYPE,
        PIXEL_SIZE,
        MATRIX_NAV_TO_CAMERA,
        IMAGE_NUMBER,
        WATER_DEPTH,
        BAYER_PATTERN,
        FRAME_RATE,
        CAMERA_MATRIX,
        DISTORTION,

        //GPSTags
        GPS_LATITUDE_REF,
        GPS_LATITUDE,
        GPS_LONGITUDE_REF,
        GPS_LONGITUDE,
        GPS_ALTITUDE_REF,
        GPS_ALTITUDE,
        GPS_TIMESTAMP,
        GPS_DATESTAMP,
        LENGTH_SUPPORTED_TAGS
    };

/**
 * Structure for holding tag data.
 */
struct TagInfo {
public:
    uint16_t tag; //EXIF Tag
    ExifIfd ifd;
    size_t len; //length in bytes. 0 is variable
    DataType data_type;
    bool custom;

    TagInfo (uint16_t tag, ExifIfd ifd, size_t len, DataType data_type, bool custom) :
        tag (tag),
        ifd(ifd),
        len(len),
        data_type(data_type),
        custom (custom) {}; 
};

static const std::vector <TagInfo> TAG_INFO;
static const std::string DEFAULT_MAKE;

};

} //tags
} //tg