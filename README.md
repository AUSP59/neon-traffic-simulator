# neon-traffic-simulator (Green Baseline)
Minimal, deterministic C++20 traffic simulator stub that compiles fast and passes CI.
Build:
```
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
ctest --test-dir build --output-on-failure
```
Run sample:
```
./build/bin/neon_traffic_cli simulate --network examples/grid4/network --vehicles examples/grid4/vehicles.csv --lights examples/grid4/lights.csv --duration 10 --tick-ms 100 --out outputs
```


> The original project you provided is preserved in `legacy_source/original_project/` and not built by default.
