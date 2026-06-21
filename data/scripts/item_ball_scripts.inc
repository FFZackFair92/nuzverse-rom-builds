Common_EventScript_FindItem::
	callnative GetItemBallIdAndAmountFromTemplate
	finditem VAR_RESULT VAR_0x8009
	end

@ Nuzverse (Hoenn): ex-bacca = poke-ball con strumento RANDOM seedato. NvGetBerryRandomItem
@ mette item+1 in VAR_RESULT/VAR_0x8009, poi finditem da' l'oggetto e nasconde lo spot (flag).
NV_EventScript_BerryRandomItem::
	callnative NvGetBerryRandomItem
	finditem VAR_RESULT VAR_0x8009
	end
