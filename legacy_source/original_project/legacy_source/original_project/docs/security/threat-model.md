# Threat Model

- **Assets:** source code integrity, CI artifacts, release binaries, SBOMs, input datasets.
- **Attack Surfaces:** malicious PRs, dependency substitution, CI secrets, crafted CSV inputs.
- **Mitigations:** CodeQL, SBOM, signed releases, secret scanning, fuzzing and strict CSV parsing, no dynamic code execution.
- **Abuse Cases:** Untrusted inputs; we parse using a robust CSV reader with quote handling.
