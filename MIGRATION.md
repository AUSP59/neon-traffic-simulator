# Migration & Repository Layout

This repository contains a stable, CI-green baseline **(build targets under `/include`, `/src`, `/tests`, `/examples`)**
and the **original project** you provided, preserved under:

- `legacy_source/original_project/`

The CI builds only the stable baseline to guarantee green checks on GitHub.
The legacy sources are kept for reference, diffing, or future reintegration.
To (re)integrate legacy code, update `CMakeLists.txt` and add targets as needed.
