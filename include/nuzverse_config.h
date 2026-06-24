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

// Game over a prova di bomba: il black-out (sconfitta) termina la run DENTRO la ROM
// (soft-reset -> instant-start = run nuova). Vale per IronMon e Nuzlocke.
#ifndef NV_GAMEOVER_ON_LOSS
#define NV_GAMEOVER_ON_LOSS 1
#endif

// Regole Kaizo IronMon in-ROM. SOLO IronMon (NON Nuzlocke): si auto-disattiva quando
// NV_PERMADEATH=1, quindi le variant nuzlocke non le ricevono (per ora). Entrambe le regioni.
#ifndef NV_KAIZO
  #if NV_PERMADEATH
    #define NV_KAIZO 0
  #else
    #define NV_KAIZO 1
  #endif
#endif

// ⚠️ DEBUG/TEMP — tutti i Pokémon degli allenatori a Lv1 (battaglie lampo per collaudo).
// Rimettere a 0 (o build con -DNV_DEBUG_ENEMY_LV1=0) prima del bilanciamento/lancio.
#ifndef NV_DEBUG_ENEMY_LV1
#define NV_DEBUG_ENEMY_LV1 1
#endif
#if NV_DEBUG_ENEMY_LV1
#define NV_TRAINER_LEVEL(lvl) 1
#else
#define NV_TRAINER_LEVEL(lvl) (lvl)
#endif

// Centri Pokemon chiusi: la porta non entra (l'infermiera e' gia' fuori dal Centro).
// Il warp d'ingresso viene neutralizzato -> il giocatore resta davanti alla porta.
// Entrambe le regioni (tutti i layout di Centro 1F).
#ifndef NV_NO_POKECENTERS
#define NV_NO_POKECENTERS 1
#endif

// Market (Poke Mart) chiusi: come i Centri, la porta non entra. Neutralizza il warp
// d'ingresso -> il giocatore resta davanti. Entrambe le regioni (LAYOUT_MART Hoenn +
// LAYOUT_MART_FRLG Kanto).
#ifndef NV_NO_POKEMARTS
#define NV_NO_POKEMARTS 1
#endif

// Quest Regi rimossa: niente Regirock/Regice/Registeel ne' Sealed Chamber.
// Sigilla l'ingresso (warp neutralizzato) di tutte le camere Regi + Sealed Chamber.
#ifndef NV_NO_REGI
#define NV_NO_REGI 1
#endif

// Safari Zone sigillata: contenuto opzionale (mini-gioco di cattura) incompatibile con
// le regole IronMon. Neutralizza il warp d'ingresso al gate di Route 121 -> il giocatore
// resta fuori (non entra nemmeno nel gate, quindi non paga l'attendente). Solo Hoenn.
#ifndef NV_NO_SAFARI
#define NV_NO_SAFARI 1
#endif

// Edifici opzionali sigillati (porta inerte, come gli altri ingressi): a Lilycove i
// Grandi Magazzini, la Contest Lobby e il Museo. Sono contenuto secondario non necessario
// al percorso. Lista mappe in NvIsSealedOptionalBuilding (estendibile).
#ifndef NV_NO_OPTIONAL_BUILDINGS
#define NV_NO_OPTIONAL_BUILDINGS 1
#endif

// Regola MT Kaizo (stile StartAB): quando insegni una MT consumabile c'e' solo il 50%
// di probabilita' che il Pokemon la impari; sul fallimento la MT viene comunque bruciata.
// MN e oggetti 'importanti' (GetItemImportance != 0) NON sono toccati.
// Default = NV_KAIZO (ON IronMon su entrambe le regioni, OFF Nuzlocke). Override da CLI.
#ifndef NV_TM_5050
#define NV_TM_5050 NV_KAIZO
#endif

// Interruttore repellente (key item): ON = nessun incontro selvatico + nessuno spawn OWE,
// SENZA conteggio passi. Stato in un flag salvato (entrambe le regioni). Toggle dallo zaino.
#define FLAG_NV_REPEL_TOGGLE FLAG_UNUSED_0x4A7

