

file (GLOB HEADERS "${INCLUDE_PATH}/${PROJECT_NAME}/*.h" "${INCLUDE_PATH}/${PROJECT_NAME}/*.hpp" )

set(SRC
  "${SRC_PATH}/Tags.cpp"
)

# Set project test source files.
set(TEST_SRC
  "${TEST_SRC_PATH}/TestTags.cpp"
)
