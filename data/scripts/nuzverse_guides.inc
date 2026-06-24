@ ===== Nuzverse: guide DEOXYS in ogni citta' (indicazioni sul da farsi) =====
@ Un NPC Deoxys per citta' (sprite OBJ_EVENT_GFX_DEOXYS) che ricorda obiettivo + prossima
@ destinazione, secondo l'ordine linearizzato. Oldale ha gia' la sua (OldaleTown_EventScript_MewtwoGuide).
@ Pattern: lock / faceplayer / msgbox / release / end. Testo: narrazione IT + nomi luogo come in-game.

@ ---------- HOENN ----------
NvGuide_Littleroot::
	lock
	faceplayer
	msgbox NvGuide_Text_Littleroot, MSGBOX_DEFAULT
	release
	end
NvGuide_Text_Littleroot:
	.string "DEOXYS: La tua avventura comincia.\p"
	.string "Sali a NORD fino a OLDALE, poi a\n"
	.string "OVEST verso PETALBURG.$"

NvGuide_Petalburg::
	lock
	faceplayer
	msgbox NvGuide_Text_Petalburg, MSGBOX_DEFAULT
	release
	end
NvGuide_Text_Petalburg:
	.string "DEOXYS: Attraversa il BOSCO a OVEST\n"
	.string "fino a RUSTBORO: la 1ª PALESTRA.\p"
	.string "La palestra di NORMAN qui si apre\n"
	.string "con 4 MEDAGLIE.$"

NvGuide_Rustboro::
	lock
	faceplayer
	msgbox NvGuide_Text_Rustboro, MSGBOX_DEFAULT
	release
	end
NvGuide_Text_Rustboro:
	.string "DEOXYS: 1ª PALESTRA - ROXANNE.\p"
	.string "Poi scendi a SUD e imbarcati con\n"
	.string "BRINEY verso DEWFORD.$"

NvGuide_Dewford::
	lock
	faceplayer
	msgbox NvGuide_Text_Dewford, MSGBOX_DEFAULT
	release
	end
NvGuide_Text_Dewford:
	.string "DEOXYS: 2ª PALESTRA - BRAWLY.\p"
	.string "Poi riprendi la barca per\n"
	.string "SLATEPORT.$"

NvGuide_Slateport::
	lock
	faceplayer
	msgbox NvGuide_Text_Slateport, MSGBOX_DEFAULT
	release
	end
NvGuide_Text_Slateport:
	.string "DEOXYS: Qui niente palestra.\p"
	.string "Sali a NORD verso MAUVILLE.$"

NvGuide_Mauville::
	lock
	faceplayer
	msgbox NvGuide_Text_Mauville, MSGBOX_DEFAULT
	release
	end
NvGuide_Text_Mauville:
	.string "DEOXYS: 3ª PALESTRA - WATTSON.\p"
	.string "Poi sali a NORD verso LAVARIDGE\n"
	.string "passando per il SENTIERO ROVENTE.$"

NvGuide_Verdanturf::
	lock
	faceplayer
	msgbox NvGuide_Text_Verdanturf, MSGBOX_DEFAULT
	release
	end
NvGuide_Text_Verdanturf:
	.string "DEOXYS: Cittadina di passaggio.\p"
	.string "Punta a LAVARIDGE per la 4ª\n"
	.string "MEDAGLIA.$"

NvGuide_Fallarbor::
	lock
	faceplayer
	msgbox NvGuide_Text_Fallarbor, MSGBOX_DEFAULT
	release
	end
NvGuide_Text_Fallarbor:
	.string "DEOXYS: Zona nord.\p"
	.string "Dirigiti a LAVARIDGE per la 4ª\n"
	.string "PALESTRA.$"

NvGuide_Lavaridge::
	lock
	faceplayer
	msgbox NvGuide_Text_Lavaridge, MSGBOX_DEFAULT
	release
	end
NvGuide_Text_Lavaridge:
	.string "DEOXYS: 4ª PALESTRA - FLANNERY.\p"
	.string "Poi torna a PETALBURG per sfidare\n"
	.string "NORMAN, la 5ª PALESTRA.$"

NvGuide_Fortree::
	lock
	faceplayer
	msgbox NvGuide_Text_Fortree, MSGBOX_DEFAULT
	release
	end
NvGuide_Text_Fortree:
	.string "DEOXYS: 6ª PALESTRA - WINONA.\p"
	.string "Poi vai a EST verso LILYCOVE e\n"
	.string "MOSSDEEP.$"

NvGuide_Lilycove::
	lock
	faceplayer
	msgbox NvGuide_Text_Lilycove, MSGBOX_DEFAULT
	release
	end
NvGuide_Text_Lilycove:
	.string "DEOXYS: Qui niente palestra.\p"
	.string "Prosegui in MARE verso MOSSDEEP.$"

NvGuide_Mossdeep::
	lock
	faceplayer
	msgbox NvGuide_Text_Mossdeep, MSGBOX_DEFAULT
	release
	end
NvGuide_Text_Mossdeep:
	.string "DEOXYS: 7ª PALESTRA - TATE e LIZA.\p"
	.string "Poi raggiungi SOOTOPOLIS in mare.$"

NvGuide_Sootopolis::
	lock
	faceplayer
	msgbox NvGuide_Text_Sootopolis, MSGBOX_DEFAULT
	release
	end
