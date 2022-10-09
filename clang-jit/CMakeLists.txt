cmake_minimum_required(VERSION 3.22)
set(CMAKE_VERBOSE_MAKEFILE on )


project(clangjit)

set(CMAKE_CXX_STANDARD 17)

add_executable(clangjit src/main.cpp)

include_directories("/usr/lib/llvm-14/include")


find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}, LLVM_INCLUDE_DIRS: ${LLVM_INCLUDE_DIRS}, LLVM_LIBRARY_DIR: ${LLVM_LIBRARY_DIR}")
include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIR})
add_definitions(${LLVM_DEFINITIONS})
#llvm-config --components
#llvm-config-14   --libs  --link-static    codegen
#llvm_map_components_to_libnames(llvm_libs codegen)
#target_link_libraries(clangjit ${llvm_libs})


target_link_libraries(clangjit LLVM LLVM-14 clang clang-cpp clang-cpp14)