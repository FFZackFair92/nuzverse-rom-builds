# HANDOFF — Traduzione ROM multilingua + build/deploy

Documento operativo per un agente (Sonnet) che deve **tradurre la ROM in una nuova lingua e
buildarla/deployarla**. L'italiano è già fatto e in produzione. Restano: **es, fr, de, pt**.
Tutto è collaudato: una build IT è verde su tutte e 4 le varianti e il deploy in produzione
funziona. Segui i passi nell'ordine.

---

## 0. Contesto in 30 secondi

- **Fork decomp** (sorgente ROM): `FFZackFair92/nuzverse-expansion`, branch **`nuzverse/main`**,
  clonato in `C:\Users\ashso\Documents\Lavoro\Progetti\Pokemon IronMon\decomp\nuzverse-expansion`.
  Builda 4 varianti: Kanto `firered` + `firered-nuzlocke` (`make firered`), Hoenn `ironmon-full` +
  `nuzlocke-emerald` (`make`). La regione = macro `GAME_VERSION`/`IS_FRLG`.
- **NV_LANG** = flag compile-time (una ROM per lingua). Valori: `LANGUAGE_ENGLISH=2`,
  `LANGUAGE_ITALIAN=4`, `LANGUAGE_FRENCH=3`, `LANGUAGE_GERMAN=5`, `LANGUAGE_SPANISH=7`,
  `NV_LANG_PORTUGUESE=8` (custom). Gen3 non ha switch a runtime.
- **Pipeline i18n**: tutta in `tools/i18n/` del fork.
- **Nomi mosse/abilità**: GIÀ tradotti in-ROM per tutte e 5 le lingue (tabelle `gNvMoveNames`/
  `gNvAbilityNames` in `src/nv_i18n_moves.c`, generate da `pokemon-ironmon/scripts/gen-tracker-i18n.mjs`).
  **Per una nuova lingua NON serve rifarli.** Servono solo: DIALOGHI + DESCRIZIONI mosse/abilità.
- **Build CI**: repo `FFZackFair92/nuzverse-rom-builds`. Workflow: Kanto `300185120`, Emerald `300185118`.
- **Webapp**: già pronta per il selettore lingua (`config/ironmon.php` → `supported_rom_langs` =
  en/it/es/fr/de/pt; ROM servite per `(game_key, variant, lang)` con fallback a `en`).

---

## 1. Strumenti (`tools/i18n/`)

| File | Cosa fa |
|---|---|
| `extract.py` | Genera `catalog.en.json` (≈12995 voci): dialoghi `.string` per LABEL + nomi/descrizioni (COMPOUND_STRING, incluse le **descrizioni mosse multi-riga**). **Gitignored**, si rigenera. |
| `catalog.<lang>.json` | Le traduzioni (chiave=label o `MOVE_X.description`/`ABILITY_X.description`). **Committate.** |
| `translate.mjs <lang>` | Traduttore LLM OpenAI-compatibile (anche GLM locale). Traduce TUTTO `catalog.en` → `catalog.<lang>.json`. Batch+resume. |
| `apply_translation.py <lang>` | INJECT dei dialoghi nei `.inc` in CI prima di `make`. Riscrive i blocchi `.string` per LABEL. |
| `gen_desc.py` | Genera `src/nv_i18n_desc.c` (descrizioni mosse+abilità per NV_LANG) da `catalog.<lang>.json`. **Committare il .c.** |
| `validate.py <lang>` | Controlla placeholder/terminatore/pagine di una traduzione vs EN. Da lanciare PRIMA del build. |
| `_split.py <lang> [N]` | (per il metodo sub-agenti) spezza i dialoghi raggiungibili non tradotti in `parts/src_NN.json`. |

I getter override (`GetMoveName/GetMoveDescription` in `include/move.h`, `GetAbilityName/
GetAbilityDescription` in `include/pokemon.h`) sono GIÀ in place: con `NV_LANG != ENGLISH` usano le
tabelle tradotte, altrimenti l'inglese. Non toccarli.

---

## 2. REGOLE CRITICHE (valgono per OGNI lingua)

Queste 3 cose il compilatore NON le valida ma rompono il testo/il build. Sono già gestite dai tool,
ma chi traduce deve rispettarle:

1. **Codici di controllo invariati e in posizione**: `\n` (a capo), `\l` (scroll), `\p` (nuova
   pagina), terminatore finale `$`. Nel JSON appaiono come `\\n \\l \\p`. **Mai veri a-capo.**
2. **Placeholder invariati**: `{PLAYER} {RIVAL} {STR_VAR_1..3} {KUN} {PLAY_BGM} {MUS_...} {PAUSE ...}
   {PAUSE_MUSIC} {RESUME_MUSIC} {FONT_...} {COLOR ...} {SHADOW ...} {CLEAR_TO ...} {*_ARROW}
   {POKEBLOCK} {PLUS}` e ogni `{TOKEN}`. Un `{PLAYER}` perso = glitch in gioco.
