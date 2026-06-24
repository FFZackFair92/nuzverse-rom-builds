// tools/i18n/gen_rom_names.mjs
// Genera src/nv_i18n_moves.c (tabelle NOMI tradotti di MOSSE e ABILITA' per NV_LANG) RIUSANDO
// il dataset del tracker (pokemon-ironmon/resources/js/utils/trackerI18nData.js).
//   - mosse: indicizzate per ID (= id PokeAPI/nazionale).
//   - abilita': il dataset e' per SLUG; mappo ABILITY_<X> -> nome EN (da src/data/abilities.h)
//     -> slug -> traduzione. Cosi' l'indice resta la costante ABILITY_<X> del ROM.
// Lanciare in LOCALE (serve il repo webapp accanto al decomp); l'output .c viene committato e
// la CI lo compila (preproc/charmap via COMPOUND_STRING). USO: node tools/i18n/gen_rom_names.mjs
import { writeFile, readFile } from 'node:fs/promises'
import { dirname, join } from 'node:path'
import { fileURLToPath, pathToFileURL } from 'node:url'

const here = dirname(fileURLToPath(import.meta.url))
const OUT = join(here, '..', '..', 'src', 'nv_i18n_moves.c')
const ABILITIES_H = join(here, '..', '..', 'src', 'data', 'abilities.h')
const DATA = join(here, '..', '..', '..', '..', 'pokemon-ironmon', 'resources', 'js', 'utils', 'trackerI18nData.js')

const LANGS = [
  ['it', 'LANGUAGE_ITALIAN'], ['es', 'LANGUAGE_SPANISH'], ['fr', 'LANGUAGE_FRENCH'],
  ['de', 'LANGUAGE_GERMAN'], ['pt', 'NV_LANG_PORTUGUESE'],
]
const MOVE_MAX = 847

function sanitize(s) {
  return String(s)
    .replace(/[‘’ʼ]/g, "'").replace(/[“”]/g, '"').replace(/[–—]/g, '-')
    .replace(/…/g, '...').replace(/[   ]/g, ' ')
    .replace(/\\/g, '\\\\').replace(/"/g, '\\"')
}
const slug = (s) => String(s).toLowerCase().trim().replace(/[^a-z0-9]+/g, '-').replace(/^-+|-+$/g, '')

// abilities.h: mappa costante ABILITY_<X> -> nome inglese (per lo slug)
async function parseAbilities() {
  const txt = await readFile(ABILITIES_H, 'utf8')
  const map = [] // {constName, en}
  let cur = null
  for (const line of txt.split('\n')) {
    const im = line.match(/\[(ABILITY_[A-Z0-9_]+)\]/)
    if (im) cur = im[1]
    const nm = line.match(/\.name\s*=\s*(?:COMPOUND_STRING|_)\("((?:[^"\\]|\\.)*)"\)/)
    if (nm && cur) { map.push({ c: cur, en: nm[1] }); cur = null }
  }
  return map
}

async function main() {
  const mod = await import(pathToFileURL(DATA).href)
  const MOVE = mod.MOVE_I18N || {}
  const ABIL = mod.ABILITY_I18N || {}
  const abilities = await parseAbilities()

  let out = '// AUTO-GENERATO da tools/i18n/gen_rom_names.mjs (riusa il dataset del tracker).\n'
  out += '// NON modificare a mano. Tabelle nomi MOSSE+ABILITA tradotti, selezionate da NV_LANG.\n'
  out += '#include "global.h"\n#include "constants/moves.h"\n#include "constants/abilities.h"\n\n'

  LANGS.forEach(([loc, macro], idx) => {
    out += (idx === 0 ? '#if' : '#elif') + ` NV_LANG == ${macro}\n`
    // mosse
    out += 'const u8 *const gNvMoveNames[MOVES_COUNT] = {\n'
    let mc = 0
    for (let id = 1; id <= MOVE_MAX; id++) {
      const e = MOVE[id] && MOVE[id][loc]
      if (!e || !e.n) continue
      out += `    [${id}] = COMPOUND_STRING("${sanitize(e.n)}"),\n`; mc++
    }
    out += '};\n'
    // abilita'
    out += 'const u8 *const gNvAbilityNames[ABILITIES_COUNT] = {\n'
    let ac = 0
    for (const { c, en } of abilities) {
      const e = ABIL[slug(en)] && ABIL[slug(en)][loc]
      if (!e || !e.n) continue
      out += `    [${c}] = COMPOUND_STRING("${sanitize(e.n)}"),\n`; ac++
    }
    out += '};\n'
    console.log(`  ${loc}: mosse ${mc}, abilita ${ac}`)
  })
  out += '#endif\n'

  await writeFile(OUT, out)
  console.log('OK ->', OUT)
}

main().catch((e) => { console.error(e); process.exit(1) })
