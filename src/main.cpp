/**
 * main.cpp
 * 
 * Copyright 2G Robotics Inc., 2021
 * 
 * This file contains the logic for a simple command line 2G exif parser.
 * 
 */
#include "cxxopts/cxxopts.hpp"
#include "EXIFTags/Tags.h"
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

int main (int argc, char * argv[]) {
    cxxopts::Options options("exif2Gtool", "Parser exif tags from 2G files.");

    std::string filename;
    options.add_options()
        ("filename", "Input filename", cxxopts::value<std::string>(filename))
        ("v,verbose", "Verbose output", cxxopts::value<bool>()->default_value("false"))
        ;

    options.parse_positional({"filename"});
    options.allow_unrecognised_options();

    auto result = options.parse(argc, argv);
    bool verbose = result["verbose"].as<bool>();

    if (filename == "") {
        std::cerr << "USAGE: exif2Gtool <input filename .tif or .jpg>";
        return -1;
    }

    tg::tags::Tags tags;
    std::string error_message;

    if (!tags.loadHeader (filename, error_message)) {
        std::cerr << error_message << std::endl;
        return -1;
    }

    uint64_t dt = tags.dateTime();
    double lat = tags.latitude();
    double lon = tags.longitude();
    double range = tags.subjectDistance();
    tg::tags::Tags::LatitudeRefType lat_ref = tags.latitudeRef();
    tg::tags::Tags::LongitudeRefType lon_ref = tags.longitudeRef();
    std::string lat_ref_s = lat_ref == tg::tags::Tags::LATITUDEREF_NORTH ? "N" : "S";
    std::string lon_ref_s = lon_ref == tg::tags::Tags::LONGITUDEREF_WEST ? "E" : "W";

    std::cout << dt << ", " ;
    std::cout << std::setprecision(9) << lat; 
    std::cout << lat_ref_s << ", "; 
    std::cout << std::setprecision (9) << lon << lon_ref_s << ", " << range << std::endl;

    return 0;
}