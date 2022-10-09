cmake_minimum_required(VERSION 3.22)
set(CMAKE_VERBOSE_MAKEFILE on )
SET(CMAKE_BUILD_TYPE "Debug")


project(embedlua)

set(CMAKE_CXX_STANDARD 17)
file(GLOB CPP_SRC
      "src/*cpp"
      "src/tests/*cpp"
)

add_executable(embedlua ${CPP_SRC})

target_link_libraries(embedlua lua5.4 gtest)

add_custom_target(run
     COMMAND ./embedlua  
)
add_dependencies(run embedlua)