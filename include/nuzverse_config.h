#ifndef GUARD_NUZVERSE_CONFIG_H
#define GUARD_NUZVERSE_CONFIG_H

// ─────────────────────────────────────────────────────────────────────────────
// Nuzverse — flag feature compile-time per il branch unico `nuzverse/main`.
// Le 4 variant = stesso albero, scelte da: target make (`make` Hoenn / `make
// firered` Kanto) + questi flag passati dalla CI via `NV_DEFINES`.
//   es.  make firered NV_DEFINES="-DNV_PERMADEATH=1"   -> Nuzlocke Kanto
// I default qui sotto valgono per IronMon Emerald (tutto ON tranne permadeath).
// Ogni #ifndef permette l'override da riga di comando (-DNV_xxx=...).
// ─────────────────────────────────────────────────────────────────────────────

// MN-less: Taglio/Spaccaroccia/Forza/Surf/Cascata/Sub/RockClimb usabili su
// qualsiasi ostacolo SENZA possedere la MN né la medaglia (mon di testa esegue
// l'effetto). Sostituisce traghetti + rimozione ostacoli.
#ifndef NV_HM_FREE
#define NV_HM_FREE 1
#endif

// Permadeath in-ROM (Nuzlocke): lo svenimento marca il Pokémon. 1 = Nuzlocke.
#ifndef NV_PERMADEATH
#define NV_PERMADEATH 0
#endif

// Storia "mai bloccante": preset di flag/var a new-game che rimuovono roadblock,
// tutorial forzati e gate sì/no del percorso principale (Hoenn + Kanto).
#ifndef NV_NO_BLOCKERS
#define NV_NO_BLOCKERS 1
#endif

// Instant-start: salta intro e schermata nome, spawn nel laboratorio pronti a
// scegliere lo starter (Birch su Emerald, Oak su Kanto).
#ifndef NV_INSTANT_START
#define NV_INSTANT_START 1
#endif

#endif // GUARD_NUZVERSE_CONFIG_H
