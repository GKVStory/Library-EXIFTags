//ExifTagsPython.cpp
//Copyright 2G Robotics Inc., 2021
#include <pybind11/pybind11.h>
#include "EXIFTags/Tags.h"
#include "EXIFTags/ImageHandler.h"

#include <stdexcept>
#include <fstream>

namespace py = pybind11;

/**
 * This function loads an image and resaves it with updated depth.
 * @param double [in] depth/subject_distance
 * @param const [in] reference to an input filename.
 * @param const [in] reference to an output filename.
 * @throws exception if anything fails.
 */

void saveTags(tg::tags::Tags & tags, const std::string & in_filename, const std::string & out_filename) {
    std::string error_message;

    std::vector<uint8_t> image_data;
    
    std::ifstream file(in_filename, std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);    
    image_data.resize(static_cast<unsigned int>(size));
    if (!file.read(reinterpret_cast<char *>(image_data.data()), size )) {
        error_message = tg::tags::ErrorMessages::failed_file_load + in_filename;
        throw std::exception(error_message.c_str());
        return;
    }
    file.close();

    if (image_data.size() < 2) {
        //raise an exception and return
        throw std::exception("Insufficient data loaded.");
        return;
    }

    std::vector<uint8_t> output_image_data;
    if (image_data[0] == tg::tags::ImageHandler::JPEGHeaderStart[0] && image_data[1] == tg::tags::ImageHandler::JPEGHeaderStart[1]) {
        //jpeg file
        if (!tg::tags::ImageHandler::tagJpeg(tags, image_data, output_image_data, error_message)) {
            throw std::exception(error_message.c_str());
            return;
        }

    } else {
        //assume tiff
        if (!tg::tags::ImageHandler::tagTiff(tags, image_data, output_image_data, error_message)) {
            throw std::exception(error_message.c_str());
            return;
        }
    }

    std::ofstream fileout(out_filename, std::ios::binary);
    if (!fileout.write(reinterpret_cast<char *>(output_image_data.data()), output_image_data.size() )) {
        error_message = "Could not save file: " + out_filename;
        throw std::exception(error_message.c_str());
        return;
    }
    fileout.close();

}

