//TagConstants.cpp
//Copyright 2G Robotics Inc., 2021
#include "EXIFTags/TagConstants.h"

using namespace tg;
using namespace tags;

const std::vector<Constants::TagInfo> Constants::TAG_INFO {
    TagInfo(EXIF_TAG_NEW_SUBFILE_TYPE, EXIF_IFD_0, sizeof(uint32_t), UINT32, false), //SUBFILE_TYPE,
    TagInfo(EXIF_TAG_IMAGE_WIDTH, EXIF_IFD_0, sizeof(uint16_t), UINT16, false), //IMAGE_WIDTH,
    TagInfo(EXIF_TAG_IMAGE_LENGTH, EXIF_IFD_0, sizeof(uint16_t), UINT16, false), //IMAGE_HEIGHT,
                            //EXIF_TAG_BITS_PER_SAMPLE?
    TagInfo(EXIF_TAG_COMPRESSION, EXIF_IFD_0, sizeof(uint16_t), UINT16, false), //COMPRESSION,
    TagInfo(EXIF_TAG_PHOTOMETRIC_INTERPRETATION, EXIF_IFD_0, sizeof(uint16_t), UINT16, false), //PHOTOMETRIC_INTERPOLATION,
    TagInfo(EXIF_TAG_IMAGE_DESCRIPTION, EXIF_IFD_0, 0, STRING, false), //IMAGE_DESCRIPTION,
    TagInfo(EXIF_TAG_MAKE, EXIF_IFD_0, 0, STRING, false), //MAKE,
    TagInfo(EXIF_TAG_MODEL, EXIF_IFD_0, 0, STRING, false), //MODEL,
    TagInfo(EXIF_TAG_ORIENTATION, EXIF_IFD_0, sizeof(uint16_t), UINT16, false), //ORIENTATION,
    TagInfo(EXIF_TAG_SAMPLES_PER_PIXEL, EXIF_IFD_0, sizeof(uint16_t), UINT16, false), //SAMPLES_PER_PIXEL,
    TagInfo(EXIF_TAG_PLANAR_CONFIGURATION, EXIF_IFD_0, sizeof(uint16_t), UINT16, false), //PLANAER_CONFIGURATION,
    TagInfo(EXIF_TAG_SOFTWARE, EXIF_IFD_0, 0, STRING, false), //SOFTWARE,
    TagInfo(EXIF_TAG_EXPOSURE_TIME, EXIF_IFD_EXIF, sizeof(double), UDOUBLE, false), //EXPOSURE_TIME,
    TagInfo(EXIF_TAG_FNUMBER, EXIF_IFD_EXIF, sizeof(double), UDOUBLE, false), //F_NUMBER,
    //(EXIF_TAG_GPS_INFO_IFD_POINTER, 1), //GPS_INFO,
    TagInfo(EXIF_TAG_DATE_TIME_ORIGINAL, EXIF_IFD_EXIF, 0, STRING, false), //DATE_TIME_ORIGINAL,
    TagInfo(EXIF_TAG_SUB_SEC_TIME_ORIGINAL, EXIF_IFD_EXIF, 0, STRING, false), //SUB_SEC_ORIGINAL,
    TagInfo(EXIF_TAG_SUBJECT_DISTANCE, EXIF_IFD_EXIF, sizeof(double), UDOUBLE, false), //SUBJECT_DISTANCE,
    TagInfo(EXIF_TAG_LIGHT_SOURCE, EXIF_IFD_EXIF, sizeof(uint16_t), UINT16, false), //LIGHT_SOURCE,
    TagInfo(EXIF_TAG_FLASH, EXIF_IFD_EXIF, sizeof(uint16_t), UINT16, false), //FLASH,
    TagInfo(EXIF_TAG_FOCAL_LENGTH, EXIF_IFD_EXIF, sizeof(double), UDOUBLE, false), //FOCAL_LENGTH,
    TagInfo(EXIF_TAG_MAKER_NOTE, EXIF_IFD_EXIF, 0, STRING, false), //MAKER_NOTE_2GR,
    TagInfo(EXIF_TAG_COLOR_SPACE, EXIF_IFD_EXIF, sizeof(uint16_t), UINT16, false), //COLOR_SPACE,
    TagInfo(EXIF_TAG_PIXEL_X_DIMENSION, EXIF_IFD_EXIF, sizeof(uint16_t), UINT16, false), //PIXEL_X_DIMENSION,
    TagInfo(EXIF_TAG_PIXEL_Y_DIMENSION, EXIF_IFD_EXIF, sizeof(uint16_t), UINT16, false), //PIXEL_Y_DIMENSION,
    TagInfo(EXIF_TAG_FLASH_ENERGY, EXIF_IFD_EXIF, sizeof(double), UDOUBLE, false), //FLASH_ENERGY,
    TagInfo(EXIF_TAG_BODY_SERIAL_NUMBER, EXIF_IFD_EXIF, 0, STRING, false), //SERIAL_NUMBER,
    TagInfo(EXIF_TAG_LENS_MODEL, EXIF_IFD_EXIF, 0, STRING, false), //LENS_MODEL
    ////MakerNote Tags
    TagInfo(0x0000, EXIF_IFD_0, sizeof(double), UDOUBLE, true), //INDEX_OF_REFRACTION,
    TagInfo(0x0001, EXIF_IFD_0, sizeof(double), UDOUBLE, true), //VIEWPORT_INDEX,
    TagInfo(0x0002, EXIF_IFD_0, sizeof(double), UDOUBLE, true), //VIEWPORT_THICKNESS,
    TagInfo(0x0003, EXIF_IFD_0, sizeof(double), UDOUBLE, true), //VIEWPORT_DISTANCE,
    TagInfo(0x0004, EXIF_IFD_0, sizeof(uint16_t), UINT16, true), //VIGNETTING,
    TagInfo(0x0005, EXIF_IFD_0, sizeof(uint16_t), UINT16, true), //VIEWPORT_TYPE,
    TagInfo(0x0006, EXIF_IFD_0, sizeof(uint16_t), UINT16, true), //ENAHNCEMENT_TYPE,
    TagInfo(0x0007, EXIF_IFD_0, sizeof(uint16_t) * 2, UINT16_ARRAY, true), //PIXEL_SIZE,
    TagInfo(0x0008, EXIF_IFD_0, sizeof(double) * 16, DOUBLE_ARRAY, true),//MATRIX_NAV_TO_CAMERA,
    TagInfo(0x0009, EXIF_IFD_0, sizeof(uint32_t), UINT32, true), //IMAGE_NUMBER,
    TagInfo(0x000a, EXIF_IFD_0, sizeof(double), DOUBLE, true), //WATER_DEPTH,
    TagInfo(0x000b, EXIF_IFD_0, sizeof(uint16_t), UINT16, true), //BAYER_PATTERN,
    TagInfo(0x000c, EXIF_IFD_0, sizeof(double), UDOUBLE, true), //FRAME_RATE,
    TagInfo(0x000d, EXIF_IFD_0, sizeof(double) * 4, DOUBLE_ARRAY, true), //CAMERA_MATRIX,
    TagInfo(0x000e, EXIF_IFD_0, sizeof(double) * 5, DOUBLE_ARRAY, true), //DISTORTION,
    TagInfo(0x000f, EXIF_IFD_0, sizeof(double) * 3, DOUBLE_ARRAY, true), //POSE,
    TagInfo(0x0010, EXIF_IFD_0, sizeof(double), UDOUBLE, true), //VEHICLE_ALTITUDE,
    ////GPSTags
    TagInfo(EXIF_TAG_GPS_LATITUDE_REF, EXIF_IFD_GPS, 2, STRING, false), //GPS_LATITUDE_REF, N/S
    TagInfo(EXIF_TAG_GPS_LATITUDE, EXIF_IFD_GPS, sizeof(double) * 3, UDOUBLE_ARRAY, false), //GPS_LATITUDE,
    TagInfo(EXIF_TAG_GPS_LONGITUDE_REF, EXIF_IFD_GPS, 2, STRING, false), //GPS_LONGITUDE_REF, E/W
    TagInfo(EXIF_TAG_GPS_LONGITUDE, EXIF_IFD_GPS,  sizeof(double) * 3, UDOUBLE_ARRAY, false), //GPS_LONGITUDE,
    TagInfo(EXIF_TAG_GPS_ALTITUDE_REF, EXIF_IFD_GPS, sizeof(uint8_t), UINT8, false), //GPS_ALTITUDE_REF, 0 above, 1 below
    TagInfo(EXIF_TAG_GPS_ALTITUDE, EXIF_IFD_GPS, sizeof(double) * 1, UDOUBLE_ARRAY, false), //GPS_ALTITUDE, Making this an array is a hack to fix libexif's poor support of writing GPS tags.

    TagInfo(65000, EXIF_IFD_0, sizeof(uint16_t), UINT32, false), //PPS_TIME_UPPER.
    TagInfo(65001, EXIF_IFD_0, sizeof(uint16_t), UINT32, false), //PPS_TIME_LOWER,
    
};

