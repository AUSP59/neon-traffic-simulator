# NeonTrafficSimulator

Deterministic, high-performance **C++20** traffic simulation engine with lane-based dynamics (IDM + MOBIL-lite), **A\*/Dijkstra** routing, CSV/GeoJSON I/O, and a hardened OSS stack (CI, security, governance). Production-ready, portable, and dependency-light.

[![CI](https://img.shields.io/badge/CI-ubuntu--latest-brightgreen)](#) [![CodeQL](https://img.shields.io/badge/CodeQL-enabled-blue)](#) [![License](https://img.shields.io/badge/license-Apache--2.0-green)](LICENSE)

> Maintainer: **@AUSP59** · <alanursapu@gmail.com> · Version: **3.0.0**

---

## Features

- **Model**
  - Multi-lane edges with **IDM** longitudinal dynamics and **MOBIL-lite** lane changes.
  - Signal plans (NS/EW phases, amber, offsets).
  - Reproducible fixed-timestep loop; optional `--seed` placeholder for stochastic scenarios.
- **Routing**: **A\*** (time-heuristic) by default; **Dijkstra** fallback (`--router`).
- **Interfaces**
  - CLI (`neon_traffic_cli`) and **JSON scenario runner** (`neon_scenario`).
  - Tools: **gridgen**, **vehgen**, **validate**, **geojson**.
- **I/O & Outputs**
  - Robust CSV parser (quoted fields, commas).
  - `outputs/metrics.csv` (summary), `outputs/metrics_timeseries.csv` (per-tick), `outputs/events.csv`, `manifest.json`.
- **Engineering**
  - Modern **CMake** (+ presets), `install` targets, **Dockerfile** (multi-stage), VS Code **devcontainer**.
  - CI: build/tests, sanitizers (ASan/UBSan), determinism check, **CodeQL**, SBOM on tagged releases.
- **OSS Quality**
  - **Apache-2.0**, NOTICE, DCO, **REUSE**; **SECURITY.md** (CVSS), Threat Model; **CODE_OF_CONDUCT**, **CONTRIBUTING**, **GOVERNANCE**.

---

## Quick Start

### Prerequisites
- C++20 toolchain: GCC ≥ 11 / Clang ≥ 14 / MSVC ≥ 19.36  
- CMake ≥ 3.24

### Build & Test
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
ctest --test-dir build --output-on-failure
Run a Sample
bash
Copiar código
./build/bin/neon_traffic_cli simulate \
  --network examples/grid4/network \
  --vehicles examples/grid4/vehicles.csv \
  --lights   examples/grid4/lights.csv \
  --duration 600 --tick-ms 100 \
  --router astar \
  --out outputs
# Outputs: outputs/{metrics.csv, metrics_timeseries.csv, events.csv, manifest.json}
Docker (optional)
bash
Copiar código
docker build -t neonsim:dev .
docker run --rm -v $PWD:/work neonsim:dev ./build/bin/neon_traffic_cli --version
Command-line Tools
neon_traffic_cli
simulate options:

php-template
Copiar código
--network <dir>   # nodes.csv, edges.csv
--vehicles <file> # vehicles.csv
--lights <file>   # lights.csv
--duration <sec>  --tick-ms <ms>  --router astar|dijkstra  --out <dir>  [--seed <int>]
neon_scenario
Run end-to-end from JSON:

bash
Copiar código
./build/bin/neon_scenario --config examples/scenario.json
neon_gridgen (grid network generator)

bash
Copiar código
./build/bin/neon_gridgen --rows 4 --cols 4 --len 100 --speed 10 --cap 10 --out examples/grid4/network
neon_vehgen (vehicle arrivals, Poisson)

bash
Copiar código
./build/bin/neon_vehgen --count 200 --origin 1 --destination 3 --rate 0.5 --out examples/grid4/vehicles.csv
neon_validate (basic network sanity checks)

bash
Copiar código
./build/bin/neon_validate --network examples/grid4/network
neon_geojson (export to GeoJSON)

bash
Copiar código
./build/bin/neon_geojson --network examples/grid4/network --out outputs/network.geojson
Data Formats
network/nodes.csv
column	type	notes
id	int	unique node id
x	double	position (x)
y	double	position (y)

network/edges.csv
column	type	notes
id	int	unique edge id
from	int	node id
to	int	node id
length	double	meters
speed_limit	double	m/s (free-flow reference)
capacity	int	max vehicles on edge
lanes	int	optional, default 1 (if absent)

vehicles.csv
column	type	notes
id	int	unique vehicle id
origin	int	node id
destination	int	node id
depart_time_s	double	seconds since t=0

lights.csv
column	type	notes
node	int	signalized node id
ns_g	double	north/south green (s)
ew_g	double	east/west green (s)
amber	double	amber time (s)
offset	double	cycle offset (s)

Outputs
metrics.csv — final snapshot: time_s,active,finished,mean_travel_time_s,throughput_vps

metrics_timeseries.csv — per tick time series with same columns

events.csv — vehicle_id,type,time_s,ref_id (depart/arrive/edge_enter/edge_exit)

manifest.json — metadata (engine, version, tick, duration)

Configuration via JSON (Scenario)
json
Copiar código
{
  "network": "examples/grid4/network",
  "vehicles": "examples/grid4/vehicles.csv",
  "lights": "examples/grid4/lights.csv",
  "duration": 600,
  "tick_ms": 100,
  "router": "astar",
  "out": "outputs"
}
Determinism & Reproducibility
Fixed-timestep updates; ordered lane processing prevents overtaking artifacts.

CLI accepts --seed (reserved for stochastic extensions).

CI includes a determinism workflow to keep results stable across changes.

Build Options (CMake)
NEON_BUILD_TESTS (ON) — unit tests via CTest

NEON_BUILD_BENCH (ON) — microbenchmarks

NEON_BUILD_FUZZ (ON) — fuzz harness for parsers
Install:

bash
Copiar código
cmake --install build --prefix dist
Project Structure
bash
Copiar código
include/               # Public headers (API)
src/                   # Engine, CLI, tools
examples/              # Ready-to-run samples (grid4, scenario.json)
docs/                  # Whitepaper, design, policies, reports
tests/                 # Unit tests
.github/workflows/     # CI (build/tests, sanitizers, determinism, CodeQL, SBOM)
Security & Responsible Disclosure
Please review SECURITY.md.
Report privately to @AUSP59 — alanursapu@gmail.com. CVSS-based triage; coordinated disclosure policy.

Contributing & Community
Read CONTRIBUTING.md for setup, style, tests, DCO/sign-off.

Participate respectfully — see CODE_OF_CONDUCT.md.

Governance and roles: GOVERNANCE.md · Maintainers: MAINTAINERS.md

License & Attribution
Licensed under Apache-2.0.

NOTICE, REUSE, DCO included.

Cite this project via CITATION.cff.

Citation (example)
scss
Copiar código
Saavedra, A. (2025). NeonTrafficSimulator (v3.0.0) [Software]. Apache-2.0.
Roadmap (high-level, non-breaking)
Optional calibration against real datasets.

Sigstore signing & provenance attestations.

Extended signal control (turn phases) behind flags.
