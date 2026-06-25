// tools/i18n/translate.mjs <lang>
// Traduce TUTTO il catalogo dialoghi (catalog.en.json) in <lang> -> catalog.<lang>.json usando
// un LLM (API OpenAI-compatibile). Gestisce: placeholder ({PLAYER},{RIVAL},{STR_VAR_x}...),
// codici di controllo (\n \l \p) e il terminatore $, e ri-spezza alle finestre GBA (<=26 char/riga).
// Batch + resume (scrive in modo incrementale: puoi interrompere e rilanciare).
//
// ENV richieste:
//   NV_TRANSLATE_API_KEY   = la tua API key
//   NV_TRANSLATE_API_BASE  = opz. (default https://api.openai.com/v1)
//   NV_TRANSLATE_MODEL     = opz. (default gpt-4o-mini)
// USO:  NV_TRANSLATE_API_KEY=sk-... node tools/i18n/translate.mjs it
import { readFile, writeFile } from 'node:fs/promises'
import { existsSync } from 'node:fs'
import { dirname, join } from 'node:path'
import { fileURLToPath } from 'node:url'

const here = dirname(fileURLToPath(import.meta.url))
const LANG = process.argv[2]
if (!LANG) { console.error('uso: node translate.mjs <lang>'); process.exit(1) }
const EN = join(here, 'catalog.en.json')
const OUT = join(here, `catalog.${LANG}.json`)

const API_BASE = process.env.NV_TRANSLATE_API_BASE || 'https://api.openai.com/v1'
const API_KEY = process.env.NV_TRANSLATE_API_KEY
const MODEL = process.env.NV_TRANSLATE_MODEL || 'gpt-4o-mini'
if (!API_KEY) { console.error('manca NV_TRANSLATE_API_KEY'); process.exit(1) }

const LANG_NAME = { it: 'Italian', es: 'Spanish', fr: 'French', de: 'German', pt: 'Portuguese', ja: 'Japanese' }[LANG] || LANG
const BATCH = Number(process.env.NV_TRANSLATE_BATCH || 12) // modelli locali: meglio 8-12
const sleep = (ms) => new Promise(r => setTimeout(r, ms))

const SYS = `You are a professional localizer of Pokémon Game Boy Advance ROMs into ${LANG_NAME}.
Rules — follow EXACTLY:
- Keep every placeholder token UNCHANGED and in place: {PLAYER}, {RIVAL}, {STR_VAR_1}, {STR_VAR_2}, {STR_VAR_3}, {KUN}, {PKMN}, {POKE} and any {WORD} token.
- Keep the control codes UNCHANGED as literal two-character sequences: \\n (line break), \\l (scroll line), \\p (new text page). Keep the final $ terminator.
- Re-wrap the translation so each visible line is AT MOST 26 characters: replace/insert \\n and \\l appropriately, keep \\p where the original starts a new page. Roughly match the original number of pages.
- Write "POKéMON" with the accented é exactly like the source. Keep proper nouns, town & character names as in official ${LANG_NAME} Pokémon games when known.
- Output ONLY a raw JSON object mapping each id to its translated string. No commentary, NO markdown code fences (no \`\`\`).`

// Parser JSON tollerante: i server locali (LM Studio/Ollama/GLM) spesso avvolgono il JSON
// in code-fence o aggiungono testo. Togliamo i fence e isoliamo l'oggetto { ... }.
function parseJsonLoose(s) {
  let t = String(s || '').trim()
  t = t.replace(/^```(?:json)?\s*/i, '').replace(/\s*```$/i, '').trim()
  try { return JSON.parse(t) } catch {}
  const i = t.indexOf('{'), j = t.lastIndexOf('}')
  if (i >= 0 && j > i) {
    try { return JSON.parse(t.slice(i, j + 1)) } catch {}
  }
  return null
}

async function callLLM(items) {
  const userObj = {}
  for (const [id, en] of items) userObj[id] = en
  const body = {
    model: MODEL,
    messages: [
      { role: 'system', content: SYS },
      { role: 'user', content: 'Translate each value. Return a raw JSON object {id: translation}. No code fences.\n' + JSON.stringify(userObj) },
    ],
    temperature: 0.2,
    // LM Studio accetta solo 'text' o 'json_schema' (NON 'json_object'); usiamo 'text'
    // + parser tollerante. Compatibile anche con OpenAI/Ollama.
    response_format: { type: 'text' },
  }
  for (let attempt = 1; attempt <= 4; attempt++) {
    try {
      const res = await fetch(`${API_BASE}/chat/completions`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json', Authorization: `Bearer ${API_KEY}` },
        body: JSON.stringify(body),
      })
      if (!res.ok) throw new Error('HTTP ' + res.status + ' ' + (await res.text()).slice(0, 200))
      const data = await res.json()
      const parsed = parseJsonLoose(data.choices?.[0]?.message?.content)
      if (!parsed || typeof parsed !== 'object') throw new Error('risposta non-JSON dal modello')
      return parsed
    } catch (e) {
      if (attempt === 4) { console.warn('  ! batch fallito:', e.message); return {} }
      await sleep(1500 * attempt)
    }
  }
  return {}
}

async function main() {
  const cat = JSON.parse(await readFile(EN, 'utf8'))
  const done = existsSync(OUT) ? JSON.parse(await readFile(OUT, 'utf8')) : {}
  const todo = Object.entries(cat).filter(([id]) => !(id in done)).map(([id, v]) => [id, v.en])
  console.log(`${LANG}: ${Object.keys(cat).length} voci, gia' tradotte ${Object.keys(done).length}, da fare ${todo.length}`)

  for (let i = 0; i < todo.length; i += BATCH) {
    const slice = todo.slice(i, i + BATCH)
    const out = await callLLM(slice)
    for (const [id] of slice) if (out[id]) done[id] = out[id]
    await writeFile(OUT, JSON.stringify(done, null, 0))
    if ((i / BATCH) % 5 === 0) console.log(`  ${Math.min(i + BATCH, todo.length)}/${todo.length}`)
    await sleep(200)
  }
  console.log(`OK -> ${OUT}  (${Object.keys(done).length} stringhe)`)
}
main().catch(e => { console.error(e); process.exit(1) })
