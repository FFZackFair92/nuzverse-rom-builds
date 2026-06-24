@ ===== Nuzverse: altari NPC "modificatori" (one-shot per run, IronMon) =====
@ Tre NPC condivisi tra Hoenn e Kanto (gli object event li richiamano con gfx diverse).
@ Nascosti in Nuzlocke via flagId d'oggetto FLAG_NV_ALTARS_HIDDEN (settato in new_game se !NV_KAIZO).
@ One-shot: ogni altare consuma il suo FLAG_NV_ALTAR_* (azzerato a new-game).

@ --- Altare delle Mosse: relearner (livello/uovo/MT/tutor) ---
NvAltar_EventScript_Moves::
	lockall
	faceplayer
	goto_if_set FLAG_NV_ALTAR_MOVES, NvAltar_EventScript_Spent
	msgbox NvAltar_Text_MovesIntro, MSGBOX_DEFAULT
	setflag FLAG_NV_ALTAR_MOVES
	goto Common_EventScript_MoveRelearner
	end

@ --- Altare dell'Abilita': cicla all'abilita' valida successiva del mon scelto ---
NvAltar_EventScript_Ability::
	lockall
	faceplayer
	goto_if_set FLAG_NV_ALTAR_ABILITY, NvAltar_EventScript_Spent
	msgbox NvAltar_Text_AbilityIntro, MSGBOX_DEFAULT
	special ChoosePartyMon
	goto_if_ge VAR_0x8004, PARTY_SIZE, NvAltar_EventScript_Cancel
	specialvar VAR_RESULT, ScriptGetPartyMonSpecies
	goto_if_eq VAR_RESULT, SPECIES_EGG, NvAltar_EventScript_Cancel
	special NvAltarCycleAbility
	goto_if_eq VAR_RESULT, FALSE, NvAltar_EventScript_AbilityNoChange
	setflag FLAG_NV_ALTAR_ABILITY
	msgbox NvAltar_Text_AbilityDone, MSGBOX_DEFAULT
	releaseall
	end

NvAltar_EventScript_AbilityNoChange::
	msgbox NvAltar_Text_AbilityNoChange, MSGBOX_DEFAULT
	releaseall
	end

@ --- Altare della Natura: scegli una delle 25 nature (HIDDEN_NATURE, non tocca il PID) ---
NvAltar_EventScript_Nature::
	lockall
	faceplayer
	goto_if_set FLAG_NV_ALTAR_NATURE, NvAltar_EventScript_Spent
	msgbox NvAltar_Text_NatureIntro, MSGBOX_DEFAULT
	special ChoosePartyMon
	goto_if_ge VAR_0x8004, PARTY_SIZE, NvAltar_EventScript_Cancel
	specialvar VAR_RESULT, ScriptGetPartyMonSpecies
	goto_if_eq VAR_RESULT, SPECIES_EGG, NvAltar_EventScript_Cancel
	dynmultipush NvAltar_Text_NatureHardy, 0
	dynmultipush NvAltar_Text_NatureLonely, 1
	dynmultipush NvAltar_Text_NatureBrave, 2
	dynmultipush NvAltar_Text_NatureAdamant, 3
	dynmultipush NvAltar_Text_NatureNaughty, 4
	dynmultipush NvAltar_Text_NatureBold, 5
	dynmultipush NvAltar_Text_NatureDocile, 6
	dynmultipush NvAltar_Text_NatureRelaxed, 7
	dynmultipush NvAltar_Text_NatureImpish, 8
	dynmultipush NvAltar_Text_NatureLax, 9
	dynmultipush NvAltar_Text_NatureTimid, 10
	dynmultipush NvAltar_Text_NatureHasty, 11
	dynmultipush NvAltar_Text_NatureSerious, 12
	dynmultipush NvAltar_Text_NatureJolly, 13
	dynmultipush NvAltar_Text_NatureNaive, 14
	dynmultipush NvAltar_Text_NatureModest, 15
	dynmultipush NvAltar_Text_NatureMild, 16
	dynmultipush NvAltar_Text_NatureQuiet, 17
	dynmultipush NvAltar_Text_NatureBashful, 18
	dynmultipush NvAltar_Text_NatureRash, 19
	dynmultipush NvAltar_Text_NatureCalm, 20
	dynmultipush NvAltar_Text_NatureGentle, 21
	dynmultipush NvAltar_Text_NatureSassy, 22
	dynmultipush NvAltar_Text_NatureCareful, 23
	dynmultipush NvAltar_Text_NatureQuirky, 24
	dynmultistack 0, 0, TRUE, 7, FALSE, 0, NULL
	switch VAR_RESULT
	case MULTI_B_PRESSED, NvAltar_EventScript_Cancel
	special SetHiddenNature
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
	.string "This altar echoes with forgotten\n"
	.string "techniques… Choose a POKéMON to\l"
	.string "remember a move.$"

NvAltar_Text_AbilityIntro:
	.string "This altar can awaken a different\n"
	.string "talent within one POKéMON.\p"
	.string "Whose ability shall I shift?$"

NvAltar_Text_AbilityDone:
	.string "Its ability is now {STR_VAR_2}!$"

NvAltar_Text_AbilityNoChange:
	.string "This POKéMON has only one talent.\n"
	.string "Nothing changed.$"

NvAltar_Text_NatureIntro:
	.string "This altar can reshape a POKéMON's\n"
	.string "very nature. Choose one, then a\l"
	.string "new nature.$"

NvAltar_Text_NatureDone:
	.string "Its nature has been reshaped!$"

NvAltar_Text_Spent:
	.string "The altar's light has faded.\n"
	.string "It will not answer again this run.$"

NvAltar_Text_NatureHardy:
	.string "Hardy$"
NvAltar_Text_NatureLonely:
	.string "Lonely$"
NvAltar_Text_NatureBrave:
	.string "Brave$"
NvAltar_Text_NatureAdamant:
	.string "Adamant$"
NvAltar_Text_NatureNaughty:
	.string "Naughty$"
NvAltar_Text_NatureBold:
	.string "Bold$"
NvAltar_Text_NatureDocile:
	.string "Docile$"
NvAltar_Text_NatureRelaxed:
	.string "Relaxed$"
NvAltar_Text_NatureImpish:
	.string "Impish$"
NvAltar_Text_NatureLax:
	.string "Lax$"
NvAltar_Text_NatureTimid:
	.string "Timid$"
NvAltar_Text_NatureHasty:
	.string "Hasty$"
NvAltar_Text_NatureSerious:
	.string "Serious$"
NvAltar_Text_NatureJolly:
	.string "Jolly$"
NvAltar_Text_NatureNaive:
	.string "Naive$"
NvAltar_Text_NatureModest:
	.string "Modest$"
NvAltar_Text_NatureMild:
	.string "Mild$"
NvAltar_Text_NatureQuiet:
	.string "Quiet$"
NvAltar_Text_NatureBashful:
	.string "Bashful$"
NvAltar_Text_NatureRash:
	.string "Rash$"
NvAltar_Text_NatureCalm:
	.string "Calm$"
NvAltar_Text_NatureGentle:
	.string "Gentle$"
NvAltar_Text_NatureSassy:
	.string "Sassy$"
NvAltar_Text_NatureCareful:
	.string "Careful$"
NvAltar_Text_NatureQuirky:
	.string "Quirky$"
