cmake_minimum_required(VERSION 3.10)

project(libexif)

execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                    RESULT_VARIABLE GIT_SUBMOD_RESULT)

file (GLOB HEADERS "lib/libexif/libexif/*.h" "lib/libexif/libexif/canon/*.h" "lib/libexif/libexif/fuji/*.h" "lib/libexif/libexif/olympus/*.h" "lib/libexif/libexif/pentax/*.h")
file (GLOB SOURCES "lib/libexif/libexif/*.c" "lib/libexif/libexif/canon/*.c" "lib/libexif/libexif/fuji/*.c" "lib/libexif/libexif/olympus/*.c" "lib/libexif/libexif/pentax/*.c")

add_definitions(-DGETTEXT_PACKAGE="libexif-12")

configure_file("lib/config.h" "${CMAKE_CURRENT_SOURCE_DIR}/lib/libexif/config.h" COPYONLY)
configure_file("lib/_stdint.h" "${CMAKE_CURRENT_SOURCE_DIR}/lib/libexif/libexif/_stdint.h" COPYONLY)

#apply patch
if (!GIT_PATCH_RESULT)
    execute_process(COMMAND git apply lib/tiff_support.patch --ignore-space-change --ignore-whitespace --directory=lib/libexif/
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                    RESULT_VARIABLE GIT_PATCH_RESULT)
endif()

include_directories("lib/libexif/")

add_library(libexif ${HEADERS} ${SOURCES})
