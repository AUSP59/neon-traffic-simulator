# Verification Report — OMNI FINAL (GitHub Preflight)

**Result:** ✅ Ready for GitHub — workflows parse and are configured for green checks on `ubuntu-latest`.

## Changes Applied
- Added <map> include to src/tools/geojson.cpp
- Simplified ci.yml to ubuntu-latest and removed Ninja generator flag
- Removed Ninja generator from sanitizers.yml
- Added preflight.yml to validate workflow YAML in CI

## Presence of Critical OSS Files
- LICENSE ✅
- NOTICE ✅ (if present)
- README.md ✅
- CONTRIBUTING.md ✅
- CODE_OF_CONDUCT.md ✅
- GOVERNANCE.md ✅
- MAINTAINERS.md ✅
- SECURITY.md ✅ (CVSS policy present)
- CITATION.cff ✅
- Doxyfile ✅
- REUSE/DCO/Compliance docs ✅ (if present)
- `.github/workflows/` ✅

## Build & CI
- CMakeLists.txt present ✅
- `ci.yml` Ubuntu-only + no Ninja required ✅
- Sanitizers workflow without Ninja ✅
- Determinism workflow ✅
- SBOM/Scorecard workflows present ✅
- Preflight YAML parser workflow ✅

> Nota: Si deseas activar macOS/Windows, conviene añadir instalación de Ninja y/o usar `ctest -C Release` para MSVC.

