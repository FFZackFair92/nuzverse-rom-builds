#!/usr/bin/env python3
# tools/i18n/validate.py <lang>
# Controlla una traduzione (catalog.<lang>.json) contro l'originale (catalog.en.json)
# PRIMA del build. Pensato per output di traduttori esterni (GLM/LLM locali) che a volte
# perdono placeholder o codici di controllo: il build C/assembler NON se ne accorge
# (compila lo stesso) ma il testo si rompe in gioco.
#
# Controlli (in ordine di gravita'):
#   [ERRORE]  placeholder mancanti/aggiunti  ({PLAYER}, {STR_VAR_1}, {PAUSE ...}, {MUS_*}...)
#   [ERRORE]  terminatore $ perso/aggiunto rispetto all'originale (solo dialoghi)
#   [WARN]    \p (cambio pagina) in numero diverso dall'originale
#   [WARN]    stringa identica all'inglese (probabile NON tradotta)
#   [INFO]    chiavi non tradotte (presenti in EN, assenti nella lingua)
#
# Exit code != 0 se ci sono ERRORI (placeholder o $), cosi' si puo' bloccare la CI.
import json, os, re, sys
from collections import Counter

if len(sys.argv) < 2:
    print('uso: validate.py <lang>  (es. validate.py es)'); sys.exit(2)
LANG = sys.argv[1]
here = os.path.dirname(os.path.abspath(__file__))
en = json.load(open(os.path.join(here, 'catalog.en.json'), encoding='utf-8'))
lp = os.path.join(here, f'catalog.{LANG}.json')
if not os.path.isfile(lp):
    print(f'manca {lp}'); sys.exit(2)
tr = json.load(open(lp, encoding='utf-8'))

PLACE = re.compile(r'\{[^}]+\}')

def en_text(v):
    return v['en'] if isinstance(v, dict) else v

errors = []      # (key, tipo, dettaglio)
warns = []
identical = 0
untranslated = [k for k in en if k not in tr]

for k, tv in tr.items():
    if k not in en:
        warns.append((k, 'chiave-extra', 'non presente in EN'))
        continue
    ev = en_text(en[k])
    # placeholder: confronto multiset (un {PLAYER} perso o aggiunto = ERRORE)
    pe, pt = Counter(PLACE.findall(ev)), Counter(PLACE.findall(tv))
    if pe != pt:
        miss = list((pe - pt).elements())
        extra = list((pt - pe).elements())
        det = []
        if miss:  det.append('persi: ' + ' '.join(miss))
        if extra: det.append('aggiunti: ' + ' '.join(extra))
        errors.append((k, 'placeholder', '; '.join(det)))
    # terminatore $: solo dialoghi (le descrizioni MOVE_/ABILITY_ non ce l'hanno)
    if not (k.startswith('MOVE_') or k.startswith('ABILITY_')):
        if ev.endswith('$') and not tv.endswith('$'):
            errors.append((k, 'terminatore', '$ finale perso'))
        elif not ev.endswith('$') and tv.endswith('$'):
            errors.append((k, 'terminatore', '$ finale aggiunto'))
    # \p pagine
    if ev.count('\\p') != tv.count('\\p'):
        page_count_en = ev.count('\p')
        page_count_tv = tv.count('\p')
        warns.append((k, 'pagine', f"\p EN={page_count_en} {LANG}={page_count_tv}"))
    # identica all'inglese
    if tv == ev and ev.strip() not in ('', '-'):
        identical += 1

print(f'=== Validazione catalog.{LANG}.json ===')
print(f'voci tradotte: {len(tr)} | non tradotte (assenti): {len(untranslated)} | identiche a EN: {identical}')
print(f'ERRORI: {len(errors)} | warning: {len(warns)}')
print()
if errors:
    print('--- ERRORI (placeholder/terminatore: vanno corretti, rompono il testo in gioco) ---')
    for k, t, d in errors[:60]:
        print(f'  [{t}] {k}: {d}')
    if len(errors) > 60:
        print(f'  ... e altri {len(errors)-60}')
    print()
if warns:
    print(f'--- WARNING (primi 30 di {len(warns)}) ---')
    for k, t, d in warns[:30]:
        print(f'  [{t}] {k}: {d}')
    print()
print('OK, nessun errore.' if not errors else f'TROVATI {len(errors)} ERRORI: correggili in catalog.{LANG}.json prima del build.')
sys.exit(1 if errors else 0)
