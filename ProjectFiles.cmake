

file (GLOB HEADERS "${INCLUDE_PATH}/${PROJECT_NAME}/*.h" "${INCLUDE_PATH}/${PROJECT_NAME}/*.hpp" )

set(SRC
  "${SRC_PATH}/Tags.cpp"
  "${SRC_PATH}/Tag.cpp"
  "${SRC_PATH}/TagConstants.cpp"
  "${SRC_PATH}/ImageHandler.cpp"
)

set(MAIN_SRC 
  "${SRC_PATH}/main.cpp"
)

set(PYTHON_SRC 
  "${SRC_PATH}/ExifTagsPython.cpp"
)

# Set project test source files.
set(TEST_SRC
  "${TEST_SRC_PATH}/TestTags.cpp"
  "${TEST_SRC_PATH}/TestTag.cpp"
  "${TEST_SRC_PATH}/TestTagConstants.cpp"
  "${TEST_SRC_PATH}/TestImageHandler.cpp"
)
