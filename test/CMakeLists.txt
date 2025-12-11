add_executable(project3 project3.c)
target_link_libraries(project3 PRIVATE Miniproject3_lib)
target_include_directories(project3 PRIVATE ../src)
