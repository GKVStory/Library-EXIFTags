#pragma once
/**
 * Tags.h
 *
 * Copyright Voyis Inc., 2021
 *
 * This class represents all the tags supported by the 2G exif library.
 *
 */
#include "EXIFTags/Tag.h"
#include "EXIFTags/TagConstants.h"
#include <memory>
#include <string>
#include <vector>

namespace tg {
namespace tags {

/**
 * @brief This class wraps all of the 2G supported individual tags associated with a particular
 * image file.
 */
class Tags {
  public:
    /**
     * @brief constructor, generates an empty set of flags
     */
    Tags();
    virtual ~Tags();

    /**
     * @brief Given the contents of an image (or at least the header part of it), load the included
     * tags.
     * @param image_header_data, vector of bytes containing at a minimum the image header data.
     * @param error emssage returned by reference in case of a failure.
     * @return bool was the load successful?
     */
    bool loadHeader(const std::vector<uint8_t>& image_header_data, std::string& error_message);

    /**
     * @brief Given a file, load the included tags.
     * @param image_header_data, vector of bytes containing at a minimum the image header data.
     * @param error emssage returned by reference in case of a failure.
     * @return bool was the load successful?
     */
    bool loadHeader(const std::string& filename, std::string& error_message);

    /**
     * @brief Generate an EXIF header to be placed into an image based on the classes data.
     * @param pointer [out] unique pointerto an array of characters, the image header data.
     * @param unsigned [out] length of buffer in bytes.
     * @param string [out] an error message returned by reference when there is a failure.
     * @return bool was the header generation successful
     */
    bool generateHeader(std::unique_ptr<unsigned char[], void (*)(void*)>& image_header_data,
                        unsigned int& length,
                        std::string& error_message) const;

    ///--------------------------------------------------------------------
    /// Accessors and associate enums
    /// (Fixed fields have to setters)
    ///--------------------------------------------------------------------

    enum SubfileTypes { FULL_RESOLUTION_IMAGE = 0, REDUCED_RESOLUTION_IMAGE, PAGE_OF_MULTIPAGE };
    SubfileTypes subfileType() const;

    uint32_t imageWidth() const;
    void imageWidth(uint32_t width);

    uint32_t imageHeight() const;
    void imageHeight(uint32_t height);

    std::vector<uint16_t> bitsPerSample() const;
    void bitsPerSample(const std::vector<uint16_t>& bits);

    enum CompressionType {
        COMPRESSION_EXIF_NONE = 1,
        COMPRESSION_EXIF_CCITTRLE = 2,
        COMPRESSION_EXIF_CCITTFAX3 = 3,
        COMPRESSION_EXIF_CCITTFAX4 = 4,
        COMPRESSION_EXIF_LZW = 5,
        COMPRESSION_EXIF_OJPEG = 6,
        COMPRESSION_EXIF_JPEG = 7,
        COMPRESSION_EXIF_NEXT = 32766,
        COMPRESSION_EXIF_CCITTRLEW = 32771,
        COMPRESSION_EXIF_PACKBITS = 32773,
        COMPRESSION_EXIF_THUNDERSCAN = 32809,
        COMPRESSION_EXIF_IT8CTPAD = 32895,
        COMPRESSION_EXIF_IT8LW = 32896,
        COMPRESSION_EXIF_IT8MP = 32897,
        COMPRESSION_EXIF_IT8BL = 32898,
        COMPRESSION_EXIF_PIXARFILM = 32908,
        COMPRESSION_EXIF_PIXARLOG = 32909,
        COMPRESSION_EXIF_DEFLATE = 32946,
        COMPRESSION_EXIF_ADOBE_DEFLATE = 8,
        COMPRESSION_EXIF_DCS = 32947,
        COMPRESSION_EXIF_JBIG = 34661,
        COMPRESSION_EXIF_SGILOG = 34676,
        COMPRESSION_EXIF_SGILOG24 = 34677,
        COMPRESSION_EXIF_JP2000 = 34712,
    };
    CompressionType compression() const;
    void compression(CompressionType compression);

    enum PhotometricInterpolationType {
        PHOTOMETRIC_EXIF_MINISWHITE = 0,
        PHOTOMETRIC_EXIF_MINISBLACK = 1,
        PHOTOMETRIC_EXIF_RGB = 2,
        PHOTOMETRIC_EXIF_PALETTE = 3,
        PHOTOMETRIC_EXIF_MASK = 4,
        PHOTOMETRIC_EXIF_SEPARATED = 5,
        PHOTOMETRIC_EXIF_YCBCR = 6,
        PHOTOMETRIC_EXIF_CIELAB = 8,
        PHOTOMETRIC_EXIF_ICCLAB = 9,
        PHOTOMETRIC_EXIF_ITULAB = 10,
        PHOTOMETRIC_EXIF_LOGL = 32844,
        PHOTOMETRIC_EXIF_LOGLUV = 32845,
    };
    PhotometricInterpolationType photometricInterpolation() const;
    void photometricInterpolation(PhotometricInterpolationType pi);

