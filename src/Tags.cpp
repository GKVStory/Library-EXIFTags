//Tags.cpp
//Copyright 2G Robotics Inc., 2021

#include "EXIFTags/Tags.h"
#include "EXIFTags/ImageHandler.h"

#include <sstream>
#include <iomanip>
#include <ctime>

using namespace tg;
using namespace tags;

Tags::Tags() {
    m_tags.reserve( Constants::LENGTH_SUPPORTED_TAGS );
    for (auto i = 0; i < Constants::LENGTH_SUPPORTED_TAGS; ++i) {
        m_tags.push_back(Tag::tagFactory(static_cast<Constants::SupportedTags>(i)));
    }

    //Set the default, non-user accessible tags
    dynamic_cast<Tag_UINT32*>(m_tags[Constants::SUBFILE_TYPE].get())->setData(FULL_RESOLUTION_IMAGE);
    compression(COMPRESSION_EXIF_NONE);
    bitsPerSample(std::vector<uint16_t>{8});
    photometricInterpolation(PHOTOMETRIC_EXIF_MINISBLACK);
    dynamic_cast<Tag_STRING*>(m_tags[Constants::MAKE].get())->setData(Constants::DEFAULT_MAKE);
    dateTime(0);
    dynamic_cast<Tag_UINT16*>(m_tags[Constants::ORIENTATION].get())->setData(ORIENTATION_EXIF_TOPLEFT);
    samplesPerPixel(1);
    dynamic_cast<Tag_UINT16*>(m_tags[Constants::PLANAR_CONFIGURATION].get())->setData(PLANARCONFIG_EXIF_CONTIG);
    //predictor(PREDICTOR_HORIZONTAL_DIFFERENCING);
    //sampleFormat (std::vector<SampleFormatType> {SAMPLE_FORMAT_UNSIGNED});
    colourSpace(COLOURSPACE_sRGB);
    indexOfRefraction(Constants::DEFAULT_INDEX);
    viewportIndex(Constants::DEFAULT_VIEWPORT_INDEX);
    matrixNavToCamera(Constants::DEFAULT_TRANSFORM);
    cameraMatrix(Constants::DEFAULT_CAM_MATRIX);
    distortion(Constants::DEFAULT_DISTORTION);
    pose(Constants::DEFAULT_POSE);
    latitudeRef(LATITUDEREF_NORTH);
    longitude (0.0);
    longitudeRef(LONGITUDEREF_EAST);
    latitude (0.0);
    altitudeRef(ALTITUDEREF_ABOVE_SEA_LEVEL);
}

Tags::~Tags() {}

bool Tags::loadHeader(const std::vector <uint8_t> & image_header_data, std::string & error_message) {
    
    ExifData * ed = exif_data_new_from_data (reinterpret_cast<const unsigned char *>(image_header_data.data()), static_cast<unsigned int>(image_header_data.size()));
    if (!ed) {
        error_message = ErrorMessages::failed_header_load;
        return false;
    }
    parseExifData(ed);
    exif_data_unref (ed);
    return true;
}

bool Tags::loadHeader(const std::string & filename, std::string & error_message) {

    std::vector<uint8_t> image_header_data;

    if (!ImageHandler::loadHeader (filename, image_header_data, error_message)) {
        return false; //Failed to lead header
    }

    if (!loadHeader(image_header_data, error_message)) {
        return false; //failed to parse header
    }

    return true;
}

bool Tags::generateHeader(std::unique_ptr <unsigned char[], void (*)(void *)> & image_header_data, unsigned int & length, std::string & error_message) const {
    ExifData *exif = exif_data_new();
	if (!exif) {
		error_message = ErrorMessages::memory_error;
		return false;
	}

    exif_data_set_option(exif, EXIF_DATA_OPTION_FOLLOW_SPECIFICATION);
    exif_data_set_data_type(exif, EXIF_DATA_TYPE_COMPRESSED);
	exif_data_set_byte_order(exif, Constants::DEFAULT_BYTE_ORDER);

	/* Create the mandatory EXIF fields with default data */
	exif_data_fix(exif);

    for (auto & tag: m_tags) {
        if (tag->isSet()) {
            if (tag->isStandardTag()) {
                tag->setTag(exif);
            } else {
                //TODO handle the custom 2G tags.
            }
        }
    }

    unsigned char *exif_data;
	unsigned int exif_data_len;
    exif_data_save_data(exif, &exif_data, &exif_data_len);

    if (!exif_data) {
        error_message = ErrorMessages::memory_error;
        exif_data_unref(exif);
        return false;
    }

    //The following gives ownership and management of the memory to the unique pointer.
    image_header_data = std::unique_ptr<unsigned char[], void (*)(void *)> (exif_data, &std::free);
    length = exif_data_len;

    /* //Put in for debugging.
    FILE* pFile;
    pFile = fopen("file.binary", "wb");
       
    fwrite(image_header_data.get(), 1, exif_data_len*sizeof(unsigned char), pFile);

    fclose(pFile);
    */

    exif_data_unref(exif);
    return true;
}

