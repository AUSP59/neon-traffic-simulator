# Security Policy

Project: **NeonTrafficSimulator**  
Maintainer: **@AUSP59** — <alanursapu@gmail.com>  
Advisories: GitHub Security Advisories (GHSA) for this repo

---

## 1) Reporting a Vulnerability (Private)

Please email **alanursapu@gmail.com** with the subject **[SECURITY]**. Include:

- Affected version/commit (tag or SHA), OS/toolchain used
- Minimal reproducible steps and PoC (no destructive payloads)
- Impact and likelihood, suggested fix if known
- Your disclosure timeline preferences

**PGP:** A PGP key is available on request; we’ll encrypt follow-ups if needed.

**We acknowledge within 72 hours** and provide an initial triage update **within 7 days**. We keep your identity confidential to the fullest extent legally possible. If the issue involves a maintainer, they will recuse themselves.

---

## 2) Coordinated Disclosure

We prefer coordinated disclosure:
- We fix and prepare patches, tests, and advisories.
- We publish a **GitHub Security Advisory (GHSA)** (and request a **CVE** via GitHub if applicable).
- We credit reporters (opt-in) in the advisory and release notes.

If active exploitation is suspected, we may shorten timelines and fast-track a patch release.

---

## 3) Severity & Target Timelines (CVSS v3.1)

| Severity | Example Score | Target Fix Window |
|---|---:|---|
| Critical | 9.0–10.0 | patch ASAP; public advisory within **72 h** of fix |
| High | 7.0–8.9 | **≤ 7 days** after triage |
| Medium | 4.0–6.9 | **≤ 30 days** |
| Low | < 4.0 | best effort |

We may provide mitigations or configuration guidance when patches are non-trivial.

---

## 4) Scope

In scope:
- Source code in this repository (engine, CLI, tools)
- Build/CI configurations that affect supply chain integrity
- Data parsers (CSV, JSON scenario), output writers (CSV/GeoJSON)

Out of scope:
- Third-party services, OS/compiler bugs, non-security functional defects
- DoS via unreasonable resource exhaustion on pathological inputs (e.g., multi-GB CSVs) unless it bypasses documented limits
- Social engineering, spam, or issues requiring privileged/local attacker without project contribution vectors

---

## 5) Examples

Consider security issues (non-exhaustive):
- Memory safety (UB, OOB, UAF), integer over/underflow leading to memory corruption
- Path traversal or arbitrary file write/read via tool inputs
- Command execution or code injection (none expected; we do not eval)
- Logic flaws enabling privilege or policy bypass in CI/release artifacts
- Supply chain risks: tampering with build outputs, SBOM/provenance

Typically **not** security:
- Formatting quirks, benign crashes on malformed input that do not cross trust boundaries
- Performance-only regressions without security impact

---

## 6) Supported Versions

We use SemVer and maintain the latest major/minor:

| Version | Status |
|---|---|
| **3.0.x** | **Supported** (security fixes) |
| 2.x / 1.x | End of maintenance; please upgrade |

Security fixes will be released as patch versions (e.g., 3.0.1).

---

## 7) Hardening & Supply Chain Practices

- **CodeQL** (C/C++) for static analysis in CI
- **ASan/UBSan** sanitizer jobs; determinism checks on sample scenarios
- **SBOM** (SPDX) on tagged releases; dependency minimization (STL-first)
- **SLSA plan** documented; GitHub releases as the distribution channel
- **No telemetry**; CLI opts are explicit and local
- **REUSE/DCO/License** compliance; provenance to be strengthened with **Sigstore** (planned)

---

## 8) Handling Secrets & Sensitive Data

If you find exposed secrets in the repo (we strive to keep none):
- Email **alanursapu@gmail.com** immediately.
- Do **not** open a public issue.
- We will rotate/revoke and investigate the root cause.

---

## 9) Patch Process

1. Triage and reproduce.
2. Prepare fix with tests and regression coverage.
3. Run CI (build, tests, sanitizers, CodeQL).
4. Publish advisory (GHSA/CVE), release patched versions, and update docs.
5. Credit reporter (opt-in).

---

## 10) Safe Harbor

We will not pursue or support legal action against good-faith security research that:
- Accesses only your own systems and data
- Avoids privacy violations, service degradation, and data exfiltration
- Respects coordinated disclosure and gives us reasonable time to fix
- Follows applicable laws

---

## 11) Contact

**Primary:** **@AUSP59** — <alanursapu@gmail.com>  
Advisories: GHSA for this repository

Thank you for helping keep NeonTrafficSimulator safe and reliable.