3. **Charmap Gen3**: niente virgolette dritte ASCII `"` (usa le curve “ ”), niente `«» °`
   (vengono normalizzati da `apply_translation`/`gen_desc`, ma meglio non introdurli). Gli accenti
   latini (à é è ì ò ù ñ ç ü ö ä ß) e — per ES/FR — `¿ ¡` sono OK. Lunghezza: una `.string` non
   deve superare ~1024 byte (l'injector spezza ai codici, ma tieni le righe ~26 caratteri).

I tre fix che le garantiscono sono già in `apply_translation.py` (sanitize virgolette→curve,
split a 1024 byte, mappa `«»°`) e in `gen_desc.py` (stessa sanitize per le descrizioni).

### Cosa NON tradurre (tenere in INGLESE MAIUSCOLO)
Nomi di città/luoghi (Kanto **e** Hoenn: PALLET TOWN, CELADON CITY, MT. MOON, ROCK TUNNEL,
LITTLEROOT TOWN, RUSTBORO CITY, ecc. — **NON** italianizzare/spagnolizzare i toponimi), nomi di
specie Pokémon, TEAM ROCKET / TEAM MAGMA / TEAM AQUA, nomi propri dei personaggi, e i "marchi"
oggetto (POKé BALL, MASTER BALL, POKéNAV, POKéDEX). **POKéMON** si scrive sempre con la `é`.

### Glossario termini da TRADURRE
| EN | IT | ES | FR | DE | PT |
|---|---|---|---|---|---|
| TRAINER | ALLENATORE | ENTRENADOR | DRESSEUR | TRAINER | TREINADOR |
| GYM | PALESTRA | GIMNASIO | ARÈNE | ARENA | GINÁSIO |
| GYM LEADER | CAPOPALESTRA | LÍDER DE GIMNASIO | CHAMPION D'ARÈNE | ARENALEITER | LÍDER DE GINÁSIO |
| BADGE | MEDAGLIA | MEDALLA | BADGE | ORDEN | INSÍGNIA |
| POKéMON LEAGUE | LEGA POKéMON | LIGA POKéMON | LIGUE POKéMON | POKéMON-LIGA | LIGA POKéMON |
| ELITE FOUR | SUPERQUATTRO | ALTO MANDO | CONSEIL 4 | TOP VIER | ELITE QUATRO |
| CHAMPION | CAMPIONE | CAMPEÓN | MAÎTRE | CHAMPION | CAMPEÃO |
| POKéMON CENTER | CENTRO POKéMON | CENTRO POKéMON | CENTRE POKéMON | POKéMON-CENTER | CENTRO POKéMON |
| POKéMON MART | POKéMART | TIENDA POKéMON | BOUTIQUE POKéMON | POKéMON-MARKT | LOJA POKéMON |
| TM / HM | MT / MN | MT / MO | CT / CS | TM / VM | MT / MH |
| POTION | POZIONE | POCIÓN | POTION | TRANK | POÇÃO |
| DAD / MOM | PAPÀ / MAMMA | PAPÁ / MAMÁ | PAPA / MAMAN | PAPA / MAMA | PAI / MÃE |
| CONTEST | GARA | CONCURSO | CONCOURS | WETTBEWERB | CONCURSO |

(Le descrizioni mosse/abilità: brevi, max ~2 righe come l'originale, `\n` preservato; termini
stat es. Attack/Defense/Speed → tradotti nella lingua; "foe"→nemico/avversario.)

---

## 3. TRADURRE — due metodi

### Metodo A — GLM locale (LM Studio già configurato dall'utente)
Server OpenAI-compatibile a `http://127.0.0.1:1234`, modello `zai-org/glm-4.6v-flash`.
```bat
cd /d "C:\Users\ashso\Documents\Lavoro\Progetti\Pokemon IronMon\decomp\nuzverse-expansion"
python tools\i18n\extract.py
set NV_TRANSLATE_API_BASE=http://127.0.0.1:1234/v1
set NV_TRANSLATE_API_KEY=lm-studio
set NV_TRANSLATE_MODEL=zai-org/glm-4.6v-flash
set NV_TRANSLATE_BATCH=8
node tools\i18n\translate.mjs es
```
Resumabile: se si pianta, rilancia. `translate.mjs` ha già `response_format:text` + parser tollerante
(LM Studio non accetta `json_object`). Traduce dialoghi **E** descrizioni (catalog.en le contiene
entrambe). Qualità GLM Flash: discreta ma perde più placeholder/codici di un modello grande → il
passo `validate.py` (sotto) è OBBLIGATORIO.

### Metodo B — sub-agenti Claude in parallelo (qualità migliore, è così che è stato fatto l'IT)
Il GENITORE pre-genera le fette, gli agenti traducono SOLO la loro fetta e scrivono `parts/part_NN.json`
(NON toccano `catalog.<lang>.json` né lanciano `_dump`/`extract` — altrimenti corrompono il catalog
in concorrenza). Poi il genitore fa merge.
```bat
python tools\i18n\extract.py
python tools\i18n\_split.py es 16      REM -> parts/src_01..16.json (dialoghi)
```
Per le **descrizioni** (mosse+abilità) servono fette separate: filtra da `catalog.en.json` le chiavi
con `type=="description"` non ancora in `catalog.es.json` e splittale in `parts/dsrc_NN.json`
(stesso pattern; regole "brevi, no `$`, `\n` preservato").
Lancia 6–8 agenti per ondata, ciascuno: legge `src_NN.json` → traduce con il glossario della lingua
→ scrive `part_NN.json` → valida JSON. Poi il genitore fa **merge** in `catalog.es.json`
(normalizzando eventuali toponimi tradotti per errore → riportarli in inglese) e committa.

**Gotcha sub-agenti** (imparati sull'IT): NON far girare `_dump.py`/`extract.py` agli agenti in
parallelo (race → un agente "ripara" e tronca il catalog). Pre-split lato genitore = niente race.

---

## 4. VALIDARE (sempre, prima del build)
```bat
python tools\i18n\validate.py es
```
Stampa ERRORI (placeholder persi/aggiunti, `$` perso) e WARNING (pagine, stringhe identiche a EN).
**Correggi gli ERRORI** in `catalog.es.json` prima di buildare (il build compila lo stesso ma il
testo si rompe in gioco). I WARNING "pagine"/"identica" sono spesso ok.

---

## 5. DESCRIZIONI in-ROM (codegen)
```bat
python tools\i18n\gen_desc.py
```
Rigenera `src/nv_i18n_desc.c` con i blocchi `#if NV_LANG == ...` di TUTTE le lingue che hanno un
`catalog.<lang>.json`. Sanity rapido: il file deve avere graffe bilanciate e ~1248 `COMPOUND_STRING`
per lingua tradotta.

---

## 6. COMMIT + PUSH (fork)
Usa **Desktop Commander / git Windows** (MAI git via bash sul mount: corrompe i blob). NIENTE `->`
nei messaggi di commit (cmd dà "Accesso negato").
```bat
cd /d "...\decomp\nuzverse-expansion"
git add tools/i18n/catalog.es.json src/nv_i18n_desc.c
git commit -m "Traduzione ES (dialoghi + descrizioni)"
git push origin nuzverse/main
```

---

## 7. BUILD + DEPLOY (CI)
Builda e deploya in produzione lo slot di quella lingua (la webapp lo serve come opzione; l'inglese
resta default). I 4 comandi (2 regioni × le varianti):
```bat
gh workflow run 300185120 -R FFZackFair92/nuzverse-rom-builds -f variant=all -f lang=es -f deploy=true   REM Kanto (firered + firered-nuzlocke)
gh workflow run 300185118 -R FFZackFair92/nuzverse-rom-builds -f variant=all -f lang=es -f deploy=true   REM Hoenn (ironmon-full + nuzlocke-emerald)
```
Il workflow: lancia `validate.py` (report) → `apply_translation.py es` → `make ... -DNV_LANG=LANGUAGE_SPANISH`
→ genera bps → con `deploy=true` POSTa a `/api/rom/deploy` con `-F lang=es` (slot lingua, NON sovrascrive EN).

Monitora (build ~5–8 min/variante):
```bat
gh run list -R FFZackFair92/nuzverse-rom-builds -w 300185120 -L 1 --json databaseId,status,conclusion
gh run view <RUN_ID> -R FFZackFair92/nuzverse-rom-builds   REM step "Build variant"; se rosso: --log-failed | findstr error:
```
Se il `make` fallisce, l'errore è quasi sempre uno dei 3 (virgolette/1024/char fuori charmap): il
fix è già nei tool, ma se compare un carattere nuovo aggiorna `_SUB` in `apply_translation.py`
(dialoghi) e in `gen_desc.py` (descrizioni), poi ripusha e rilancia.

### Mapping lang → NV_LANG (già nei workflow)
`it→LANGUAGE_ITALIAN, es→LANGUAGE_SPANISH, fr→LANGUAGE_FRENCH, de→LANGUAGE_GERMAN, pt→NV_LANG_PORTUGUESE`.

---

## 8. Checklist per lingua (copia-incolla)
- [ ] `extract.py` (rigenera catalog.en)
- [ ] traduci → `catalog.<lang>.json` (Metodo A o B) — dialoghi + descrizioni
- [ ] `validate.py <lang>` → 0 ERRORI
- [ ] `gen_desc.py` → `src/nv_i18n_desc.c` aggiornato
- [ ] commit `catalog.<lang>.json` + `src/nv_i18n_desc.c` + push fork
- [ ] `gh workflow run` Kanto + Emerald, `lang=<lang> deploy=true`
- [ ] build verdi (4 varianti) + deploy `{"ok":true}` nei log
- [ ] (la webapp serve già la lingua come opzione; nessun cambio extra)

---

## 9. Riferimenti
- Repo fork: `FFZackFair92/nuzverse-expansion` (privato), branch `nuzverse/main`.
- Repo build: `FFZackFair92/nuzverse-rom-builds`; workflow Kanto `300185120`, Emerald `300185118`.
- Endpoint deploy webapp: `POST /api/rom/deploy` (token `ROM_DEPLOY_TOKEN`, secret in CI).
- Lo stato IT (riferimento "fatto bene"): `catalog.it.json` (9188 voci) già in repo.
