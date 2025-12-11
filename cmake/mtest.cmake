# This file downloads mtest to the project (fetches the git repo main branch).
cmake_policy(SET CMP0135 NEW)

include(FetchContent)

FetchContent_Declare(
  mtest
  GIT_REPOSITORY https://github.com/MortenSchou/mtest.git
  GIT_TAG main
)
FetchContent_MakeAvailable(mtest)

message(STATUS "mtest_SOURCE_DIR='${mtest_SOURCE_DIR}'")
if(EXISTS "${mtest_SOURCE_DIR}/cmake/mtest.cmake")
  message(STATUS "Including mtest helper from '${mtest_SOURCE_DIR}/cmake/mtest.cmake'")
  include("${mtest_SOURCE_DIR}/cmake/mtest.cmake")
else()
  message(FATAL_ERROR "mtest.cmake not found at '${mtest_SOURCE_DIR}/cmake/mtest.cmake'.\n"
                      "If you want to use a specific release that doesn't include the cmake helper,\n"
                      "either vendor the file into your_project/cmake/mtest.cmake or switch the GIT_TAG.")
endif()