    std::string imageDescription() const;
    void imageDescription(const std::string& desc);

    std::string make() const;
    void make(const std::string& make);

    std::string model() const;
    void model(const std::string& model);

    std::vector<uint32_t> stripOffsets() const;
    void stripOffsets(const std::vector<uint32_t>& offsets);

    enum OrientationType {
        ORIENTATION_EXIF_TOPLEFT = 1,
        ORIENTATION_EXIF_TOPRIGHT = 2,
        ORIENTATION_EXIF_BOTRIGHT = 3,
        ORIENTATION_EXIF_BOTLEFT = 4,
        ORIENTATION_EXIF_LEFTTOP = 5,
        ORIENTATION_EXIF_RIGHTTOP = 6,
        ORIENTATION_EXIF_RIGHTBOT = 7,
        ORIENTATION_EXIF_LEFTBOT = 8,
    };
    OrientationType orientation() const;

    uint16_t samplesPerPixel() const;
    void samplesPerPixel(uint16_t samples);

    uint32_t rowsPerStrip() const;
    void rowsPerStrip(uint32_t rows_per_pixel);

    std::vector<uint32_t> stripByteCount() const;
    void stripByteCount(const std::vector<uint32_t>& byte_count);

    enum PlanarConfigurationType { PLANARCONFIG_EXIF_CONTIG = 1, PLANARCONFIG_EXIF_SEPARATE = 2 };
    PlanarConfigurationType planarConfiguration() const;

    std::string software() const;
    void software(const std::string& sw);

    /*These tags aren't supported by the libexif.
    enum PredictorType {
        PREDICTOR_NONE = 1,
        PREDICTOR_HORIZONTAL_DIFFERENCING = 2,
        PREDICTOR_FLOATING_POINT = 3,
        PREDICTOR_HD_X2 = 34892,
        PREDICTOR_HD_X4 = 34893,
        PREDICTOR_FP_X2 = 34894,
        PREDICTOR_FP_X4 = 34895
    };
    //Used for jpeg encoding.
    PredictorType predictor() const;
    void predictor(PredictorType type);

    enum SampleFormatType {
        SAMPLE_FORMAT_UNSIGNED = 1,
        SAMPLE_FORMAT_SIGNED = 2,
        SAMPLE_FORMAT_FLOAT = 3,
        SAMPLE_FORMAT_UNDEFINED = 4,
        SAMPLE_FORMAT_COMPLEX_INT = 5,
        SAMPLE_FORMAT_COMPLEX_FLOAT = 6
    };
    //used for jpeg encoding, takes a vector for each byte type.
    std::vector<SampleFormatType> sampleFormat() const;
    void sampleFormat (const std::vector<SampleFormatType> & type);
    */

    // units of ms
    double exposureTime() const;
    void exposureTime(double exp);

    double fNumber() const;
    void fNumber(double f);

    // Date/time is expressed in microseconds from unix epoch
    uint64_t dateTime() const;
    void dateTime(uint64_t date_time_us_epoch);

    // distance from camera to target in m
    double subjectDistance() const;
    void subjectDistance(double range);

    enum LightSourceType {
        LIGHTSOURCE_UNKNOWN = 0,
        LIGHTSOURCE_DAYLIGHT = 1, // also flash off
        LIGHTSOURCE_WHITELED = 200,
        LIGHTSOURCE_BLUELED = 201
    };
    LightSourceType lightSource() const;
    void lightSource(LightSourceType light_source);

    enum FlashType {
        FLASH_NO_FLASH = 0,
        FLASH_FIRED = 1,
        FLASH_DIDNOTFIRE = 8,
    };
    FlashType flash() const;
    void flash(FlashType flash_type);

    // units of mm
    double focalLength() const;
    void focalLength(double length);

    enum ColourSpaceType {
        COLOURSPACE_sRGB = 0x1,
        COLOURSPACE_ADOBE_RGB = 0x2,
        COLOURSPACE_WIDE_GAMUT_RGB = 0xfffd,
        COLOURSPACE_ICC_PROFILE = 0xfffe,
        COLOURSPACE_UNCALIBRATED = 0xffff
    };
    ColourSpaceType colourSpace() const;
    void colourSpace(ColourSpaceType colour_space);

    // flash energy as a percentage.
    double flashEnergy() const;
    void flashEnergy(double intensity);