// Dungeon a senso unico ("sigilla dopo completato"): quando esci da un dungeon verso
// l'esterno setti il flag persistente; con il flag attivo il warp d'ingresso e' neutralizzato.
#define NV_ONEWAY_DUNGEONS 1
// NB: usare flag presenti SIA in flags.h (Emerald) SIA in flags_frlg.h (FireRed) — il
// codice e' condiviso tra le due build. 0x4A8-0x4AB sono unused in entrambi (accanto al repel 0x4A7).
#define FLAG_NV_DUNGEON_MTMOON       FLAG_UNUSED_0x4A8
#define FLAG_NV_DUNGEON_ROCKTUNNEL   FLAG_UNUSED_0x4A9
#define FLAG_NV_DUNGEON_VICTORYRD_K  FLAG_UNUSED_0x4AA
#define FLAG_NV_DUNGEON_VICTORYRD_H  FLAG_UNUSED_0x4AB
#define FLAG_NV_DUNGEON_WARDWOODS    FLAG_UNUSED_0x4AC
#define FLAG_NV_DUNGEON_GRANITE      FLAG_UNUSED_0x4AD
// Dungeon Hoenn aggiunti (23 giu): 0x4AE-0x4B4 unused in flags.h E flags_frlg.h.
#define FLAG_NV_DUNGEON_MTPYRE       FLAG_UNUSED_0x4AE
#define FLAG_NV_DUNGEON_CAVEORIGIN   FLAG_UNUSED_0x4AF
#define FLAG_NV_DUNGEON_SEAFLOOR     FLAG_UNUSED_0x4B0
#define FLAG_NV_DUNGEON_MAGMA        FLAG_UNUSED_0x4B1
#define FLAG_NV_DUNGEON_AQUA         FLAG_UNUSED_0x4B2
#define FLAG_NV_DUNGEON_NEWMAUVILLE  FLAG_UNUSED_0x4B3
#define FLAG_NV_DUNGEON_SKYPILLAR    FLAG_UNUSED_0x4B4

// Ordine palestre G1->G8 (Hoenn): blocca l'ingresso di una palestra se manca la
// medaglia precedente (usa i FLAG_BADGE0x_GET, comuni a entrambe le build).
#define NV_GYM_ORDER 1

// Camera "lock" sul player: ri-aggancia la camera allo sprite ATTUALE del giocatore
// se il collegamento si stacca. Sintomo (Caso 2): cammini ma il mondo resta fermo e
// il personaggio scorre decentrato — la camera inseguiva uno sprite stale (capita con
// instant-start / ricreazione sprite / feature OWE-follower). Agisce SOLO quando hai
// il controllo (mai durante script/cutscene, che spostano la camera di proposito) e
// SOLO se il link e' davvero diverso dallo sprite del player (re-aggancio ogni frame
// azzererebbe il movimento). Entrambe le regioni.
#ifndef NV_CAMERA_LOCK
#define NV_CAMERA_LOCK 1
#endif

// Fune di Fuga disabilitata: usarla NON ha effetto ("non si puo' usare qui"). Mina
// i dungeon a senso unico (NV_ONEWAY_DUNGEONS) e la difficolta'. Solo l'ITEM Escape
// Rope (la mossa Fossa/Dig non e' toccata). Entrambe le regioni.
#ifndef NV_NO_ESCAPE_ROPE
#define NV_NO_ESCAPE_ROPE 1
#endif

// Mosse campo NON essenziali disabilitate (usarle dal menu Pokemon = "non si puo'
// usare qui"): Volo, Fossa, Teletrasporto, Profumino, Forza Segreta, Covauovo,
// Lattepronto. Sono scorciatoie/fughe/cure che minano dungeon a senso unico e
// difficolta'. Le mosse di TRAVERSATA restano usabili (Surf, Taglio, Forza,
// Cascata, Spaccaroccia, Sub, Rampicante, Flash). Solo uso sul CAMPO: in battaglia
// Volo/Fossa restano mosse d'attacco normali. Entrambe le regioni.
#ifndef NV_NO_UTILITY_FIELD_MOVES
#define NV_NO_UTILITY_FIELD_MOVES 1
#endif

#endif // GUARD_NUZVERSE_CONFIG_H
