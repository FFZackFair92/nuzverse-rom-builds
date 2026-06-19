#ifndef GUARD_NUZVERSE_LEVELS_H
#define GUARD_NUZVERSE_LEVELS_H

// Curva livelli Kaizo (vedi docs/rom/PIANO_LIVELLI_KAIZO.md).
// boss (palestre/E4/campione/rivale chiave) = ace esatto per ID; altri mon scalano sotto l'ace.
// trainer generici = livello per AREA (gMapHeader.regionMapSectionId), scalato sul party.
u8 NvKaizoLevel(u8 defaultLevel, u16 trainerId, u8 monIndex, u8 partySize);

#endif // GUARD_NUZVERSE_LEVELS_H
