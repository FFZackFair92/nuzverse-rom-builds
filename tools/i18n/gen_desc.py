#!/usr/bin/env python3
# tools/i18n/gen_desc.py
# Genera src/nv_i18n_desc.c con le tabelle DESCRIZIONI tradotte di MOSSE e ABILITA',
# lette da catalog.<lang>.json (chiavi MOVE_X.description / ABILITY_X.description).
# Selezione a compile-time via NV_LANG. Le voci mancanti restano NULL -> fallback inglese.
import json, os, sys

here = os.path.dirname(os.path.abspath(__file__))
root = os.path.abspath(os.path.join(here, '..', '..'))
OUT = os.path.join(root, 'src', 'nv_i18n_desc.c')

LANGS = {  # lang-code -> macro NV_LANG
    'it': 'LANGUAGE_ITALIAN',
    'es': 'LANGUAGE_SPANISH',
    'fr': 'LANGUAGE_FRENCH',
    'de': 'LANGUAGE_GERMAN',
    'pt': 'NV_LANG_PORTUGUESE',
}

_LDQ = '“'  # virgoletta curva di apertura (charmap B1)
_RDQ = '”'  # virgoletta curva di chiusura (charmap B2)
_SUB = {'‘': "'", '’': "'", '«': _LDQ, '»': _RDQ,
        '–': '-', '—': '-', '…': '...', '°': '',
        ' ': ' '}

def cstr(s):
    # il valore contiene gia' i codici di controllo come backslash-n letterale: tienili.
    # Il charmap Gen3 NON ha la virgoletta dritta ASCII " (ne' dentro COMPOUND_STRING):
    # convertiamo le dritte in curve (alternando apri/chiudi) e normalizziamo i caratteri
    # fuori charmap (guillemette, trattino lungo, ellissi, grado). Cosi' anche l'output di
    # un traduttore esterno (GLM) compila. I backslash (\n \l \p) restano.
    for k, v in _SUB.items():
        s = s.replace(k, v)
    out = []
    oq = True
    for ch in s:
        if ch == '"':
            out.append(_LDQ if oq else _RDQ)
            oq = not oq
        else:
            out.append(ch)
    return ''.join(out)

def load(lang):
    p = os.path.join(here, f'catalog.{lang}.json')
    if not os.path.isfile(p):
        return None
    return json.load(open(p, encoding='utf-8'))

def emit_table(cat, name, prefix, count_macro):
    lines = [f'const u8 *const {name}[{count_macro}] = {{']
    n = 0
    for k, v in cat.items():
        if not (k.startswith(prefix) and k.endswith('.description')):
            continue
        if not v or v.strip() in ('-', '---'):
            continue
        sym = k[:-len('.description')]  # es. MOVE_POUND / ABILITY_STENCH
        lines.append(f'    [{sym}] = COMPOUND_STRING("{cstr(v)}"),')
        n += 1
    lines.append('};')
    return '\n'.join(lines), n

out = []
out.append('// AUTO-GENERATO da tools/i18n/gen_desc.py. NON modificare a mano.')
out.append('// Tabelle DESCRIZIONI mosse+abilita tradotte, selezionate da NV_LANG.')
out.append('#include "global.h"')
out.append('#include "constants/moves.h"')
out.append('#include "constants/abilities.h"')
out.append('')
out.append('#if NV_LANG != LANGUAGE_ENGLISH')

first = True
total = {}
for lang, macro in LANGS.items():
    cat = load(lang)
    if cat is None:
        continue
    guard = '#if' if first else '#elif'
    out.append(f'{guard} NV_LANG == {macro}')
    t1, n1 = emit_table(cat, 'gNvMoveDescriptions', 'MOVE_', 'MOVES_COUNT')
    t2, n2 = emit_table(cat, 'gNvAbilityDescriptions', 'ABILITY_', 'ABILITIES_COUNT')
    out.append(t1)
    out.append('')
    out.append(t2)
    total[lang] = (n1, n2)
    first = False

# fallback per lingue NV_LANG != EN senza catalogo: array vuoti (tutto NULL)
if not first:
    out.append('#else')
out.append('const u8 *const gNvMoveDescriptions[MOVES_COUNT] = {0};')
out.append('const u8 *const gNvAbilityDescriptions[ABILITIES_COUNT] = {0};')
if not first:
    out.append('#endif')
out.append('#endif // NV_LANG != LANGUAGE_ENGLISH')
out.append('')

open(OUT, 'w', encoding='utf-8', newline='\n').write('\n'.join(out))
print('Scritto', OUT)
for lang, (n1, n2) in total.items():
    print(f'  {lang}: mosse {n1}, abilita {n2}')
