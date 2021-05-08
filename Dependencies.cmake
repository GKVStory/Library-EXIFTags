if (BUILD_PYTHON) 
    include(PyBind11-Setup.cmake)
    include_directories(SYSTEM "${PYBIND11_INCLUDE_DIR}")
endif()

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
set(CONAN_SYSTEM_INCLUDES ON)