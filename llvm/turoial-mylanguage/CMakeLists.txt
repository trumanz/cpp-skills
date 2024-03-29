cmake_minimum_required(VERSION 3.20)
project(turoial_mylanguage)

set(CMAKE_CXX_STANDARD 14)

find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})


file(GLOB_RECURSE SRC_FILES
        "src/*.cpp"
        )

add_executable(turoial_mylanguage ${SRC_FILES} )

find_package(log4cplus CONFIG REQUIRED)
target_link_libraries(turoial_mylanguage  log4cplus::log4cplus)

# Find the libraries that correspond to the LLVM components that we wish to use
llvm_map_components_to_libnames(llvm_libs support core irreader)
# Link against LLVM libraries
target_link_libraries( turoial_mylanguage ${llvm_libs})
