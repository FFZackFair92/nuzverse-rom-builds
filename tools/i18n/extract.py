#!/usr/bin/env python3
# tools/i18n/extract.py
# Estrae TUTTE le stringhe traducibili del ROM (decomp) in un catalogo per la pipeline di
# traduzione. Due fonti:
#   1) .string "..."  con LABEL  -> dialoghi/menu (chiave stabile = la label)
#   2) COMPOUND_STRING("...")    -> nomi/descrizioni nelle tabelle dati (chiave = file+indice)
# Output: tools/i18n/catalog.en.json  +  statistiche di volume.
#
# I codici di controllo del testo GBA ({PLAYER}, {STR_VAR_1}, \n \l \p, ecc.) sono PRESERVATI
# come parte del testo: la traduzione dovra' mascherarli/ripristinarli (vedi pipeline).
import os, re, json, sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..'))
OUT = os.path.join(os.path.dirname(__file__), 'catalog.en.json')

# cartelle dove vivono i testi
TEXT_DIRS = [os.path.join(ROOT, 'data', 'text'), os.path.join(ROOT, 'data', 'maps')]
DATA_FILES = [
    os.path.join(ROOT, 'src', 'data', 'moves_info.h'),
    os.path.join(ROOT, 'src', 'data', 'abilities.h'),
    os.path.join(ROOT, 'src', 'data', 'items.h'),
]

# .string con label che la precede: "Label::\n\t.string \"...\"$"
RE_LABEL = re.compile(r'^([A-Za-z_][A-Za-z0-9_]*)::')
RE_STRING = re.compile(r'\.string\s+"((?:[^"\\]|\\.)*)"')
RE_COMPOUND = re.compile(r'COMPOUND_STRING\(\s*"((?:[^"\\]|\\.)*)"\s*\)')

def control_codes(s):
    return sorted(set(re.findall(r'\{[A-Z0-9_]+\}|\\[nlp]', s)))

catalog = {}
stats = {'dialogue': 0, 'data': 0, 'chars': 0, 'files': 0}

# --- 1) dialoghi/menu: .string con label ---
for base in TEXT_DIRS:
    for dirpath, _, files in os.walk(base):
        for fn in files:
            if not fn.endswith('.inc'):
                continue
            path = os.path.join(dirpath, fn)
            try:
                lines = open(path, encoding='utf-8', errors='replace').read().splitlines()
            except Exception:
                continue
            stats['files'] += 1
            cur_label = None
            rel = os.path.relpath(path, ROOT).replace('\\', '/')
            for i, line in enumerate(lines):
                m = RE_LABEL.match(line.strip())
                if m:
                    cur_label = m.group(1)
                sm = RE_STRING.search(line)
                if sm:
                    text = sm.group(1)
                    # piu' .string consecutive appartengono alla stessa label (testo multi-riga)
                    key = cur_label if cur_label else f'{rel}:{i}'
                    if key in catalog:
                        catalog[key]['en'] += text
                    else:
                        catalog[key] = {'en': text, 'src': rel, 'type': 'dialogue'}
                        stats['dialogue'] += 1

# --- 2) nomi/descrizioni tabelle dati: COMPOUND_STRING ---
RE_INDEX = re.compile(r'\[(MOVE_[A-Z0-9_]+|ABILITY_[A-Z0-9_]+|ITEM_[A-Z0-9_]+)\]')
for path in DATA_FILES:
    if not os.path.isfile(path):
        continue
    rel = os.path.relpath(path, ROOT).replace('\\', '/')
    text = open(path, encoding='utf-8', errors='replace').read()
    cur_idx = None
    field = None
    n = 0
    for line in text.splitlines():
        im = RE_INDEX.search(line)
        if im:
            cur_idx = im.group(1); n = 0
        # campo .name / .description prima del COMPOUND_STRING
        fm = re.search(r'\.(name|description)\s*=', line)
        if fm:
            field = fm.group(1)
        cm = RE_COMPOUND.search(line)
        if cm and cur_idx and field:
            s = cm.group(1)
            if not s or s == '-':
                continue
            key = f'{cur_idx}.{field}'
            if key in catalog:
                catalog[key]['en'] += s
            else:
                catalog[key] = {'en': s, 'src': rel, 'type': field}
                stats['data'] += 1

# codici di controllo + lunghezza, per ogni voce
for k, v in catalog.items():
    v['codes'] = control_codes(v['en'])
    v['len'] = len(re.sub(r'\{[^}]+\}|\\[nlp]', '', v['en']))
    stats['chars'] += v['len']

os.makedirs(os.path.dirname(OUT), exist_ok=True)
json.dump(catalog, open(OUT, 'w', encoding='utf-8'), ensure_ascii=False, indent=0)

print(f"Catalogo -> {OUT}")
print(f"  voci totali: {len(catalog)}")
print(f"  - dialoghi/menu (.string): {stats['dialogue']}")
print(f"  - nomi/descrizioni (COMPOUND_STRING): {stats['data']}")
print(f"  file .inc scansionati: {stats['files']}")
print(f"  caratteri totali (no codici): ~{stats['chars']}")
print(f"  stima parole: ~{stats['chars']//6} | x6 lingue: ~{stats['chars']*6//6} parole-target")
