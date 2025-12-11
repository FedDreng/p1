cmake_minimum_required(VERSION 4.0)
project(p1 C)
set(CMAKE_C_STANDARD 99)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


include(FetchContent)

FetchContent_Declare(
  mtest
  GIT_REPOSITORY https://github.com/MortenSchou/mtest.git
  GIT_TAG main
)
FetchContent_MakeAvailable(mtest)

message(STATUS "mtest_SOURCE_DIR='${mtest_SOURCE_DIR}'")
if(EXISTS "${mtest_SOURCE_DIR}/cmake/mtest.cmake")
  message(STATUS "Adding mtest cmake/ to CMAKE_MODULE_PATH and including module")
  list(APPEND CMAKE_MODULE_PATH "${mtest_SOURCE_DIR}/cmake")
  include(mtest) # loads cmake/mtest.cmake which registers discover_tests() and the mtest target
else()
  message(WARNING "mtest.cmake not found in fetched mtest. If you prefer, vendor cmake/mtest.cmake "
                  "into your_project/cmake/mtest.cmake and include it directly.")
endif()


# --- Add Raylib ---
set(FETCHCONTENT_QUIET FALSE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG master
)
FetchContent_MakeAvailable(raylib)

# --- Add raygui header (no build target needed) ---
FetchContent_Declare(
  raygui
  GIT_REPOSITORY https://github.com/raysan5/raygui.git
  GIT_TAG master
)
FetchContent_GetProperties(raygui)
if(NOT raygui_POPULATED)
  FetchContent_MakeAvailable(raygui)
endif()
include_directories(${raygui_SOURCE_DIR}/src)

# --- Source files ---
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS 
  "${CMAKE_CURRENT_LIST_DIR}/src/*.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/test/"
)

set(PROJECT_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/src/")

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE})

# Link only raylib
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

# Platform-specific networking
if(WIN32)
  target_compile_definitions(${PROJECT_NAME} PRIVATE USE_WINHTTP)
  target_link_libraries(${PROJECT_NAME} PRIVATE winhttp)
else()
  find_package(CURL REQUIRED)
  target_include_directories(${PROJECT_NAME} PRIVATE ${CURL_INCLUDE_DIRS})
  target_link_libraries(${PROJECT_NAME} PRIVATE ${CURL_LIBRARIES})
  target_compile_definitions(${PROJECT_NAME} PRIVATE USE_LIBCURL)
endif()

target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/")

enable_testing()
add_subdirectory(test)

# Linux needs access to syscall numbers and O_DIRECTORY
#if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
#    message(STATUS "Configuring for Linux (using SYS_getdents syscall)")
#    target_compile_definitions(listfiles PRIVATE __linux__)
#endif()

# Windows needs no extra libraries
#if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
#    message(STATUS "Configuring for Windows (using popen + dir /b)")
#endif()






