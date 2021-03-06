/**
 * main.cpp
 *
 * Copyright Voyis Inc., 2021
 *
 * This file contains the logic for a simple command line 2G exif parser.
 *
 */
#include "EXIFTags/Tags.h"
#include "cxxopts/cxxopts.hpp"
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
    cxxopts::Options options("exif2Gtool", "Parser exif tags from 2G files.");

    std::string filename;
    options.add_options()("filename", "Input filename", cxxopts::value<std::string>(filename))(
        //"v,verbose", "Verbose output", cxxopts::value<bool>()->default_value("false"))(
        "o,old_file",
        "Use old format",
        cxxopts::value<bool>()->default_value("false"));

    options.parse_positional({"filename"});
    options.allow_unrecognised_options();

    auto result = options.parse(argc, argv);
    // bool verbose = result["verbose"].as<bool>();
    bool old_style = result["old_file"].as<bool>();

    if (filename == "") {
        std::cerr << "USAGE: exif2Gtool <input filename .tif or .jpg>" << std::endl;
        return -1;
    }

    tg::tags::Tags tags;
    std::string error_message;

    if (!tags.loadHeader(filename, error_message)) {
        std::cerr << error_message << std::endl;
        return -1;
    }

    uint64_t dt = tags.dateTime();
    if (dt == 0 || old_style) {
        dt = tags.ppsTime();
    }
    double lat = tags.latitude();
    double lon = tags.longitude();
    double range = tags.subjectDistance();
    tg::tags::Tags::LatitudeRefType lat_ref = tags.latitudeRef();
    tg::tags::Tags::LongitudeRefType lon_ref = tags.longitudeRef();
    std::string lat_ref_s = lat_ref == tg::tags::Tags::LATITUDEREF_NORTH ? "N" : "S";
    std::string lon_ref_s = lon_ref == tg::tags::Tags::LONGITUDEREF_WEST ? "W" : "E";

    tg::tags::Tags::LightSourceType light_source = tags.lightSource();
    double exposure = tags.exposureTime();

    std::cout << dt << ", ";
    if (lat != 0.0 || lon != 0.0) {
        std::cout << std::fixed << std::setprecision(7);
    }
    std::cout << lat << lat_ref_s << ", ";
    std::cout << lon << lon_ref_s << ", ";
    std::cout << std::defaultfloat << range << ", ";
    std::cout << light_source << ", ";
    std::cout << exposure << std::endl;

    return 0;
}
