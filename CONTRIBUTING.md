# Contributing
- Build: cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build -j
- Tests: ctest --test-dir build --output-on-failure
- DCO: sign-off commits with -s
