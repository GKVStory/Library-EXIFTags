#pragma once
/**
 * Tags.h
 * 
 * Copyright 2G Robotics Inc., 2021
 * 
 * This class represents all the tags supported by the 2G exif library.
 * 
 */
#include "EXIFTags/Tag.h"
#include "EXIFTags/TagConstants.h"
#include <vector>
#include <memory>
#include <string>

namespace tg {
namespace tags{

/**
 * @brief This class wraps all of the 2G supported individual tags associated with a particular image file.
 */
class Tags {
public:
    /**
     * @brief constructor, generates an empty set of flags
     */
    Tags();
    virtual ~Tags();

    /**
     * @brief Given the contents of an image (or at least the header part of it), load the included tags.
     * @param image_header_data, vector of bytes containing at a minimum the image header data.
     * @param error emssage returned by reference in case of a failure.
     * @return bool was the load successful?
     */
    bool loadHeader(const std::vector <uint8_t> & image_header_data, std::string & error_message);

    /**
     * @brief Generate an EXIF header to be placed into an image based on the classes data.
     * @param vector of bytes, the image header data, returned by reference.
     * @param string an error message returned by reference when there is a failure.
     * @return bool was the header generation successful
     */
    bool generateHeader(std::vector <uint8_t> & image_header_data, std::string & error_message);

    ///--------------------------------------------------------------------
    /// Accessors and associate enums
    /// (Fixed fields have to setters)
    ///--------------------------------------------------------------------
    
    enum SubfileTypes {
        FULL_RESOLUTION_IMAGE = 0,
        REDUCED_RESOLUTION_IMAGE,
        PAGE_OF_MULTIPAGE
    };
    SubfileTypes subfileType () const;

    uint32_t imageWidth() const;
    void imageWidth(uint32_t width);

    uint32_t imageHeight() const;
    void imageHeight(uint32_t height); 

    enum CompressionType {
        COMPRESSION_NONE = 1,
        COMPRESSION_CCITTRLE = 2,
        COMPRESSION_CCITTFAX3 = 3,
        COMPRESSION_CCITTFAX4 = 4,
        COMPRESSION_LZW = 5,
        COMPRESSION_OJPEG = 6,
        COMPRESSION_JPEG = 7,
        COMPRESSION_NEXT = 32766,
        COMPRESSION_CCITTRLEW = 32771,
        COMPRESSION_PACKBITS = 32773,
        COMPRESSION_THUNDERSCAN = 32809,
        COMPRESSION_IT8CTPAD = 32895,
        COMPRESSION_IT8LW = 32896,
        COMPRESSION_IT8MP = 32897,
        COMPRESSION_IT8BL = 32898,
        COMPRESSION_PIXARFILM = 32908,
        COMPRESSION_PIXARLOG = 32909,
        COMPRESSION_DEFLATE = 32946,
        COMPRESSION_ADOBE_DEFLATE = 8,
        COMPRESSION_DCS = 32947,
        COMPRESSION_JBIG = 34661,
        COMPRESSION_SGILOG = 34676,
        COMPRESSION_SGILOG24 = 34677,
        COMPRESSION_JP2000 = 34712,
    };
    CompressionType compression() const;
    void compression (CompressionType compression);

    enum PhotometricInterpolationType {
        PHOTOMETRIC_MINISWHITE = 0,
        PHOTOMETRIC_MINISBLACK = 1,
        PHOTOMETRIC_RGB = 2,
        PHOTOMETRIC_PALETTE = 3,
        PHOTOMETRIC_MASK = 4,
        PHOTOMETRIC_SEPARATED = 5,
        PHOTOMETRIC_YCBCR = 6,
        PHOTOMETRIC_CIELAB = 8,
        PHOTOMETRIC_ICCLAB = 9,
        PHOTOMETRIC_ITULAB = 10,
        PHOTOMETRIC_LOGL = 32844,
        PHOTOMETRIC_LOGLUV = 32845,
    };
    PhotometricInterpolationType photometricInterpolation() const;
    void photometricInterpolation (PhotometricInterpolationType pi);

    std::string imageDescription() const;
    void imageDescription(const std::string & desc);

    std::string make() const;

    std::string model() const;
    void model(const std::string & model);

    enum OrientationType {
        ORIENTATION_TOPLEFT = 1,
        ORIENTATION_TOPRIGHT = 2,
        ORIENTATION_BOTRIGHT = 3,
        ORIENTATION_BOTLEFT = 4,
        ORIENTATION_LEFTTOP = 5,
        ORIENTATION_RIGHTTOP = 6,
        ORIENTATION_RIGHTBOT = 7,
        ORIENTATION_LEFTBOT = 8,
    };
    OrientationType orientation() const;

    uint16_t samplesPerPixel () const;
    void samplesPerPixel (uint16_t samples);

private:

    //storage for the different tags supported by 2G.
    std::vector <std::unique_ptr<Tag>> m_tags;

};

} //tags
} //tg