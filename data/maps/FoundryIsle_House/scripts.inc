FoundryIsle_House_MapScripts::
	.byte 0

FoundryIsle_House_EventScript_Smith::
	lock
	faceplayer
	goto_if_set FLAG_GOT_FOUNDRY_MAGNET, FoundryIsle_House_EventScript_SmithAfter
	msgbox FoundryIsle_House_Text_SmithGift
	checkitemspace ITEM_MAGNET
	goto_if_eq VAR_RESULT, FALSE, FoundryIsle_House_EventScript_SmithNoRoom
	giveitem_msg FoundryIsle_House_Text_SmithReceived, ITEM_MAGNET
	setflag FLAG_GOT_FOUNDRY_MAGNET
	msgbox FoundryIsle_House_Text_SmithAfter
	release
	end

FoundryIsle_House_EventScript_SmithAfter::
	msgbox FoundryIsle_House_Text_SmithAfter
	release
	end

FoundryIsle_House_EventScript_SmithNoRoom::
	msgbox FoundryIsle_House_Text_SmithNoRoom
	release
	end

