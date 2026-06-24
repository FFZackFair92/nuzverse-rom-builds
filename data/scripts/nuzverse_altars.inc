@ ===== Nuzverse: altari NPC "modificatori" (one-shot per run, IronMon) =====
@ Tre NPC condivisi tra Hoenn e Kanto. Nascosti in Nuzlocke via flagId FLAG_NV_ALTARS_HIDDEN.
@ One-shot: ogni altare consuma il suo FLAG_NV_ALTAR_* (azzerato a new-game).
@ ESITO RANDOM ma SEEDATO (solo dal seed della run): il giocatore sceglie SOLO il Pokemon;
@ l'abilita'/natura/mossa risultante e' decisa dagli special seedati (field_specials.c) e
@ ricalcolabile dalla webapp (Seed Log + post-run). gStringVar2 = esito (nome) bufferato dallo special.

@ --- Altare delle Mosse: mossa seedata forzata in uno slot seedato ---
NvAltar_EventScript_Moves::
	lockall
	faceplayer
	goto_if_set FLAG_NV_ALTAR_MOVES, NvAltar_EventScript_Spent
	msgbox NvAltar_Text_MovesIntro, MSGBOX_DEFAULT
	special ChoosePartyMon
	goto_if_ge VAR_0x8004, PARTY_SIZE, NvAltar_EventScript_Cancel
	specialvar VAR_RESULT, ScriptGetPartyMonSpecies
	goto_if_eq VAR_RESULT, SPECIES_EGG, NvAltar_EventScript_Cancel
	special NvAltarSeededMove
	setflag FLAG_NV_ALTAR_MOVES
	msgbox NvAltar_Text_MovesDone, MSGBOX_DEFAULT
	releaseall
	end

@ --- Altare dell'Abilita': slot abilita' seedato ---
NvAltar_EventScript_Ability::
	lockall
	faceplayer
	goto_if_set FLAG_NV_ALTAR_ABILITY, NvAltar_EventScript_Spent
	msgbox NvAltar_Text_AbilityIntro, MSGBOX_DEFAULT
	special ChoosePartyMon
	goto_if_ge VAR_0x8004, PARTY_SIZE, NvAltar_EventScript_Cancel
	specialvar VAR_RESULT, ScriptGetPartyMonSpecies
	goto_if_eq VAR_RESULT, SPECIES_EGG, NvAltar_EventScript_Cancel
	special NvAltarSeededAbility
	setflag FLAG_NV_ALTAR_ABILITY
	msgbox NvAltar_Text_AbilityDone, MSGBOX_DEFAULT
	releaseall
	end

@ --- Altare della Natura: natura seedata (HIDDEN_NATURE, non tocca il PID) ---
NvAltar_EventScript_Nature::
	lockall
	faceplayer
	goto_if_set FLAG_NV_ALTAR_NATURE, NvAltar_EventScript_Spent
	msgbox NvAltar_Text_NatureIntro, MSGBOX_DEFAULT
	special ChoosePartyMon
	goto_if_ge VAR_0x8004, PARTY_SIZE, NvAltar_EventScript_Cancel
	specialvar VAR_RESULT, ScriptGetPartyMonSpecies
	goto_if_eq VAR_RESULT, SPECIES_EGG, NvAltar_EventScript_Cancel
	special NvAltarSeededNature
	setflag FLAG_NV_ALTAR_NATURE
	msgbox NvAltar_Text_NatureDone, MSGBOX_DEFAULT
	releaseall
	end

@ --- Sub-script condivisi ---
NvAltar_EventScript_Spent::
	msgbox NvAltar_Text_Spent, MSGBOX_DEFAULT
	releaseall
	end

NvAltar_EventScript_Cancel::
	releaseall
	end

@ ===== Testi =====
NvAltar_Text_MovesIntro:
	.string "This altar bends fate. Choose a POKéMON\n"
	.string "to receive the technique it has chosen.$"

NvAltar_Text_MovesDone:
	.string "Fate grants the move… {STR_VAR_2}!$"

NvAltar_Text_AbilityIntro:
	.string "This altar will awaken a talent fate has\n"
	.string "already chosen. Whose?$"

NvAltar_Text_AbilityDone:
	.string "Its ability is now {STR_VAR_2}!$"

NvAltar_Text_NatureIntro:
	.string "This altar reshapes a nature fate has\n"
	.string "already chosen. For which POKéMON?$"

NvAltar_Text_NatureDone:
	.string "Its nature is now {STR_VAR_2}!$"

NvAltar_Text_Spent:
	.string "The altar's light has faded.\n"
	.string "It will not answer again this run.$"