Tags::SubfileTypes Tags::subfileType () const {
    return static_cast<SubfileTypes>(dynamic_cast<Tag_UINT32*>(m_tags[Constants::SUBFILE_TYPE].get())->getData());
}

uint32_t Tags::imageWidth() const {
    uint32_t width =  dynamic_cast<Tag_UINT16*>(m_tags[Constants::IMAGE_WIDTH].get())->getData();
    if (width == 0) { //handles case of loading jpg without tiff headers
        width =  dynamic_cast<Tag_UINT16*>(m_tags[Constants::PIXEL_X_DIMENSION].get())->getData();
    }
    return width;
}
void Tags::imageWidth(uint32_t width) {
    dynamic_cast<Tag_UINT16*>(m_tags[Constants::IMAGE_WIDTH].get())->setData(width);
    dynamic_cast<Tag_UINT16*>(m_tags[Constants::PIXEL_X_DIMENSION].get())->setData(width);
}

uint32_t Tags::imageHeight() const {
    uint32_t height =  dynamic_cast<Tag_UINT16*>(m_tags[Constants::IMAGE_HEIGHT].get())->getData();
    if (height == 0) { //handles case of loading jpg without tiff headers
        height =  dynamic_cast<Tag_UINT16*>(m_tags[Constants::PIXEL_Y_DIMENSION].get())->getData();
    }
    return height;
}
void Tags::imageHeight(uint32_t height) {
    dynamic_cast<Tag_UINT16*>(m_tags[Constants::IMAGE_HEIGHT].get())->setData(height);
    dynamic_cast<Tag_UINT16*>(m_tags[Constants::PIXEL_Y_DIMENSION].get())->setData(height);
}

std::vector<uint16_t> Tags::bitsPerSample() const {
    return dynamic_cast<Tag_UINT16_ARRAY*>(m_tags[Constants::BITS_PER_SAMPLE].get())->getData();
}
void Tags::bitsPerSample( const std::vector<uint16_t> &  bits ) {
    dynamic_cast<Tag_UINT16_ARRAY*>(m_tags[Constants::BITS_PER_SAMPLE].get())->setData(bits);
}

Tags::CompressionType Tags::compression() const {
    return static_cast<CompressionType>(dynamic_cast<Tag_UINT16*>(m_tags[Constants::COMPRESSION].get())->getData());
}
void Tags::compression (Tags::CompressionType compression) {
    dynamic_cast<Tag_UINT16*>(m_tags[Constants::COMPRESSION].get())->setData(compression);
}

Tags::PhotometricInterpolationType Tags::photometricInterpolation() const {
    return static_cast<PhotometricInterpolationType>(dynamic_cast<Tag_UINT16*>(m_tags[Constants::PHOTOMETRIC_INTERPOLATION].get())->getData());
}
void Tags::photometricInterpolation (Tags::PhotometricInterpolationType pi) {
    dynamic_cast<Tag_UINT16*>(m_tags[Constants::PHOTOMETRIC_INTERPOLATION].get())->setData(pi);
}

std::string Tags::imageDescription() const {
    return dynamic_cast<Tag_STRING*>(m_tags[Constants::IMAGE_DESCRIPTION].get())->getData();
}
void Tags::imageDescription(const std::string & desc) {
    dynamic_cast<Tag_STRING*>(m_tags[Constants::IMAGE_DESCRIPTION].get())->setData(desc);
}

std::string Tags::make() const {
    return dynamic_cast<Tag_STRING*>(m_tags[Constants::MAKE].get())->getData();
}
void Tags::make(const std::string & make) {
    dynamic_cast<Tag_STRING*>(m_tags[Constants::MAKE].get())->setData(make);
}


