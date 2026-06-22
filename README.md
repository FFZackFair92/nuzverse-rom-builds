# nuzverse-rom-builds

Build-runner PUBBLICO per le ROM Nuzverse (IronMon Kanto/Emerald).
Repo pubblico = GitHub Actions gratis e illimitate.

Il sorgente del decomp resta PRIVATO: viene clonato a build-time dal fork
privato FFZackFair92/nuzverse-expansion tramite il secret FORK_PAT.

## Segreti richiesti (Settings -> Secrets and variables -> Actions)
- FORK_PAT          PAT fine-grained, READ Contents su nuzverse-expansion
- ROM_DEPLOY_TOKEN  token endpoint auto-deploy bps
- ROM_DEPLOY_URL    base URL del sito (es. https://nuzverse.com)

## Build
Actions -> "ROM - Kanto/Emerald variants (FORK)" -> Run workflow
(variant = all, fork_ref = nuzverse/main).
