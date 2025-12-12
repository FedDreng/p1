# Collect all test source files in this directory
file(GLOB TEST_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/*.c")

foreach(test_src IN LISTS TEST_SOURCES)
  get_filename_component(test_name ${test_src} NAME_WE)
  
  # Add the test executable with test source + needed implementation files
  # (excluding main.c since tests don't need it)
  add_executable(${test_name} 
    ${test_src}
    "${CMAKE_SOURCE_DIR}/src/car_assigner.c"
    "${CMAKE_SOURCE_DIR}/src/lot_matrix_utils.c"
    "${CMAKE_SOURCE_DIR}/src/preferences.c"
    "${CMAKE_SOURCE_DIR}/src/Car_input.c"
    "${CMAKE_SOURCE_DIR}/src/Licenseplate.c"
    "${CMAKE_SOURCE_DIR}/src/navbar.c"
    "${CMAKE_SOURCE_DIR}/src/busyness_prediction.c"
  )
  
  target_include_directories(${test_name} PRIVATE "${CMAKE_SOURCE_DIR}/src")
  
  # Link libraries
  target_link_libraries(${test_name} PRIVATE mtest raylib m)
  
  # Handle CURL for networking
  if(NOT WIN32)
    find_package(CURL REQUIRED)
    target_include_directories(${test_name} PRIVATE ${CURL_INCLUDE_DIRS})
    target_link_libraries(${test_name} PRIVATE ${CURL_LIBRARIES})
    target_compile_definitions(${test_name} PRIVATE USE_LIBCURL)
  else()
    target_link_libraries(${test_name} PRIVATE winhttp)
    target_compile_definitions(${test_name} PRIVATE USE_WINHTTP)
  endif()
  
  target_compile_definitions(${test_name} PUBLIC ASSETS_PATH="${CMAKE_SOURCE_DIR}/assets/")
  discover_tests(${test_name})
endforeach()
