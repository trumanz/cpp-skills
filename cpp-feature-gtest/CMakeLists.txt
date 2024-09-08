cmake_minimum_required(VERSION 3.10)
set(CMAKE_VERBOSE_MAKEFILE ON)
# Set the project name
project(MyGTestProject)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(src)



# Add the source files
add_library(my_math src/my_math.cpp)

# Add Google Test
# add_subdirectory(googletest)  # If using submodule, otherwise see the alternative below




 
# Find Google Test
find_package(GTest REQUIRED)
message(STATUS "GTest include directories: ${GTEST_INCLUDE_DIRS}")
message(STATUS "GTest libraries: ${GTEST_LIBRARIES}")
include_directories(${GTEST_INCLUDE_DIRS})


# Manually set OpenMP flags for Clang on macOS
if(APPLE)
   message(STATUS "Compile on Apple")
   set(OpenMP_C_FLAGS "-Xpreprocessor -fopenmp -I/opt/homebrew/opt/libomp/include")
   set(OpenMP_C_LIB_NAMES "omp")
   set(OpenMP_CXX_FLAGS "-Xpreprocessor -fopenmp -I/opt/homebrew/opt/libomp/include")
   set(OpenMP_CXX_LIB_NAMES "omp")
   set(OpenMP_omp_LIBRARY "/opt/homebrew/opt/libomp/lib/libomp.dylib")
   
   # Apply these flags to the compiler and linker
   set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
   set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L/opt/homebrew/opt/libomp/lib -lomp")
else()
    find_package(OpenMP REQUIRED)
endif()

# Add the OpenMP flags to the compiler
if(OpenMP_CXX_FOUND)
    target_compile_options(runTests PUBLIC ${OpenMP_CXX_FLAGS})
endif()


# Find SQLite3 library
find_package(SQLite3 REQUIRED)
 



# Add the test executable
file(GLOB_RECURSE SRC_FILES  ./test/*.cpp)
message(STATUS "Source files: ${SRC_FILES}")
add_executable(runTests ${SRC_FILES})
# Link test executable against gtest & gtest_main
target_link_libraries(runTests ${GTEST_LIBRARIES} SQLite::SQLite3 ${OpenMP_CXX_LIB_NAMES} pthread my_math)