std::string Tags::model() const{
    return dynamic_cast<Tag_STRING*>(m_tags[Constants::MODEL].get())->getData();
}
void Tags::model(const std::string & model) {
    dynamic_cast<Tag_STRING*>(m_tags[Constants::MODEL].get())->setData(model);
}

std::vector<uint32_t> Tags::stripOffsets() const {
    return dynamic_cast<Tag_UINT32_ARRAY*>(m_tags[Constants::STRIP_OFFSETS].get())->getData();
}
void Tags::stripOffsets(const std::vector<uint32_t> & offsets) {
    dynamic_cast<Tag_UINT32_ARRAY*>(m_tags[Constants::STRIP_OFFSETS].get())->setData(offsets);
}

Tags::OrientationType Tags::orientation() const {
     return static_cast<OrientationType>(dynamic_cast<Tag_UINT16*>(m_tags[Constants::ORIENTATION].get())->getData());
}

uint16_t Tags::samplesPerPixel () const {
    return dynamic_cast<Tag_UINT16*>(m_tags[Constants::SAMPLES_PER_PIXEL].get())->getData();
}
void Tags::samplesPerPixel (uint16_t samples) {
    dynamic_cast<Tag_UINT16*>(m_tags[Constants::SAMPLES_PER_PIXEL].get())->setData(samples);
}

uint32_t Tags::rowsPerStrip() const {
    return dynamic_cast<Tag_UINT32*>(m_tags[Constants::ROWS_PER_STRIP].get())->getData();
}
void Tags::rowsPerStrip(uint32_t rows_per_pixel) {
    dynamic_cast<Tag_UINT32*>(m_tags[Constants::ROWS_PER_STRIP].get())->setData(rows_per_pixel);
}

std::vector <uint32_t> Tags::stripByteCount() const {
    return dynamic_cast<Tag_UINT32_ARRAY*>(m_tags[Constants::STRIP_BYTE_COUNTS].get())->getData();
}
void Tags::stripByteCount(const std::vector<uint32_t> & byte_count) {
    dynamic_cast<Tag_UINT32_ARRAY*>(m_tags[Constants::STRIP_BYTE_COUNTS].get())->setData(byte_count);
}

Tags::PlanarConfigurationType Tags::planarConfiguration() const {
    return static_cast<PlanarConfigurationType>(dynamic_cast<Tag_UINT16*>(m_tags[Constants::PLANAR_CONFIGURATION].get())->getData());
}

std::string Tags::software() const {
    return dynamic_cast<Tag_STRING*>(m_tags[Constants::SOFTWARE].get())->getData();
}
void Tags::software(const std::string & sw) {
    dynamic_cast<Tag_STRING*>(m_tags[Constants::SOFTWARE].get())->setData(sw);
}

/*
Tags::PredictorType Tags::predictor() const {
    return static_cast<PredictorType>(dynamic_cast<Tag_UINT16*>(m_tags[Constants::PREDICTOR].get())->getData());
}
void Tags::predictor(Tags::PredictorType type) {
    dynamic_cast<Tag_UINT16*>(m_tags[Constants::PREDICTOR].get())->setData(type);
}

std::vector<Tags::SampleFormatType> Tags::sampleFormat() const {
    std::vector<uint16_t> temp ( dynamic_cast<Tag_UINT16_ARRAY*>(m_tags[Constants::SAMPLE_FORMAT].get())->getData() );
    std::vector <SampleFormatType> type_convert;
    type_convert.reserve(temp.size());
    for (auto val : temp) {
        type_convert.push_back (static_cast<SampleFormatType>(val));
    }
    return type_convert;
}
void Tags::sampleFormat (const std::vector<Tags::SampleFormatType> & type) {
    std::vector <uint16_t> type_convert;
    type_convert.reserve(type.size());
    for (auto val : type) {
        type_convert.push_back (static_cast<uint16_t>(val));
    }
    dynamic_cast<Tag_UINT16_ARRAY*>(m_tags[Constants::SAMPLE_FORMAT].get())->setData(type_convert);
}
*/

double Tags::exposureTime() const {
    return dynamic_cast<Tag_UDOUBLE*>(m_tags[Constants::EXPOSURE_TIME].get())->getData();
}
void Tags::exposureTime(double exp) {
    dynamic_cast<Tag_UDOUBLE*>(m_tags[Constants::EXPOSURE_TIME].get())->setData(exp);
}

