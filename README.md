# neon-traffic-simulator (Zero-Fail)
Ultra-stable C++20 baseline that always builds and produces deterministic outputs.
- Build: `cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build -j`
- Test: `ctest --test-dir build --output-on-failure`
- Run: `./build/bin/neon_traffic_cli simulate --duration 1 --tick-ms 100 --out outputs`
