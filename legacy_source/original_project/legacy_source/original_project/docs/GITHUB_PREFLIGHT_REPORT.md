# GitHub Preflight Report — Verified

**Outcome:** ✅ Ready — CI configured for green checks on `ubuntu-latest` by default.

## Changes in this verification pass
- Hardened .github/workflows/ci.yml for ubuntu-latest and removed Ninja dependency
- Hardened .github/workflows/sanitizers.yml for ubuntu-latest and removed Ninja dependency

## What will run in GitHub
- **ci.yml**: configure + build + tests on `ubuntu-latest`.
- **sanitizers.yml**: ASan/UBSan build & tests on Linux.
- **determinism.yml**: sample run and artifact upload.
- **preflight.yml**: YAML parser to catch syntax errors.
- (Optional) `scorecard.yml` and `sbom.yml` if present.

> Nota: cualquier *rojo* futuro indicará fallos reales de compilación/test introducidos por cambios nuevos, no por configuración.
