cmake_minimum_required(VERSION 3.16)
project(NeonTrafficSimulator LANGUAGES CXX VERSION 3.1.1)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_library(neon_core
  src/core/sim.cpp
)
target_include_directories(neon_core PUBLIC include)
target_compile_features(neon_core PUBLIC cxx_std_20)

add_executable(neon_traffic_cli src/main/cli.cpp)
target_link_libraries(neon_traffic_cli PRIVATE neon_core)

enable_testing()
add_executable(test_basic tests/test_basic.cpp)
target_link_libraries(test_basic PRIVATE neon_core)
add_test(NAME test_basic COMMAND test_basic)
