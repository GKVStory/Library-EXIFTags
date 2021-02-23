#pragma once
/**
 * Tags.h
 * 
 * Copyright 2G Robotics Inc., 2021
 * 
 * This class represents all the tags supported by the 2G exif library.
 * 
 */
#include <EXIFTags/Tag.h>

namespace tg {
namespace tags{

class Tags {
public:

private:

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
        GPS_INFO,
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
        LENS_MODEL
    
        //MakerNote Tags
        INDEX_OF_REFRACTION,
        VIEWPORT_INDEX,
        VIEWPORT_THICKNESS,
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
    }

};

} //tags
} //tg