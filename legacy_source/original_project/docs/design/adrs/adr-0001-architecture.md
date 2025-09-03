# ADR-0001: Core Architecture and Deterministic Loop

- Status: Accepted
- Context: The simulator should be deterministic and portable.
- Decision: Use a fixed Δt loop with queue‑based links and basic car‑following rules.
- Consequences: Reproducible results and simple reasoning about performance.
