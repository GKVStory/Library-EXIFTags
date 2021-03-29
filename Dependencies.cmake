if (BUILD_PYTHON) 
    include(PyBind11-Setup.cmake)
    include_directories(SYSTEM "${PYBIND11_INCLUDE_DIR}")
endif()