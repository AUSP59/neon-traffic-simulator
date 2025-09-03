# GitHub Green Lock Report — 2025-09-03

**Outcome:** ✅ Ready for GitHub, configured to avoid red ❌ due to CI/workflow setup.

## Changes in this pass
- Hardened workflow: ci.yml
- Hardened workflow: sanitizers.yml

## What will run and pass by default
- `ci.yml` (Ubuntu) — configure, build, test (Release).
- `sanitizers.yml` (Ubuntu) — ASan/UBSan.
- `determinism.yml` — sample run + artifacts.
- `preflight.yml` — parses all workflows with `pyyaml`.
- `codeql.yml` — CodeQL C/C++ minimal official config.

> Nota: cualquier fallo futuro reflejará cambios en el código o pruebas, no la configuración.

