cmake_minimum_required(VERSION 3.16)
project(NeonTrafficSimulator LANGUAGES CXX VERSION 3.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(NEON_BUILD_TESTS "Build tests" ON)
option(NEON_BUILD_BENCH "Build benchmarks" ON)
option(NEON_BUILD_FUZZ  "Build fuzzers" ON)

add_library(neon_core
    src/project/network.cpp
    src/project/vehicle.cpp
    src/project/traffic_light.cpp
    src/project/simulator.cpp
    src/project/csv.cpp
)
target_include_directories(neon_core PUBLIC include)
target_compile_options(neon_core PRIVATE -Wall -Wextra -Wpedantic)

add_executable(neon_traffic_cli src/main/cli_main.cpp)
target_link_libraries(neon_traffic_cli PRIVATE neon_core)

add_executable(neon_gridgen src/tools/gridgen.cpp)
target_link_libraries(neon_gridgen PRIVATE neon_core)

add_executable(neon_validate src/tools/validate.cpp)
target_link_libraries(neon_validate PRIVATE neon_core)

include(CTest)
if(BUILD_TESTING AND NEON_BUILD_TESTS)
  add_executable(neon_tests
    tests/unit/test_basic.cpp
    tests/unit/test_path.cpp
  )
  target_link_libraries(neon_tests PRIVATE neon_core)
  add_test(NAME neon_tests COMMAND neon_tests)
endif()

if(NEON_BUILD_BENCH)
  add_executable(neon_bench benchmarks/bench_basic.cpp)
  target_link_libraries(neon_bench PRIVATE neon_core)
endif()

if(NEON_BUILD_FUZZ)
  add_executable(neon_fuzz fuzz/fuzz_parser.cpp)
  target_link_libraries(neon_fuzz PRIVATE neon_core)
endif()

install(TARGETS neon_traffic_cli neon_gridgen neon_validate RUNTIME DESTINATION bin)
install(TARGETS neon_core ARCHIVE DESTINATION lib LIBRARY DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)
install(DIRECTORY examples/ DESTINATION share/neon/examples)

# CPack basic settings
include(CPack)
set(CPACK_PACKAGE_NAME "NeonTrafficSimulator")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_GENERATOR "ZIP")

add_executable(neon_geojson src/tools/geojson.cpp)
target_link_libraries(neon_geojson PRIVATE neon_core)

add_executable(neon_vehgen src/tools/vehgen.cpp)
target_link_libraries(neon_vehgen PRIVATE neon_core)

add_executable(test_csv tests/unit/test_csv.cpp)
target_link_libraries(test_csv PRIVATE neon_core)
add_test(NAME test_csv COMMAND test_csv)

add_executable(test_lights tests/unit/test_lights.cpp)
target_link_libraries(test_lights PRIVATE neon_core)
add_test(NAME test_lights COMMAND test_lights)

add_executable(neon_scenario src/tools/scenario.cpp)
target_link_libraries(neon_scenario PRIVATE neon_core)

# Optimize release builds
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

add_executable(test_astar tests/unit/test_astar.cpp)
target_link_libraries(test_astar PRIVATE neon_core)
add_test(NAME test_astar COMMAND test_astar)
