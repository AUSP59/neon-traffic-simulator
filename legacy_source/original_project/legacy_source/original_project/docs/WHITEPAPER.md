# NeonTrafficSimulator — Whitepaper (ULTRA Edition)

## Abstract
NeonTrafficSimulator is a deterministic, high-performance C++ engine for simulating urban traffic.
The ULTRA edition implements a queue-free car-following rule with time headway and minimal gap,
green-phase gating at intersections, and robust CSV parsing, all while emphasizing supply-chain
security, reproducibility, and accessibility for public-sector decision-making.

## Core Principles
- Deterministic updates with fixed timestep
- Portable, dependency-light runtime (no heavy third-party libs)
- Reproducible experiments (seed control; manifest metadata)
- Observability by design (metrics + events CSV)
- OSS governance, security, and ethics baked in

## Model Overview
- **Edges** are directed links with length, speed limit, capacity.
- **Vehicles** maintain per-edge position and cannot pass.
- **Traffic Lights** gate movements via simple two-phase plan.
- **Dynamics**: front vehicle moves at free-flow; followers maintain `gap = min_gap + headway*speed`.
