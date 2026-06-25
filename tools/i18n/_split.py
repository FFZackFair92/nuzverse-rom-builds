import json, os, sys, collections
here = os.path.dirname(os.path.abspath(__file__))
en = json.load(open(os.path.join(here, 'catalog.en.json'), encoding='utf-8'))
LANG = sys.argv[1] if len(sys.argv) > 1 else 'es'
N = int(sys.argv[2]) if len(sys.argv) > 2 else 16
lp = os.path.join(here, f'catalog.{LANG}.json')
done = json.load(open(lp, encoding='utf-8')) if os.path.isfile(lp) else {}
parts = os.path.join(here, 'parts'); os.makedirs(parts, exist_ok=True)

SKIP = ('text/tv.', 'match_call', 'apprentice', 'battle_tent', 'berries', 'blend_master',
        'shoal_cave', 'secret_base', 'mauville_man', 'lottery', 'event_ticket', 'contest',
        'fame_checker', 'pokemon_news', 'pokenews', 'check_furniture', 'questionnaire',
        'record_mix', 'pokemon_jump', 'mystery', 'frontier', 'BattleFrontier', 'TrainerHill',
        'trainer_hill', 'roulette', 'slot_machine', 'Contest', 'battle_pike', 'battle_dome',
        'battle_pyramid', 'battle_factory', 'battle_arena', 'battle_tower')
def isskip(src): return any(s in src for s in SKIP)

rem = []
for k, v in en.items():
    if k in done: continue
    if v.get('type') != 'dialogue': continue
    if k.startswith('data/') and ':' in k: continue
    if isskip(v.get('src', '')): continue
    rem.append(k)
rem.sort()
print('Reachable dialogue', LANG, ':', len(rem))
chunks = [[] for _ in range(N)]
for i, k in enumerate(rem):
    chunks[i % N].append(k)
for i, ch in enumerate(chunks, 1):
    d = {k: en[k]['en'] for k in ch}
    json.dump(d, open(os.path.join(parts, f'src_{i:02}.json'), 'w', encoding='utf-8'), ensure_ascii=False, indent=1)
    print(f'  src_{i:02}.json  {len(d)}')
