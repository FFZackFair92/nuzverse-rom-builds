// tools/i18n/gen_rom_names.mjs
// Genera src/nv_i18n_moves.c (tabella nomi mossa tradotti per NV_LANG) RIUSANDO il dataset
// gia' creato per il tracker (pokemon-ironmon/resources/js/utils/trackerI18nData.js).
// Va lanciato in LOCALE (serve il repo webapp accanto al decomp); l'output .c viene committato
// e la CI lo compila. La traduzione passa dal preproc/charmap del build (COMPOUND_STRING).
//
// USO: node tools/i18n/gen_rom_names.mjs
import { writeFile } from 'node:fs/promises'
import { dirname, join } from 'node:path'
import { fileURLToPath, pathToFileURL } from 'node:url'

const here = dirname(fileURLToPath(import.meta.url))
const OUT = join(here, '..', '..', 'src', 'nv_i18n_moves.c')
// decomp/nuzverse-expansion/tools/i18n -> 4 livelli su = root del progetto -> pokemon-ironmon
const DATA = join(here, '..', '..', '..', '..', 'pokemon-ironmon', 'resources', 'js', 'utils', 'trackerI18nData.js')

// locale dataset -> macro lingua del ROM
const LANGS = [
  ['it', 'LANGUAGE_ITALIAN'],
  ['es', 'LANGUAGE_SPANISH'],
  ['fr', 'LANGUAGE_FRENCH'],
  ['de', 'LANGUAGE_GERMAN'],
  ['pt', 'NV_LANG_PORTUGUESE'],
]
const MOVE_MAX = 847

// Normalizza la punteggiatura non-ASCII assente nel charmap Gen3 (gli accenti latini restano),
// poi escape per la stringa C.
function sanitize(s) {
  return String(s)
    .replace(/[‘’ʼ]/g, "'")  // virgolette/apostrofi curvi -> '
    .replace(/[“”]/g, '"')         // virgolette doppie curve -> "
    .replace(/[–—]/g, '-')         // trattini lunghi -> -
    .replace(/…/g, '...')               // ellissi
    .replace(/[   ]/g, ' ')   // spazi speciali
    .replace(/\\/g, '\\\\')
    .replace(/"/g, '\\"')
}

async function main() {
  const mod = await import(pathToFileURL(DATA).href)
  const MOVE = mod.MOVE_I18N || {}

  let out = '// AUTO-GENERATO da tools/i18n/gen_rom_names.mjs (riusa il dataset del tracker).\n'
  out += '// NON modificare a mano. Tabella nomi mossa tradotti, selezionata da NV_LANG.\n'
  out += '#include "global.h"\n#include "constants/moves.h"\n\n'

  LANGS.forEach(([loc, macro], idx) => {
    out += (idx === 0 ? '#if' : '#elif') + ` NV_LANG == ${macro}\n`
    out += 'const u8 *const gNvMoveNames[] = {\n'
    let count = 0
    for (let id = 1; id <= MOVE_MAX; id++) {
      const e = MOVE[id] && MOVE[id][loc]
      if (!e || !e.n) continue
      out += `    [${id}] = COMPOUND_STRING("${sanitize(e.n)}"),\n`
      count++
    }
    out += '};\n'
    console.log(`  ${loc}: ${count} nomi`)
  })
  out += '#endif\n'

  await writeFile(OUT, out)
  console.log('OK ->', OUT)
}

main().catch((e) => { console.error(e); process.exit(1) })