double Tags::fNumber() const {
    return dynamic_cast<Tag_UDOUBLE*>(m_tags[Constants::F_NUMBER].get())->getData();
}
void Tags::fNumber(double f) {
    dynamic_cast<Tag_UDOUBLE*>(m_tags[Constants::F_NUMBER].get())->setData(f);
}

uint64_t Tags::dateTime () const {
    std::string datetime = dynamic_cast<Tag_STRING*>(m_tags[Constants::DATE_TIME_ORIGINAL].get())->getData();
    std::string datetime_subsec = dynamic_cast<Tag_STRING*>(m_tags[Constants::SUB_SEC_ORIGINAL].get())->getData();

    std::tm t{};
    double subsec (0.0);
    std::istringstream ss_dt(datetime);
    std::istringstream ss_ss(datetime_subsec);

    ss_dt >> std::get_time(&t, Constants::DEFAULT_TIMESTAMP_FORMAT.c_str());
    if (ss_dt.fail()) {
        return 0; //couldn't parse the time correctly
    }

#ifdef _WIN32
    std::time_t dt = _mkgmtime(&t);
#else
    std::time_t dt = timegm(&t);
#endif

    uint64_t epoch_time_us = static_cast <uint64_t> (dt) * 1000000;
    
    ss_ss >> subsec;
    if (ss_ss.fail()) {
        subsec = 0.0;
    }
    epoch_time_us += static_cast <uint64_t> (subsec * 1.0E6);

    return epoch_time_us;
}
void Tags::dateTime (uint64_t date_time_us_epoch) {
    std::ostringstream ss_dt;
    std::ostringstream ss_ss;

    uint64_t s_from_epoch (date_time_us_epoch / 1000000);
    double subsec ( static_cast<double>(date_time_us_epoch - s_from_epoch*1.0E6) / 1.0E6);

    std::time_t t = static_cast<time_t>(s_from_epoch);
    std::tm tm = *std::gmtime(&t);

    ss_dt << std::put_time (&tm, Constants::DEFAULT_TIMESTAMP_FORMAT.c_str());
    ss_ss << subsec;

    dynamic_cast<Tag_STRING*>(m_tags[Constants::DATE_TIME_ORIGINAL].get())->setData(ss_dt.str());
    dynamic_cast<Tag_STRING*>(m_tags[Constants::SUB_SEC_ORIGINAL].get())->setData(ss_ss.str());
}

double Tags::subjectDistance() const {
    return dynamic_cast<Tag_UDOUBLE*>(m_tags[Constants::SUBJECT_DISTANCE].get())->getData();
}
void Tags::subjectDistance(double range) {
    dynamic_cast<Tag_UDOUBLE*>(m_tags[Constants::SUBJECT_DISTANCE].get())->setData(range);
}

Tags::LightSourceType Tags::lightSource() const {
    return static_cast<LightSourceType>(dynamic_cast<Tag_UINT16*>(m_tags[Constants::LIGHT_SOURCE].get())->getData());
}
void Tags::lightSource (Tags::LightSourceType light_source) {
    dynamic_cast<Tag_UINT16*>(m_tags[Constants::LIGHT_SOURCE].get())->setData(light_source);
}

Tags::FlashType Tags::flash() const {
    return static_cast<FlashType>(dynamic_cast<Tag_UINT16*>(m_tags[Constants::FLASH].get())->getData());
}
void Tags::flash(Tags::FlashType flash_type) {
    dynamic_cast<Tag_UINT16*>(m_tags[Constants::FLASH].get())->setData(flash_type);
}

double Tags::focalLength() const {
    return dynamic_cast<Tag_UDOUBLE*>(m_tags[Constants::FOCAL_LENGTH].get())->getData();
}
void Tags::focalLength( double length ) {
    dynamic_cast<Tag_UDOUBLE*>(m_tags[Constants::FOCAL_LENGTH].get())->setData(length);
}

Tags::ColourSpaceType Tags::colourSpace() const {
    return static_cast<ColourSpaceType>(dynamic_cast<Tag_UINT16*>(m_tags[Constants::COLOR_SPACE].get())->getData());
}
void Tags::colourSpace(Tags::ColourSpaceType colour_space) {
    dynamic_cast<Tag_UINT16*>(m_tags[Constants::COLOR_SPACE].get())->setData(colour_space);
}

