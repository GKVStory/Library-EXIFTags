cmake_minimum_required(VERSION 3.10)

project (EXIFTags)
set (PROJECT_NAME EXIFTags)
set (BIN_NAME exif2Gtool)
 
set(CMAKE_CXX_STANDARD 14)

set(PROJECT_PATH                               "${CMAKE_CURRENT_SOURCE_DIR}")
set(INCLUDE_PATH                               "${PROJECT_PATH}/include")
set(SRC_PATH                                   "${PROJECT_PATH}/src")
set(TEST_SRC_PATH                              "${PROJECT_PATH}/tests")
set(LIB_NAME                                   "${PROJECT_NAME}Lib")
set(TEST_NAME                                   "${PROJECT_NAME}Test")
OPTION(BUILD_TESTS                             "Build tests"                     OFF)
OPTION(BUILD_MAIN                              "Build command line parser"       OFF)

configure_file (
    "${PROJECT_SOURCE_DIR}/tests/TestConstants.h.i" 
    "${PROJECT_SOURCE_DIR}/tests/TestConstants.h"
)

if(WIN32)
  if(MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_CRT_SECURE_NO_WARNINGS")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /W3 /FC -D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS -D_WIN32_WINNT=0x0501 ")
  else(MSVC)
    set(CMAKE_CXX_FLAGS
      "${CMAKE_CXX_FLAGS} -Wall -Woverloaded-virtual -Wold-style-cast -Wnon-virtual-dtor -fPIC")
  endif(MSVC)
else(WIN32)
  set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -Wall -Woverloaded-virtual -Wold-style-cast -Wnon-virtual-dtor -fPIC")
endif(WIN32)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g3")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
    set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS}")
    if (ADDRESS_SANITIZE)
        set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -fsanitize=address")
    endif (ADDRESS_SANITIZE)
endif(CMAKE_COMPILER_IS_GNUCXX)

include(BuildLibExif.cmake)
include(Dependencies.cmake)
include(ProjectFiles.cmake)
include_directories(AFTER "${INCLUDE_PATH}")

add_library(${LIB_NAME} ${SRC} ${HEADERS})
target_link_directories(${LIB_NAME} PUBLIC ${CMAKE_BINARY_DIR})
target_link_libraries (${LIB_NAME} libexif)

if(BUILD_TESTS)
  # Download and unpack googletest at configure time
  configure_file(GoogleTest.txt.in googletest-download/CMakeLists.txt)
  execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/googletest-download )
  if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
  endif()
  execute_process(COMMAND ${CMAKE_COMMAND} --build .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/googletest-download )
  if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
  endif()

  
  # Prevent overriding the parent project's compiler/linker
  # settings on Windows
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

  # Add googletest directly to our build. This defines
  # the gtest and gtest_main targets.
  add_subdirectory(${PROJECT_BINARY_DIR}/googletest-src
                 ${PROJECT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

  # The gtest/gtest_main targets carry header search path
  # dependencies automatically when using CMake 2.8.11 or
  # later. Otherwise we have to add them here ourselves.
  if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
  endif()

 
  enable_testing()
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${TEST_PATH})

  add_executable(${TEST_NAME} ${TEST_SRC})

  target_link_libraries(${TEST_NAME} ${LIB_NAME} gtest_main )
  add_test(NAME ${TEST_NAME} COMMAND "${TEST_PATH}/${TEST_NAME}")

  if(BUILD_COVERAGE_ANALYSIS)
    include(CodeCoverage)
    set(COVERAGE_EXTRACT '${PROJECT_PATH}/include/*' '${PROJECT_PATH}/src/*')
    setup_target_for_coverage(coverage "${TEST_NAME}" coverage)
  endif(BUILD_COVERAGE_ANALYSIS)
endif(BUILD_TESTS)

if (BUILD_MAIN)
  add_executable (${BIN_NAME} "${MAIN_SRC}")
  target_link_libraries (${BIN_NAME} ${LIB_NAME})
endif (BUILD_MAIN)
