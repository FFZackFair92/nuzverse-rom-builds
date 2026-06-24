#!/usr/bin/env python3
# tools/i18n/apply_translation.py <lang>
# INJECT dei dialoghi: sostituisce in-place le stringhe .string dei file .inc con le traduzioni
# del catalogo tools/i18n/catalog.<lang>.json (chiave = LABEL della stringa, come estratto da
# extract.py). Va lanciato in CI PRIMA di `make`, SOLO per le build tradotte (NV_LANG != EN),
# sul sorgente clonato (cosi' non si tocca il sorgente inglese committato).
#
# Il valore tradotto deve essere gia' nel formato GBA: con i codici di controllo (\n \l \p),
# i placeholder ({PLAYER} ecc.) e il terminatore $ — esattamente come l'originale.
import json, os, re, sys

if len(sys.argv) < 2:
    print('uso: apply_translation.py <lang>'); sys.exit(1)
LANG = sys.argv[1]
ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..'))
CAT = os.path.join(os.path.dirname(__file__), f'catalog.{LANG}.json')
if not os.path.isfile(CAT):
    print(f'catalog mancante: {CAT} (nessuna traduzione applicata)'); sys.exit(0)

cat = json.load(open(CAT, encoding='utf-8'))

# Sanitizza la punteggiatura non-ASCII assente nel charmap Gen3 (gli accenti latini restano).
_SUB = {'‘': "'", '’': "'", '“': '"', '”': '"', '–': '-',
        '—': '-', '…': '...', ' ': ' ', ' ': ' '}
def sanitize(s):
    for k, v in _SUB.items():
        s = s.replace(k, v)
    return s

DIRS = [os.path.join(ROOT, 'data', 'text'), os.path.join(ROOT, 'data', 'maps')]
RE_LABEL = re.compile(r'^([A-Za-z_][A-Za-z0-9_]*)::')
RE_STRING = re.compile(r'^\s*\.string\s+"')

applied = 0
files = 0
for base in DIRS:
    for dp, _, fns in os.walk(base):
        for fn in fns:
            if not fn.endswith('.inc'):
                continue
            path = os.path.join(dp, fn)
            lines = open(path, encoding='utf-8', errors='replace').read().splitlines()
            out = []
            i = 0
            changed = False
            while i < len(lines):
                line = lines[i]
                m = RE_LABEL.match(line.strip())
                if m and m.group(1) in cat:
                    out.append(line)  # tieni la label
                    i += 1
                    # consuma il blocco .string originale (righe consecutive)
                    j = i
                    while j < len(lines) and RE_STRING.match(lines[j]):
                        j += 1
                    if j > i:  # c'era almeno una .string
                        out.append('\t.string "' + sanitize(cat[m.group(1)]) + '"')
                        i = j
                        applied += 1
                        changed = True
                    continue
                out.append(line)
                i += 1
            if changed:
                open(path, 'w', encoding='utf-8', newline='\n').write('\n'.join(out) + '\n')
                files += 1

print(f'[i18n {LANG}] applicate {applied} stringhe in {files} file (su {len(cat)} voci nel catalogo)')
