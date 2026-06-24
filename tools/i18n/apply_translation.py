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
# NB: il charmap Gen3 NON ha la virgoletta dritta ASCII " (romperebbe la .string:
# 'junk at end of line'); ha invece le curve LEFTDQ=B1 e RIGHTDQ=B2. Quindi le dritte
# vanno convertite in curve (alternando apri/chiudi). Gli apostrofi ' restano (nel charmap).
_LEFTDQ = '\u201c'
_RIGHTDQ = '\u201d'
_SUB = {'\u2018': "'", '\u2019': "'", '\u2013': '-', '\u2014': '-',
        '\u2026': '...', '\u00a0': ' ', '\u2007': ' ', '\u202f': ' ',
        '\u00ab': _LEFTDQ, '\u00bb': _RIGHTDQ, '\u00b0': ''}
def sanitize(s):
    # le curve eventuali restano (mappate dal charmap); converto le dritte in curve
    for k, v in _SUB.items():
        s = s.replace(k, v)
    out = []
    open_q = True
    for ch in s:
        if ch == '"':
            out.append(_LEFTDQ if open_q else _RIGHTDQ)
            open_q = not open_q
        else:
            out.append(ch)
    return ''.join(out)

# Spezza il testo in segmenti ai codici di controllo \n \l \p (mantenendo il codice in coda
# al segmento). Il preproc concatena le .string adiacenti ma limita ogni .string a 1024 byte:
# emettere un segmento per riga tiene ogni .string ben sotto il limite. Se un segmento e'
# comunque troppo lungo (raro), lo si taglia a forza su uno spazio vicino a ~400 byte.
def split_segments(s):
    parts = re.split(r'(\\[nlp])', s)  # mantiene i delimitatori \n \l \p
    segs = []
    buf = ''
    for tok in parts:
        buf += tok
        if tok in ('\\n', '\\l', '\\p'):
            segs.append(buf)
            buf = ''
    if buf:
        segs.append(buf)
    # taglio di sicurezza per segmenti enormi senza codici di controllo
    out = []
    for seg in segs:
        while len(seg.encode('utf-8')) > 400:
            cut = seg.rfind(' ', 0, 380)
            if cut <= 0:
                cut = 380
            out.append(seg[:cut + 1])
            seg = seg[cut + 1:]
        out.append(seg)
    return [x for x in out if x != '']

DIRS = [os.path.join(ROOT, 'data', 'text'), os.path.join(ROOT, 'data', 'maps')]
RE_LABEL = re.compile(r'^([A-Za-z_][A-Za-z0-9_]*)::?')
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
                    # salta eventuali righe vuote/commento tra label e .string (preservandole)
                    k = i
                    while k < len(lines) and (lines[k].strip() == '' or lines[k].strip().startswith('@')):
                        k += 1
                    # consuma il blocco .string originale (righe consecutive)
                    j = k
                    while j < len(lines) and RE_STRING.match(lines[j]):
                        j += 1
                    if j > k:  # c'era almeno una .string
                        for t in range(i, k):  # riemetti le righe vuote/commento saltate
                            out.append(lines[t])
                        # Spezza in piu' .string ai codici di controllo \n \l \p: il preproc
                        # concatena le .string adiacenti, ma ha un limite di 1024 byte per .string;
                        # una traduzione lunga in un'unica .string lo supererebbe.
                        for seg in split_segments(sanitize(cat[m.group(1)])):
                            out.append('\t.string "' + seg + '"')
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