double Tags::flashEnergy () const {
    return (dynamic_cast<Tag_UDOUBLE*>(m_tags[Constants::FLASH_ENERGY].get())->getData());
}
void Tags::flashEnergy (double intensity) {
    dynamic_cast<Tag_UDOUBLE*>(m_tags[Constants::FLASH_ENERGY].get())->setData(intensity);
}

std::string Tags::serialNumber() const {
     return dynamic_cast<Tag_STRING*>(m_tags[Constants::SERIAL_NUMBER].get())->getData();
}
void Tags::serialNumber( const std::string & serial_number ) {
    dynamic_cast<Tag_STRING*>(m_tags[Constants::SERIAL_NUMBER].get())->setData(serial_number);
}

std::string Tags::lensModel() const {
    return dynamic_cast<Tag_STRING*>(m_tags[Constants::LENS_MODEL].get())->getData();
}
void Tags::lensModel( const std::string & lens_model ) {
    dynamic_cast<Tag_STRING*>(m_tags[Constants::LENS_MODEL].get())->setData(lens_model);
}

double Tags::indexOfRefraction() const {
    return dynamic_cast<Tag_UDOUBLE*>(m_tags[Constants::INDEX_OF_REFRACTION].get())->getData();
}
void Tags::indexOfRefraction( double ior ) {
    dynamic_cast<Tag_UDOUBLE*>(m_tags[Constants::INDEX_OF_REFRACTION].get())->setData(ior);
}

double Tags::viewportIndex() const {
    return dynamic_cast<Tag_UDOUBLE*>(m_tags[Constants::VIEWPORT_INDEX].get())->getData();
}
void Tags::viewportIndex( double vi ) {
    dynamic_cast<Tag_UDOUBLE*>(m_tags[Constants::VIEWPORT_INDEX].get())->setData(vi);
}

double Tags::viewportThickness () const {
    return dynamic_cast<Tag_UDOUBLE*>(m_tags[Constants::VIEWPORT_THICKNESS].get())->getData();
}
void Tags::viewportThickness (double thickness ) {
    dynamic_cast<Tag_UDOUBLE*>(m_tags[Constants::VIEWPORT_THICKNESS].get())->setData(thickness);
}

double Tags::viewportDistance () const {
    return dynamic_cast<Tag_UDOUBLE*>(m_tags[Constants::VIEWPORT_DISTANCE].get())->getData();
}
void Tags::viewportDistance (double distance) {
    dynamic_cast<Tag_UDOUBLE*>(m_tags[Constants::VIEWPORT_DISTANCE].get())->setData(distance);
}
bool Tags::vignetting () const {
    return dynamic_cast<Tag_UINT16*>(m_tags[Constants::VIGNETTING].get())->getData() != 0;
}
void Tags::vignetting (bool is_vignetted) {
    dynamic_cast<Tag_UINT16*>(m_tags[Constants::VIGNETTING].get())->setData(is_vignetted);
}

Tags::ViewportType Tags::viewportType () const {
    return static_cast<ViewportType>(dynamic_cast<Tag_UINT16*>(m_tags[Constants::VIEWPORT_TYPE].get())->getData());
}
void Tags::viewportType (Tags::ViewportType viewport_type) {
    dynamic_cast<Tag_UINT16*>(m_tags[Constants::VIEWPORT_TYPE].get())->setData(viewport_type);
}

Tags::EnhancementType Tags::enhancement() const {
    return static_cast<EnhancementType>(dynamic_cast<Tag_UINT16*>(m_tags[Constants::ENAHNCEMENT_TYPE].get())->getData());
}
void Tags::enhancement ( Tags::EnhancementType enhance) {
    dynamic_cast<Tag_UINT16*>(m_tags[Constants::ENAHNCEMENT_TYPE].get())->setData(enhance);
}

std::vector<uint16_t> Tags::pixelSize() const {
    return dynamic_cast<Tag_UINT16_ARRAY*>(m_tags[Constants::PIXEL_SIZE].get())->getData();
}
void Tags::pixelSize(const std::vector<uint16_t> & pixel_size) {
    dynamic_cast<Tag_UINT16_ARRAY*>(m_tags[Constants::PIXEL_SIZE].get())->setData(pixel_size);
}

