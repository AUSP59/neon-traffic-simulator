cmake_minimum_required(VERSION 3.16)
project(NeonTrafficSimulator LANGUAGES CXX VERSION 3.0.3)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(neon_core
  src/project/csv.cpp
  src/project/network.cpp
  src/project/simulator.cpp
)
target_include_directories(neon_core PUBLIC include)
target_compile_features(neon_core PUBLIC cxx_std_20)

add_executable(neon_traffic_cli src/main/cli_main.cpp)
target_link_libraries(neon_traffic_cli PRIVATE neon_core)

add_executable(neon_geojson src/tools/geojson.cpp)
target_link_libraries(neon_geojson PRIVATE neon_core)

enable_testing()
add_executable(test_basic tests/unit/test_basic.cpp)
target_link_libraries(test_basic PRIVATE neon_core)
add_test(NAME test_basic COMMAND test_basic)

install(TARGETS neon_traffic_cli neon_geojson neon_core)