// python module for ExifTags
PYBIND11_MODULE(EXIFTagsLibPython, m) {

    m.doc() = "Exif Tag Tools";
    m.def("save_tags", &saveTags, "Add new tags to the image and save them.", py::arg("tags"), py::arg("input_file"), py::arg ("output_file"));

    py::enum_<tg::tags::Tags::SubfileTypes>(m, "SubfileTypes")
      .value("FULL_RESOLUTION_IMAGE", tg::tags::Tags::SubfileTypes::FULL_RESOLUTION_IMAGE)
      .value("REDUCED_RESOLUTION_IMAGE",  tg::tags::Tags::SubfileTypes::REDUCED_RESOLUTION_IMAGE)
      .value("PAGE_OF_MULTIPAGE", tg::tags::Tags::SubfileTypes::PAGE_OF_MULTIPAGE)
      .export_values();

    py::enum_<tg::tags::Tags::CompressionType>(m, "Compressiontype")
      .value("COMPRESSION_NONE", tg::tags::Tags::CompressionType::COMPRESSION_NONE)
      .value("COMPRESSION_CCITTRLE", tg::tags::Tags::CompressionType::COMPRESSION_CCITTRLE)
      .value("COMPRESSION_CCITTFAX3", tg::tags::Tags::CompressionType::COMPRESSION_CCITTFAX3)
      .value("COMPRESSION_CCITTFAX4", tg::tags::Tags::CompressionType::COMPRESSION_CCITTFAX4)
      .value("COMPRESSION_LZW", tg::tags::Tags::CompressionType::COMPRESSION_LZW)
      .value("COMPRESSION_OJPEG", tg::tags::Tags::CompressionType::COMPRESSION_OJPEG)
      .value("COMPRESSION_JPEG", tg::tags::Tags::CompressionType::COMPRESSION_JPEG)
      .value("COMPRESSION_NEXT", tg::tags::Tags::CompressionType::COMPRESSION_NEXT)
      .value("COMPRESSION_CCITTRLEW", tg::tags::Tags::CompressionType::COMPRESSION_CCITTRLEW)
      .value("COMPRESSION_PACKBITS", tg::tags::Tags::CompressionType::COMPRESSION_PACKBITS)
      .value("COMPRESSION_THUNDERSCAN", tg::tags::Tags::CompressionType::COMPRESSION_THUNDERSCAN)
      .value("COMPRESSION_IT8CTPAD", tg::tags::Tags::CompressionType::COMPRESSION_IT8CTPAD)
      .value("COMPRESSION_IT8LW", tg::tags::Tags::CompressionType::COMPRESSION_IT8LW)
      .value("COMPRESSION_IT8MP", tg::tags::Tags::CompressionType::COMPRESSION_IT8MP)
      .value("COMPRESSION_IT8BL", tg::tags::Tags::CompressionType::COMPRESSION_IT8BL)
      .value("COMPRESSION_PIXARFILM", tg::tags::Tags::CompressionType::COMPRESSION_PIXARFILM)
      .value("COMPRESSION_PIXARLOG", tg::tags::Tags::CompressionType::COMPRESSION_PIXARLOG)
      .value("COMPRESSION_DEFLATE", tg::tags::Tags::CompressionType::COMPRESSION_DEFLATE)
      .value("COMPRESSION_ADOBE_DEFLATE", tg::tags::Tags::CompressionType::COMPRESSION_ADOBE_DEFLATE)
      .value("COMPRESSION_DCS", tg::tags::Tags::CompressionType::COMPRESSION_DCS)
      .value("COMPRESSION_JBIG", tg::tags::Tags::CompressionType::COMPRESSION_JBIG)
      .value("COMPRESSION_SGILOG", tg::tags::Tags::CompressionType::COMPRESSION_SGILOG)
      .value("COMPRESSION_SGILOG24", tg::tags::Tags::CompressionType::COMPRESSION_SGILOG24)
      .value("COMPRESSION_JP2000", tg::tags::Tags::CompressionType::COMPRESSION_JP2000)
      .export_values();

    py::enum_<tg::tags::Tags::PhotometricInterpolationType>(m, "PhotometricInterpolationType")
      .value("PHOTOMETRIC_MINISWHITE", tg::tags::Tags::PhotometricInterpolationType::PHOTOMETRIC_MINISWHITE)
      .value("PHOTOMETRIC_MINISBLACK", tg::tags::Tags::PhotometricInterpolationType::PHOTOMETRIC_MINISBLACK)
      .value("PHOTOMETRIC_RGB", tg::tags::Tags::PhotometricInterpolationType::PHOTOMETRIC_RGB)
      .value("PHOTOMETRIC_PALETTE", tg::tags::Tags::PhotometricInterpolationType::PHOTOMETRIC_PALETTE)
      .value("PHOTOMETRIC_MASK", tg::tags::Tags::PhotometricInterpolationType::PHOTOMETRIC_MASK)
      .value("PHOTOMETRIC_SEPARATED", tg::tags::Tags::PhotometricInterpolationType::PHOTOMETRIC_SEPARATED)
      .value("PHOTOMETRIC_YCBCR", tg::tags::Tags::PhotometricInterpolationType::PHOTOMETRIC_YCBCR)
      .value("PHOTOMETRIC_CIELAB", tg::tags::Tags::PhotometricInterpolationType::PHOTOMETRIC_CIELAB)
      .value("PHOTOMETRIC_ICCLAB", tg::tags::Tags::PhotometricInterpolationType::PHOTOMETRIC_ICCLAB)
      .value("PHOTOMETRIC_ITULAB", tg::tags::Tags::PhotometricInterpolationType::PHOTOMETRIC_ITULAB)
      .value("PHOTOMETRIC_LOGL", tg::tags::Tags::PhotometricInterpolationType::PHOTOMETRIC_LOGL)
      .value("PHOTOMETRIC_LOGLUV", tg::tags::Tags::PhotometricInterpolationType::PHOTOMETRIC_LOGLUV)
      .export_values();

    py::enum_<tg::tags::Tags::OrientationType>(m, "OrientationType")
      .value("ORIENTATION_TOPLEFT", tg::tags::Tags::OrientationType::ORIENTATION_TOPLEFT)
      .value("ORIENTATION_TOPRIGHT", tg::tags::Tags::OrientationType::ORIENTATION_TOPRIGHT)
      .value("ORIENTATION_BOTRIGHT", tg::tags::Tags::OrientationType::ORIENTATION_BOTRIGHT)
      .value("ORIENTATION_BOTLEFT", tg::tags::Tags::OrientationType::ORIENTATION_BOTLEFT)
      .value("ORIENTATION_LEFTTOP", tg::tags::Tags::OrientationType::ORIENTATION_LEFTTOP)
      .value("ORIENTATION_RIGHTTOP", tg::tags::Tags::OrientationType::ORIENTATION_RIGHTTOP)
      .value("ORIENTATION_RIGHTBOT", tg::tags::Tags::OrientationType::ORIENTATION_RIGHTBOT)
      .value("ORIENTATION_LEFTBOT", tg::tags::Tags::OrientationType::ORIENTATION_LEFTBOT)
      .export_values();
      

    py::enum_<tg::tags::Tags::PlanarConfigurationType>(m, "PlanarConfigurationType")
      .value("PLANARCONFIG_CONTIG", tg::tags::Tags::PlanarConfigurationType::PLANARCONFIG_CONTIG)
      .value("PLANARCONFIG_SEPARATE", tg::tags::Tags::PlanarConfigurationType::PLANARCONFIG_SEPARATE)
      .export_values();

    py::enum_<tg::tags::Tags::LightSourceType>(m, "LightSourceType")
      .value("LIGHTSOURCE_UNKNOWN", tg::tags::Tags::LightSourceType::LIGHTSOURCE_UNKNOWN)
      .value("LIGHTSOURCE_DAYLIGHT", tg::tags::Tags::LightSourceType::LIGHTSOURCE_DAYLIGHT)
      .value("LIGHTSOURCE_WHITELED", tg::tags::Tags::LightSourceType::LIGHTSOURCE_WHITELED)
      .value("LIGHTSOURCE_BLUELED", tg::tags::Tags::LightSourceType::LIGHTSOURCE_BLUELED)
      .export_values(); 

    py::enum_<tg::tags::Tags::FlashType>(m, "FlashType")
      .value("FLASH_NO_FLASH", tg::tags::Tags::FlashType::FLASH_NO_FLASH)
      .value("FLASH_FIRED", tg::tags::Tags::FlashType::FLASH_FIRED)
      .value("FLASH_DIDNOTFIRE", tg::tags::Tags::FlashType::FLASH_DIDNOTFIRE)
      .export_values(); 

    py::enum_<tg::tags::Tags::ColourSpaceType>(m, "ColourSpaceType")
      .value("COLOURSPACE_sRGB", tg::tags::Tags::ColourSpaceType::COLOURSPACE_sRGB)
      .value("COLOURSPACE_ADOBE_RGB", tg::tags::Tags::ColourSpaceType::COLOURSPACE_ADOBE_RGB)
      .value("COLOURSPACE_WIDE_GAMUT_RGB", tg::tags::Tags::ColourSpaceType::COLOURSPACE_WIDE_GAMUT_RGB)
      .value("COLOURSPACE_ICC_PROFILE", tg::tags::Tags::ColourSpaceType::COLOURSPACE_ICC_PROFILE)
      .value("COLOURSPACE_UNCALIBRATED", tg::tags::Tags::ColourSpaceType::COLOURSPACE_UNCALIBRATED)
      .export_values(); 

    py::enum_<tg::tags::Tags::ViewportType>(m, "ViewportType")
      .value("VIEWPORT_FLAT", tg::tags::Tags::ViewportType::VIEWPORT_FLAT)
      .value("VIEWPORT_DOMED", tg::tags::Tags::ViewportType::VIEWPORT_DOMED)
      .export_values(); 

    py::enum_<tg::tags::Tags::EnhancementType>(m, "EnhancementType")
      .value("ENHANCEMENT_NONE", tg::tags::Tags::EnhancementType::ENHANCEMENT_NONE)
      .value("ENHANCEMENT_ALGORITHMIC_MONO", tg::tags::Tags::EnhancementType::ENHANCEMENT_ALGORITHMIC_MONO)
      .value("ENHANCEMENT_REALISTIC_MONO", tg::tags::Tags::EnhancementType::ENHANCEMENT_REALISTIC_MONO)
      .value("ENHANCEMENT_BASIC_COLOUR", tg::tags::Tags::EnhancementType::ENHANCEMENT_BASIC_COLOUR)
      .value("ENHANCEMENT_REALISTIC_COLOUR", tg::tags::Tags::EnhancementType::ENHANCEMENT_REALISTIC_COLOUR)
      .value("ENHANCEMENT_ENHANCED_COLOUR", tg::tags::Tags::EnhancementType::ENHANCEMENT_ENHANCED_COLOUR)
      .export_values(); 

    py::enum_<tg::tags::Tags::BayerPatternType>(m, "BayerPatternType")
      .value("BAYER_GRAYSCALE", tg::tags::Tags::BayerPatternType::BAYER_GRAYSCALE)
      .value("BAYER_BG2BGR", tg::tags::Tags::BayerPatternType::BAYER_BG2BGR)
      .value("BAYER_GB2BGR", tg::tags::Tags::BayerPatternType::BAYER_GB2BGR)
      .value("BAYER_RG2BGR", tg::tags::Tags::BayerPatternType::BAYER_RG2BGR)
      .value("BAYER_GR2BGR", tg::tags::Tags::BayerPatternType::BAYER_GR2BGR)
      .export_values(); 

    py::enum_<tg::tags::Tags::LatitudeRefType>(m, "LatitudeRefType")
      .value("LATITUDEREF_NORTH", tg::tags::Tags::LatitudeRefType::LATITUDEREF_NORTH)
      .value("LATITUDEREF_SOUTH", tg::tags::Tags::LatitudeRefType::LATITUDEREF_SOUTH)
      .export_values();

    py::enum_<tg::tags::Tags::LongitudeRefType>(m, "LongitudeRefType")
      .value("LONGITUDEREF_EAST", tg::tags::Tags::LongitudeRefType::LONGITUDEREF_EAST)
      .value("LONGITUDEREF_WEST", tg::tags::Tags::LongitudeRefType::LONGITUDEREF_WEST)
      .export_values();

    py::enum_<tg::tags::Tags::AltitudeRefType>(m, "AltitudeRefType")
      .value("ALTITUDEREF_ABOVE_SEA_LEVEL", tg::tags::Tags::AltitudeRefType::ALTITUDEREF_ABOVE_SEA_LEVEL)
      .value("ALTITUDEREF_BELOW_SEA_LEVEL", tg::tags::Tags::AltitudeRefType::ALTITUDEREF_BELOW_SEA_LEVEL)
      .export_values();

    py::class_<tg::tags::Tags>(m, "Tags")
      .def(py::init<>())
      .def("load_header", py::overload_cast<const std::string &, std::string &>(&tg::tags::Tags::loadHeader), "Load the header from a given file.", py::arg("filename"), py::arg("error_message"))
      .def("subfile_type", &tg::tags::Tags::subfileType)
      .def_property("image_width",
            py::overload_cast<void>(&tg::tags::Tags::imageWidth, py::const_),
            py::overload_cast<uint32_t>(&tg::tags::Tags::imageWidth))
      .def_property("image_height", 
            py::overload_cast<void>(&tg::tags::Tags::imageHeight, py::const_),
            py::overload_cast<uint32_t>(&tg::tags::Tags::imageHeight))
      .def_property("bits_per_sample", 
            py::overload_cast<void>(&tg::tags::Tags::bitsPerSample, py::const_),
            py::overload_cast<uint16_t>(&tg::tags::Tags::bitsPerSample))
      .def_property("compression", 
            py::overload_cast<void>(&tg::tags::Tags::compression, py::const_),
            py::overload_cast<tg::tags::Tags::CompressionType>(&tg::tags::Tags::compression))
      .def_property("photometric_interpolation", 
            py::overload_cast<void>(&tg::tags::Tags::photometricInterpolation, py::const_),
            py::overload_cast<tg::tags::Tags::PhotometricInterpolationType>(&tg::tags::Tags::photometricInterpolation))
      .def_property("image_description", 
            py::overload_cast<void>(&tg::tags::Tags::imageDescription, py::const_),
            py::overload_cast<const std::string &>(&tg::tags::Tags::imageDescription))
      .def_property("make", 
            py::overload_cast<void>(&tg::tags::Tags::make, py::const_),
            py::overload_cast<const std::string &>(&tg::tags::Tags::make))
      .def_property("model", 
            py::overload_cast<void>(&tg::tags::Tags::model, py::const_),
            py::overload_cast<const std::string &>(&tg::tags::Tags::model))
      .def_property("strip_offsets", 
            py::overload_cast<void>(&tg::tags::Tags::stripOffsets, py::const_),
            py::overload_cast<uint32_t>(&tg::tags::Tags::stripOffsets))
      .def_property_readonly("orientation", &tg::tags::Tags::orientation)
      .def_property("samples_per_pixel", 
            py::overload_cast<void>(&tg::tags::Tags::samplesPerPixel, py::const_),
            py::overload_cast<uint16_t>(&tg::tags::Tags::samplesPerPixel))
      .def_property("rows_per_strip", 
            py::overload_cast<void>(&tg::tags::Tags::rowsPerStrip, py::const_),
            py::overload_cast<uint32_t>(&tg::tags::Tags::rowsPerStrip))
      .def_property("strip_byte_count", 
            py::overload_cast<void>(&tg::tags::Tags::stripByteCount, py::const_),
            py::overload_cast<uint32_t>(&tg::tags::Tags::stripByteCount))
      .def_property_readonly("planar_config", &tg::tags::Tags::planarConfiguration)
      .def_property("software", 
            py::overload_cast<void>(&tg::tags::Tags::software, py::const_),
            py::overload_cast<const std::string &>(&tg::tags::Tags::software))
      .def_property("exposure_time", 
            py::overload_cast<void>(&tg::tags::Tags::exposureTime, py::const_),
            py::overload_cast<double>(&tg::tags::Tags::exposureTime))
      .def_property("f_number", 
            py::overload_cast<void>(&tg::tags::Tags::fNumber, py::const_),
            py::overload_cast<double>(&tg::tags::Tags::fNumber))
      .def_property("date_time", 
            py::overload_cast<void>(&tg::tags::Tags::dateTime, py::const_),
            py::overload_cast<uint64_t>(&tg::tags::Tags::dateTime))
      .def_property("subject_distance", 
            py::overload_cast<void>(&tg::tags::Tags::subjectDistance, py::const_),
            py::overload_cast<double>(&tg::tags::Tags::subjectDistance))
      .def_property("light_source", 
            py::overload_cast<void>(&tg::tags::Tags::lightSource, py::const_),
            py::overload_cast<tg::tags::Tags::LightSourceType>(&tg::tags::Tags::lightSource))
      .def_property("flash", 
            py::overload_cast<void>(&tg::tags::Tags::flash, py::const_),
            py::overload_cast<tg::tags::Tags::FlashType>(&tg::tags::Tags::flash))
      .def_property("focal_length", 
            py::overload_cast<void>(&tg::tags::Tags::focalLength, py::const_),
            py::overload_cast<double>(&tg::tags::Tags::focalLength))
      .def_property("colour_space", 
            py::overload_cast<void>(&tg::tags::Tags::colourSpace, py::const_),
            py::overload_cast<tg::tags::Tags::ColourSpaceType>(&tg::tags::Tags::colourSpace))
      .def_property("flash_energy", 
            py::overload_cast<void>(&tg::tags::Tags::flashEnergy, py::const_),
            py::overload_cast<double>(&tg::tags::Tags::flashEnergy))
      .def_property("serial_number", 
            py::overload_cast<void>(&tg::tags::Tags::serialNumber, py::const_),
            py::overload_cast<const std::string &>(&tg::tags::Tags::serialNumber))
      .def_property("lens_model", 
            py::overload_cast<void>(&tg::tags::Tags::lensModel, py::const_),
            py::overload_cast<const std::string &>(&tg::tags::Tags::lensModel))
      .def_property("index_of_refraction", 
            py::overload_cast<void>(&tg::tags::Tags::indexOfRefraction, py::const_),
            py::overload_cast<double>(&tg::tags::Tags::indexOfRefraction))
      .def_property("viewport_index", 
            py::overload_cast<void>(&tg::tags::Tags::viewportIndex, py::const_),
            py::overload_cast<double>(&tg::tags::Tags::viewportIndex))
      .def_property("viewport_thickness", 
            py::overload_cast<void>(&tg::tags::Tags::viewportThickness, py::const_),
            py::overload_cast<double>(&tg::tags::Tags::viewportThickness))
      .def_property("viewport_distance", 
            py::overload_cast<void>(&tg::tags::Tags::viewportDistance, py::const_),
            py::overload_cast<double>(&tg::tags::Tags::viewportDistance))
      .def_property("vignetting", 
            py::overload_cast<void>(&tg::tags::Tags::vignetting, py::const_),
            py::overload_cast<bool>(&tg::tags::Tags::vignetting))
      .def_property("viewportType", 
            py::overload_cast<void>(&tg::tags::Tags::viewportType, py::const_),
            py::overload_cast<tg::tags::Tags::ViewportType>(&tg::tags::Tags::viewportType))
      .def_property("enhancement", 
            py::overload_cast<void>(&tg::tags::Tags::enhancement, py::const_),
            py::overload_cast<tg::tags::Tags::EnhancementType>(&tg::tags::Tags::enhancement))
      .def_property("pixel_size", 
            py::overload_cast<void>(&tg::tags::Tags::pixelSize, py::const_),
            py::overload_cast<const std::vector<uint16_t> &>(&tg::tags::Tags::pixelSize))
      .def_property("matrix_nav_to_camera", 
            py::overload_cast<void>(&tg::tags::Tags::matrixNavToCamera, py::const_),
            py::overload_cast<const std::vector<double> &>(&tg::tags::Tags::matrixNavToCamera))
      .def_property("image_number", 
            py::overload_cast<void>(&tg::tags::Tags::imageNumber, py::const_),
            py::overload_cast<uint32_t>(&tg::tags::Tags::imageNumber))
      .def_property("water_depth", 
            py::overload_cast<void>(&tg::tags::Tags::waterDepth, py::const_),
            py::overload_cast<double>(&tg::tags::Tags::waterDepth))
      .def_property("bayer_pattern", 
            py::overload_cast<void>(&tg::tags::Tags::bayerPattern, py::const_),
            py::overload_cast<tg::tags::Tags::BayerPatternType>(&tg::tags::Tags::bayerPattern))
      .def_property("frameRate", 
            py::overload_cast<void>(&tg::tags::Tags::frameRate, py::const_),
            py::overload_cast<double>(&tg::tags::Tags::frameRate))
      .def_property("camera_matrix", 
            py::overload_cast<void>(&tg::tags::Tags::cameraMatrix, py::const_),
            py::overload_cast<const std::vector<double> &>(&tg::tags::Tags::cameraMatrix))
      .def_property("distortion", 
            py::overload_cast<void>(&tg::tags::Tags::distortion, py::const_),
            py::overload_cast<const std::vector<double> &>(&tg::tags::Tags::distortion))
      .def_property("pose", 
            py::overload_cast<void>(&tg::tags::Tags::pose, py::const_),
            py::overload_cast<const std::vector<double> &>(&tg::tags::Tags::pose))
      .def_property("vehicle_altitude", 
            py::overload_cast<void>(&tg::tags::Tags::vehicleAltitude, py::const_),
            py::overload_cast<double>(&tg::tags::Tags::vehicleAltitude))
      .def_property("latitude_ref", 
            py::overload_cast<void>(&tg::tags::Tags::latitudeRef, py::const_),
            py::overload_cast<tg::tags::Tags::LatitudeRefType>(&tg::tags::Tags::latitudeRef))
      .def_property("latitude", 
            py::overload_cast<void>(&tg::tags::Tags::latitude, py::const_),
            py::overload_cast<double>(&tg::tags::Tags::latitude))
      .def_property("longitude_ref", 
            py::overload_cast<void>(&tg::tags::Tags::longitudeRef, py::const_),
            py::overload_cast<tg::tags::Tags::LongitudeRefType>(&tg::tags::Tags::longitudeRef))
      .def_property("longitude", 
            py::overload_cast<void>(&tg::tags::Tags::longitude, py::const_),
            py::overload_cast<double>(&tg::tags::Tags::longitude))
      .def_property("altitude_ref", 
            py::overload_cast<void>(&tg::tags::Tags::altitudeRef, py::const_),
            py::overload_cast<tg::tags::Tags::AltitudeRefType>(&tg::tags::Tags::altitudeRef))
      .def_property("altitude", 
            py::overload_cast<void>(&tg::tags::Tags::altitude, py::const_),
            py::overload_cast<double>(&tg::tags::Tags::altitude))
      .def_property("pps_time", 
            py::overload_cast<void>(&tg::tags::Tags::ppsTime, py::const_),
            py::overload_cast<uint64_t>(&tg::tags::Tags::ppsTime))
      ;



}