std::vector<double> Tags::matrixNavToCamera () const {
    return dynamic_cast<Tag_DOUBLE_ARRAY*>(m_tags[Constants::MATRIX_NAV_TO_CAMERA].get())->getData();
}
void Tags::matrixNavToCamera( const std::vector<double> & matrix) {
    dynamic_cast<Tag_DOUBLE_ARRAY*>(m_tags[Constants::MATRIX_NAV_TO_CAMERA].get())->setData(matrix);
}

uint32_t Tags::imageNumber() const {
    return dynamic_cast<Tag_UINT32*>(m_tags[Constants::IMAGE_NUMBER].get())->getData();
}
void Tags::imageNumber( uint32_t count ) {
    dynamic_cast<Tag_UINT32*>(m_tags[Constants::IMAGE_NUMBER].get())->setData(count);
}

double Tags::waterDepth() const {
    return dynamic_cast<Tag_DOUBLE*>(m_tags[Constants::WATER_DEPTH].get())->getData();
}
void Tags::waterDepth(double depth) {
    dynamic_cast<Tag_DOUBLE*>(m_tags[Constants::WATER_DEPTH].get())->setData(depth);
}

Tags::BayerPatternType Tags::bayerPattern() const {
    return static_cast<BayerPatternType>(dynamic_cast<Tag_UINT16*>(m_tags[Constants::BAYER_PATTERN].get())->getData());
}
void Tags::bayerPattern (Tags::BayerPatternType pattern)
{
    dynamic_cast<Tag_UINT16*>(m_tags[Constants::BAYER_PATTERN].get())->setData(pattern);
}

double Tags::frameRate() const {
    return dynamic_cast<Tag_UDOUBLE*>(m_tags[Constants::FRAME_RATE].get())->getData();
}
void Tags::frameRate(double frame_rate) {
    dynamic_cast<Tag_UDOUBLE*>(m_tags[Constants::FRAME_RATE].get())->setData(frame_rate);
}

std::vector<double> Tags::cameraMatrix () const {
    return dynamic_cast<Tag_DOUBLE_ARRAY*>(m_tags[Constants::CAMERA_MATRIX].get())->getData();
}
void Tags::cameraMatrix( const std::vector<double> & matrix) {
    dynamic_cast<Tag_DOUBLE_ARRAY*>(m_tags[Constants::CAMERA_MATRIX].get())->setData(matrix);
}

std::vector<double> Tags::distortion () const {
    return dynamic_cast<Tag_DOUBLE_ARRAY*>(m_tags[Constants::DISTORTION].get())->getData();
}
void Tags::distortion( const std::vector<double> & matrix) {
    dynamic_cast<Tag_DOUBLE_ARRAY*>(m_tags[Constants::DISTORTION].get())->setData(matrix);
}

std::vector<double> Tags::pose () const {
    return dynamic_cast<Tag_DOUBLE_ARRAY*>(m_tags[Constants::POSE].get())->getData();
}
void Tags::pose( const std::vector<double> & matrix) {
    dynamic_cast<Tag_DOUBLE_ARRAY*>(m_tags[Constants::POSE].get())->setData(matrix);
}

double Tags::vehicleAltitude() const {
    return dynamic_cast<Tag_UDOUBLE*>(m_tags[Constants::VEHICLE_ALTITUDE].get())->getData();
}
void Tags::vehicleAltitude(double altitude) {
    dynamic_cast<Tag_UDOUBLE*>(m_tags[Constants::VEHICLE_ALTITUDE].get())->setData(altitude);
}

Tags::LatitudeRefType Tags::latitudeRef() const {
    std::string ref = dynamic_cast<Tag_STRING*>(m_tags[Constants::GPS_LATITUDE_REF].get())->getData();
    if (ref[0] == 'N') {
        return LatitudeRefType::LATITUDEREF_NORTH;
    }   else {
        return LatitudeRefType::LATITUDEREF_SOUTH;
    }
}
void Tags::latitudeRef(Tags::LatitudeRefType lat_ref ) {
    if (lat_ref == LatitudeRefType::LATITUDEREF_NORTH) {
        dynamic_cast<Tag_STRING*>(m_tags[Constants::GPS_LATITUDE_REF].get())->setData("N");
    } else {
        dynamic_cast<Tag_STRING*>(m_tags[Constants::GPS_LATITUDE_REF].get())->setData("S");
    }
}

