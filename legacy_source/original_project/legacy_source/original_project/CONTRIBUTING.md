# Contributing to NeonTrafficSimulator

> Maintainer: **@AUSP59** — <alanursapu@gmail.com>  
> Thank you for helping make NeonTrafficSimulator exceptional. This guide explains how to set up your environment, make changes, and submit high-quality contributions.

---

## 1) Ground Rules

- Be kind and respectful — see our **[Code of Conduct](CODE_OF_CONDUCT.md)**.
- Prioritize correctness, determinism, performance, and portability.
- Keep the project dependency-light (C++20 STL preferred).
- Follow the **DCO** (Developer Certificate of Origin) and license policies.

---

## 2) Quick Start (Local)

### Prerequisites
- C++20 compiler (GCC ≥ 11, Clang ≥ 14, or MSVC ≥ 19.36)
- CMake ≥ 3.24
- (Optional) Ninja, Docker, VS Code devcontainer

### Build & Test
```bash
# Release build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j

# Run tests
ctest --test-dir build --output-on-failure
Run a Sample
bash
Copiar código
./build/bin/neon_traffic_cli simulate \
  --network examples/grid4/network \
  --vehicles examples/grid4/vehicles.csv \
  --lights   examples/grid4/lights.csv \
  --duration 300 --tick-ms 100 --out outputs
Docker / Devcontainer
bash
Copiar código
docker build -t neonsim:dev .
docker run --rm -v $PWD:/work neonsim:dev ./build/bin/neon_traffic_cli --version
3) Development Workflow
Fork and create a feature branch from main:

bash
Copiar código
git checkout -b feat/my-improvement
Keep changes small, focused, and tested.

Ensure all checks pass locally (see §6).

Open a Pull Request with a clear description, motivation, and risks.

Address review feedback quickly and respectfully.

4) Style, Lint, and Formatting
Follow .clang-format and .clang-tidy.

Prefer standard containers/algorithms; avoid raw new/delete (use RAII).

No UB; no data races; use const-correctness and [[nodiscard]] when helpful.

Keep public headers clean and minimal.

Pre-commit (optional)
bash
Copiar código
pip install pre-commit
pre-commit install
pre-commit run -a
5) Tests, Benchmarks, Fuzz
Unit tests: add a small binary under tests/unit/ and register it in CMakeLists.txt with add_test.

Determinism: don’t break reproducibility; respect fixed timestep and seed controls.

Benchmarks: place microbenchmarks in benchmarks/. Bench numbers must be reproducible and described (machine/specs).

Fuzzing: fuzz parsers and input loaders under fuzz/ when applicable.

Example unit test skeleton:

cpp
Copiar código
#include "project/network.hpp"
#include <cassert>
int main() {
  neon::RoadNetwork net;
  // ... arrange
  // ... act
  // ... assert
  return 0;
}
6) What CI Requires (Green Checks)
Your PR must pass on ubuntu-latest:

Build & Tests: .github/workflows/ci.yml

Sanitizers (ASan/UBSan): .github/workflows/sanitizers.yml

Determinism sample: .github/workflows/determinism.yml

Workflow YAML parse: .github/workflows/preflight.yml

CodeQL (C/C++): .github/workflows/codeql.yml

Tip: run locally before PR:

bash
Copiar código
# Sanitizers build
cmake -S . -B build-asan -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_FLAGS="-fsanitize=address,undefined -fno-omit-frame-pointer" \
  -DCMAKE_EXE_LINKER_FLAGS="-fsanitize=address,undefined"
cmake --build build-asan -j
ctest --test-dir build-asan --output-on-failure
7) Commit Messages & Sign-off (DCO)
Use Conventional Commits style:

feat: add IDM parameter sweep tool

fix: handle quoted fields in CSV

docs: clarify A* heuristic

Sign-off every commit (DCO):

bash
Copiar código
git commit -s -m "feat: improve lane-change safety (MOBIL-lite)"
This appends:
Signed-off-by: Your Name <you@example.com>

8) Adding/Changing Code
Public APIs: Document in headers (include/project/**), update README.md and docs/.

CLI: Update --help, README examples, and scenario JSON if new flags are added.

I/O formats: Keep CSV/GeoJSON robust; never break backward compatibility without a deprecation note.

Performance: O(·) must be justified; avoid unnecessary allocations in hot loops.

Thread-safety: If you introduce concurrency, include tests and clear invariants.

9) Dependencies & Licensing
Prefer no new dependencies. If essential, choose permissive OSS licenses compatible with Apache-2.0 (e.g., MIT/BSD).

Update LICENSE/NOTICE as needed, maintain headers per REUSE policy, and ensure SBOM stays accurate.

No vendoring large libs without maintainer approval.

10) Security & Responsible Disclosure
Do not file exploitable issues publicly; follow SECURITY.md.

Report privately to @AUSP59 — alanursapu@gmail.com.

Add tests for security fixes; consider CodeQL results.

11) Documentation
Update README.md, docs/WHITEPAPER.md, and relevant design notes.

Public APIs should be discoverable via Doxygen comments.

Keep examples runnable; update examples/ when formats change.

12) Accessibility & Inclusion
Use clear, inclusive language in code and docs.

Provide alt text for images; avoid color-only signals in diagrams.

Keep CLI output readable in plain terminals; don’t rely on ANSI colors only.

13) Release Process (Maintainers)
Ensure main is green and CHANGELOG (if present) is up to date.

Tag SemVer (e.g., v3.0.0), push tag to GitHub.

SBOM and CodeQL run on the tagged release; attach build artifacts if applicable.

Announce changes and migration notes.

14) Getting Help
Open a GitHub Issue with a minimal reproduction.

Ping the maintainer: @AUSP59 — alanursapu@gmail.com.

Thank you for contributing!