NvGuide_Text_Sootopolis:
	.string "DEOXYS: 8ª PALESTRA - JUAN.\p"
	.string "Poi rotta verso EVER GRANDE e la\n"
	.string "LEGA POKéMON.$"

NvGuide_Pacifidlog::
	lock
	faceplayer
	msgbox NvGuide_Text_Pacifidlog, MSGBOX_DEFAULT
	release
	end
NvGuide_Text_Pacifidlog:
	.string "DEOXYS: Cittadina oceanica.\p"
	.string "Dirigiti a EVER GRANDE per la LEGA.$"

NvGuide_EverGrande::
	lock
	faceplayer
	msgbox NvGuide_Text_EverGrande, MSGBOX_DEFAULT
	release
	end
NvGuide_Text_EverGrande:
	.string "DEOXYS: Ci siamo.\p"
	.string "Attraversa la VIA VITTORIA e\n"
	.string "affronta la LEGA POKéMON!$"

@ ---------- KANTO ----------
NvGuide_Pallet::
	lock
	faceplayer
	msgbox NvGuide_Text_Pallet, MSGBOX_DEFAULT
	release
	end
NvGuide_Text_Pallet:
	.string "DEOXYS: L'inizio del viaggio.\p"
	.string "Sali a NORD verso VIRIDIAN e poi\n"
	.string "PEWTER.$"

NvGuide_Viridian::
	lock
	faceplayer
	msgbox NvGuide_Text_Viridian, MSGBOX_DEFAULT
	release
	end
NvGuide_Text_Viridian:
	.string "DEOXYS: Attraversa il BOSCO a NORD\n"
	.string "fino a PEWTER: la 1ª PALESTRA.\p"
	.string "La palestra di GIOVANNI qui (8ª) si\n"
	.string "apre piu' tardi.$"

NvGuide_Pewter::
	lock
	faceplayer
	msgbox NvGuide_Text_Pewter, MSGBOX_DEFAULT
	release
	end
NvGuide_Text_Pewter:
	.string "DEOXYS: 1ª PALESTRA - BROCK.\p"
	.string "Poi attraversa il MONTE LUNA a EST\n"
	.string "fino a CERULEAN.$"

NvGuide_Cerulean::
	lock
	faceplayer
	msgbox NvGuide_Text_Cerulean, MSGBOX_DEFAULT
	release
	end
NvGuide_Text_Cerulean:
	.string "DEOXYS: 2ª PALESTRA - MISTY.\p"
	.string "Poi scendi a SUD passando per\n"
	.string "SAFFRON fino a VERMILION.$"

NvGuide_Vermilion::
	lock
	faceplayer
	msgbox NvGuide_Text_Vermilion, MSGBOX_DEFAULT
	release
	end
NvGuide_Text_Vermilion:
	.string "DEOXYS: 3ª PALESTRA - LT. SURGE.\p"
	.string "Poi risali verso LAVENDER e poi\n"
	.string "CELADON.$"

NvGuide_Lavender::
	lock
	faceplayer
	msgbox NvGuide_Text_Lavender, MSGBOX_DEFAULT
	release
	end
NvGuide_Text_Lavender:
	.string "DEOXYS: Qui niente palestra.\p"
	.string "Vai a OVEST verso CELADON.$"

NvGuide_Celadon::
	lock
	faceplayer
	msgbox NvGuide_Text_Celadon, MSGBOX_DEFAULT
	release
	end
NvGuide_Text_Celadon:
	.string "DEOXYS: 4ª PALESTRA - ERIKA.\p"
	.string "Poi vai a SUD-OVEST verso\n"
	.string "FUCHSIA.$"

NvGuide_Saffron::
	lock
	faceplayer
	msgbox NvGuide_Text_Saffron, MSGBOX_DEFAULT
	release
	end
NvGuide_Text_Saffron:
	.string "DEOXYS: 6ª PALESTRA - SABRINA, si\n"
	.string "apre con 5 MEDAGLIE.\p"
	.string "Snodo centrale: di qui raggiungi le\n"
	.string "altre citta'.$"

NvGuide_Fuchsia::
	lock
	faceplayer
	msgbox NvGuide_Text_Fuchsia, MSGBOX_DEFAULT
	release
	end
NvGuide_Text_Fuchsia:
	.string "DEOXYS: 5ª PALESTRA - KOGA.\p"
	.string "Poi vai a OVEST in mare fino a\n"
	.string "CINNABAR.$"

NvGuide_Cinnabar::
	lock
	faceplayer
	msgbox NvGuide_Text_Cinnabar, MSGBOX_DEFAULT
	release
	end
NvGuide_Text_Cinnabar:
	.string "DEOXYS: 7ª PALESTRA - BLAINE.\p"
	.string "Torna a SAFFRON per SABRINA (6ª) e\n"
	.string "infine a VIRIDIAN per GIOVANNI (8ª).$"

NvGuide_IndigoPlateau::
	lock
	faceplayer
	msgbox NvGuide_Text_IndigoPlateau, MSGBOX_DEFAULT
	release
	end
NvGuide_Text_IndigoPlateau:
	.string "DEOXYS: Sei alla LEGA.\p"
	.string "Attraversa la VIA VITTORIA e\n"
	.string "affronta i SUPERQUATTRO!$"