double Constants::DMSToDeg (double degrees, double minutes, double seconds) {
    return degrees + minutes / 60.0 + seconds / 3600.0;
}

void Constants::degToDMS (double & degrees, double & minutes, double & seconds, double decdeg) {
    degrees = static_cast <double> ( static_cast<int>(decdeg) );
    minutes = static_cast<double> ( static_cast <int>((decdeg - degrees) * 60.0) ); 
    seconds = static_cast<double> ((decdeg - degrees - minutes / 60.0) * 3600.0);
}

//Constant values.
const std::string Constants::DEFAULT_MAKE = "2G Robotics";
const double Constants::DEFAULT_INDEX = 1.34;
const double Constants::DEFAULT_VIEWPORT_INDEX = 1.7;
const std::vector <double> Constants::DEFAULT_TRANSFORM = {1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0};;
const std::vector <double> Constants::DEFAULT_CAM_MATRIX = {500.0, 500.0, 1024.0, 1024.0};
const std::vector <double> Constants::DEFAULT_DISTORTION = {0.0, 0.0, 0.0, 0.0, 0.0};
const std::vector <double> Constants::DEFAULT_POSE = {0.0, 0.0, 0.0};
const std::string Constants::DEFAULT_TIMESTAMP_FORMAT = "%Y-%m-%dT%H:%M:%S";
const ExifByteOrder Constants::DEFAULT_BYTE_ORDER = EXIF_BYTE_ORDER_INTEL;

//Error messages
const std::string ErrorMessages::failed_header_load = "Failed to load header.";
const std::string ErrorMessages::failed_file_load = "Failed to load file: ";
const std::string ErrorMessages::file_too_small = "File is too small to be an image file: ";
const std::string ErrorMessages::memory_error = "Unable to allocate memory.";