double Tags::latitude() const {
    std::vector <double> degminsec = dynamic_cast<Tag_UDOUBLE_ARRAY*>(m_tags[Constants::GPS_LATITUDE].get())->getData(); 
    if (degminsec.size() != 3) {
        //Should never happen
        return 0.0;
    }
    return Constants::DMSToDeg(degminsec[0], degminsec[1], degminsec[2]);
}
void Tags::latitude( double latitude ) {
    std::vector <double> dms = {0.0, 0.0, 0.0};
    Constants::degToDMS(dms[0], dms[1], dms[2], latitude);

    dynamic_cast<Tag_UDOUBLE_ARRAY*>(m_tags[Constants::GPS_LATITUDE].get())->setData(dms);
}

Tags::LongitudeRefType Tags::longitudeRef() const {
    std::string ref = dynamic_cast<Tag_STRING*>(m_tags[Constants::GPS_LONGITUDE_REF].get())->getData();
    if (ref == "E") {
        return LongitudeRefType::LONGITUDEREF_EAST;
    }   else {
        return LongitudeRefType::LONGITUDEREF_WEST;
    }
}
void Tags::longitudeRef(Tags::LongitudeRefType long_ref ) {
    if (long_ref == LongitudeRefType::LONGITUDEREF_EAST) {
        dynamic_cast<Tag_STRING*>(m_tags[Constants::GPS_LONGITUDE_REF].get())->setData("E");
    } else {
        dynamic_cast<Tag_STRING*>(m_tags[Constants::GPS_LONGITUDE_REF].get())->setData("W");
    }
}

double Tags::longitude() const {
    std::vector <double> degminsec = dynamic_cast<Tag_UDOUBLE_ARRAY*>(m_tags[Constants::GPS_LONGITUDE].get())->getData(); 
    if (degminsec.size() != 3) {
        //Should never happen
        return 0.0;
    }
    return Constants::DMSToDeg(degminsec[0], degminsec[1], degminsec[2]);
}
void Tags::longitude(double longitude) {
    std::vector <double> dms = {0.0, 0.0, 0.0};
    Constants::degToDMS(dms[0], dms[1], dms[2], longitude);
    dynamic_cast<Tag_UDOUBLE_ARRAY*>(m_tags[Constants::GPS_LONGITUDE].get())->setData(dms);
}

Tags::AltitudeRefType Tags::altitudeRef() const {
    return static_cast<AltitudeRefType> (dynamic_cast<Tag_UINT8*>(m_tags[Constants::GPS_ALTITUDE_REF].get())->getData());
}
void Tags::altitudeRef(Tags::AltitudeRefType altitude_ref) {
    dynamic_cast<Tag_UINT8*>(m_tags[Constants::GPS_ALTITUDE_REF].get())->setData(altitude_ref);
}

double Tags::altitude() const {
    std::vector <double> temp = dynamic_cast<Tag_UDOUBLE_ARRAY*>(m_tags[Constants::GPS_ALTITUDE].get())->getData();
    if (temp.size() > 0) {
        return temp[0];
    }
    return 0.0;
}
void Tags::altitude(double alt){
    std::vector<double> temp;
    temp.push_back(alt);
    dynamic_cast<Tag_UDOUBLE_ARRAY*>(m_tags[Constants::GPS_ALTITUDE].get())->setData(temp);
}

uint64_t Tags::ppsTime() const {
    uint64_t upper = dynamic_cast<Tag_UINT32*>(m_tags[Constants::TIFFTAG_2G_PPS_TIME_UPPER].get())->getData();
    uint64_t lower = dynamic_cast<Tag_UINT32*>(m_tags[Constants::TIFFTAG_2G_PPS_TIME_LOWER].get())->getData();
    return lower + (upper << 32);
}
void Tags::ppsTime(uint64_t pps){
    uint32_t lower = pps & 0xFFFFFFFF;
    uint32_t upper = pps >> 32;

    dynamic_cast<Tag_UINT32*>(m_tags[Constants::TIFFTAG_2G_PPS_TIME_LOWER].get())->setData(lower);
    dynamic_cast<Tag_UINT32*>(m_tags[Constants::TIFFTAG_2G_PPS_TIME_UPPER].get())->setData(upper);
}

void Tags::parseExifData ( ExifData * ed ) {
    //TODO: Try to load the custom 2G tags from the makernote first

    for (auto & tag: m_tags) {
        if (tag->isStandardTag()) {
            tag->getTag(ed);
        } else {
            //TODO handle the custom 2G tags.
        }
    }
}
