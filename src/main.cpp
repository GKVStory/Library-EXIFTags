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

int main (int argc, char * argv[]) {
    cxxopts::Options options("exif2Gtool", "Parser exif tags from 2G files.");

    std::string positional;
    options.add_options()
        ("positional", "Input filename", cxxopts::value<std::string>(positional))
        ("v,verbose", "Verbose output", cxxopts::value<bool>()->default_value("false"))
        ;

    options.parse_positional({"positional"});
    options.allow_unrecognised_options();

    auto result = options.parse(argc, argv);
    bool verbose = result["verbose"].as<bool>();

    if (positional == "") {
        std::cerr << "USAGE: exif2Gtool <input filename .tif or .jpg>";
        return -1;
    }

    std::cout << "Parsing " << positional << std::endl;
    if (verbose) {
        std::cout << "verbosely" <<std::endl;
    }


    return 0;
}