    std::string serialNumber() const;
    void serialNumber(const std::string& serial_number);

    std::string lensModel() const;
    void lensModel(const std::string& lens_model);

    double indexOfRefraction() const;
    void indexOfRefraction(double ior);

    double viewportIndex() const;
    void viewportIndex(double vi);

    // units of m
    double viewportThickness() const;
    void viewportThickness(double thickness);

    // units of mm
    double viewportDistance() const;
    void viewportDistance(double distance);

    bool vignetting() const;
    void vignetting(bool is_vignetted);

    enum ViewportType { VIEWPORT_FLAT, VIEWPORT_DOMED };
    ViewportType viewportType() const;
    void viewportType(ViewportType viewport_type);

    enum EnhancementType {
        ENHANCEMENT_NONE,
        ENHANCEMENT_ALGORITHMIC_MONO,
        ENHANCEMENT_REALISTIC_MONO,
        ENHANCEMENT_BASIC_COLOUR,
        ENHANCEMENT_REALISTIC_COLOUR,
        ENHANCEMENT_ENHANCED_COLOUR,
    };
    EnhancementType enhancement() const;
    void enhancement(EnhancementType enhance);

    // units of nm
    std::vector<uint16_t> pixelSize() const;
    void pixelSize(const std::vector<uint16_t>& pixel_size);

    // 1x16 vector of doubles that can be turned into a 4x4 rotation matrix
    // that can be used to transform points in the navigation frame into
    // the camera frame (units of m)
    std::vector<double> matrixNavToCamera() const;
    void matrixNavToCamera(const std::vector<double>& matrix);

    uint32_t imageNumber() const;
    void imageNumber(uint32_t count);

    // depth to CRP in m
    double waterDepth() const;
    void waterDepth(double depth);

    enum BayerPatternType {
        BAYER_GRAYSCALE,
        BAYER_BG2BGR,
        BAYER_GB2BGR,
        BAYER_RG2BGR,
        BAYER_GR2BGR
    };
    BayerPatternType bayerPattern() const;
    void bayerPattern(BayerPatternType pattern);

    // units of Hz
    double frameRate() const;
    void frameRate(double frame_rate);

    // 1x4 vector of doubles that contains the camera matrix (fx, fy, cx, cy) parameters
    std::vector<double> cameraMatrix() const;
    void cameraMatrix(const std::vector<double>& matrix);

    // 1x5 vector of doubles that contains the 5 point rad tan disotrtion
    //  parameters (k1, k2, p1, p2, k3) parameters
    std::vector<double> distortion() const;
    void distortion(const std::vector<double>& matrix);

    // 1x3 vector of doubles that contains the pose of the vehicle
    //  (roll, pitch, heading) in degrees following the PSONNAV convention (see EXIF format
    //  footnote)
    std::vector<double> pose() const;
    void pose(const std::vector<double>& matrix);

    // Altitude of vehicle above seabed in m.
    double vehicleAltitude() const;
    void vehicleAltitude(double altitude);

    // DVL beam ranges in m
    std::vector<double> dvl() const;
    void dvl(const std::vector<double>& beams);

    enum LatitudeRefType { LATITUDEREF_NORTH, LATITUDEREF_SOUTH };
    LatitudeRefType latitudeRef() const;
    void latitudeRef(LatitudeRefType lat_ref);

    // longitude in decimal degrees
    double latitude() const;
    void latitude(double longitude);

    enum LongitudeRefType { LONGITUDEREF_EAST, LONGITUDEREF_WEST };
    LongitudeRefType longitudeRef() const;
    void longitudeRef(LongitudeRefType lat_ref);

    // longitude in decimal degrees
    double longitude() const;
    void longitude(double longitude);

    enum AltitudeRefType { ALTITUDEREF_ABOVE_SEA_LEVEL, ALTITUDEREF_BELOW_SEA_LEVEL };
    AltitudeRefType altitudeRef() const;
    void altitudeRef(AltitudeRefType altitude_ref);

    // altitude in m
    double altitude() const;
    void altitude(double alt);

    // time in us from epoch
    uint64_t ppsTime() const;
    void ppsTime(uint64_t pps);

    // Returns whether or not the tag has been set
    bool isTagSet(Constants::SupportedTags tag_id) const;

    // Returns a deep copy of the tags
    Tags clone(void) const;

  private:
    // storage for the different tags supported by 2G.
    std::vector<std::shared_ptr<Tag>> m_tags;

    /**
     * @brief handle reading the exif data into the internal data structure.
     * @param pointer to the exif data.
     */
    void parseExifData(ExifData* exif);
};

} // namespace tags
} // namespace tg
