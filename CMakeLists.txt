cmake_minimum_required(VERSION 3.1)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/cmake/toolchain.cmake")

include(HunterGate)
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.19.158.tar.gz"
    SHA1 "7bd94f374bca4eff51292ca6ba029b5d688744db"
    LOCAL
)

project(perftest VERSION 0.0.1)

set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_STANDARD 11)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR
   CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pedantic")
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-private-field")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS true)

hunter_add_package(benchmark)
find_package(benchmark CONFIG REQUIRED)

set(benchmarks istream)

foreach(benchmark_name IN LISTS benchmarks)
  add_executable(${benchmark_name}
    ${PROJECT_SOURCE_DIR}/src/${benchmark_name}.cpp)
  target_link_libraries(${benchmark_name} PUBLIC benchmark::benchmark)
endforeach()
