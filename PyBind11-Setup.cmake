# Download pybind11 at configure time
IF (NOT TARGET pybind11)
  configure_file(PyBind11.txt.in ${PROJECT_BINARY_DIR}/pybind11-download/CMakeLists.txt)
  execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
    RESULT_VARIABLE result
  WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/pybind11-download )
  if(result)
    message(FATAL_ERROR "CMake step for pybind11 failed: ${result}")
  endif()

  execute_process(COMMAND ${CMAKE_COMMAND} --build .
    RESULT_VARIABLE result
  WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/pybind11-download )
  if(result)
    message(FATAL_ERROR "Build step for pybind11 failed: ${result}")
  endif()

  # Add pybind11 directly to our build. This defines
  # the gtest and gtest_main targets.
  add_subdirectory(${PROJECT_BINARY_DIR}/pybind11-src
    ${PROJECT_BINARY_DIR}/pybind11-build
  EXCLUDE_FROM_ALL)

  # The gtest/gtest_main targets carry header search path
  # dependencies automatically when using CMake 2.8.11 or
  # later. Otherwise we have to add them here ourselves.
  include_directories(SYSTEM "${PYBIND11_INCLUDE_DIR}")
ENDIF()