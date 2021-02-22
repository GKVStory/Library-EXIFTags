cmake_minimum_required(VERSION 3.10)

project(libexif)

file (GLOB HEADERS "lib/libExif/libexif/*.h" "lib/libExif/libexif/canon/*.h" "lib/libExif/libexif/fuji/*.h" "lib/libExif/libexif/olympus/*.h" "lib/libExif/libexif/pentax/*.h")
file (GLOB SOURCES "lib/libExif/libexif/*.c" "lib/libExif/libexif/canon/*.c" "lib/libExif/libexif/fuji/*.c" "lib/libExif/libexif/olympus/*.c" "lib/libExif/libexif/pentax/*.c")

add_definitions(-DGETTEXT_PACKAGE="libexif-12")

include_directories("lib/libExif/")

add_library(libexif SHARED ${HEADERS} ${SOURCES})
