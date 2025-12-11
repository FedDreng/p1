# Collect all test source files in this directory
file(GLOB TEST_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/*.c")

foreach(test_src IN LISTS TEST_SOURCES)
  get_filename_component(test_name ${test_src} NAME_WE)
  add_executable(${test_name} ${test_src})
  target_include_directories(${test_name} PRIVATE "${CMAKE_SOURCE_DIR}/src")
  # Link the mtest target provided by the mtest project (FetchContent or vendored)
  target_link_libraries(${test_name} PRIVATE mtest)
  # Allow test runner to see ASSETS_PATH if needed
  target_compile_definitions(${test_name} PUBLIC ASSETS_PATH="${CMAKE_SOURCE_DIR}/assets/")
  # Tell mtest to discover tests in this executable (macro provided by mtest)
  discover_tests(${test_name})
endforeach()
