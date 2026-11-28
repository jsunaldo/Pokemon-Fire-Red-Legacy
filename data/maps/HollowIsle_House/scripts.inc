HollowIsle_House_MapScripts::
	.byte 0

HollowIsle_House_EventScript_OldWoman::
	lock
	faceplayer
	goto_if_set FLAG_GOT_HOLLOW_SPELL_TAG, HollowIsle_House_EventScript_OldWomanAfter
	msgbox HollowIsle_House_Text_OldWomanGift
	checkitemspace ITEM_SPELL_TAG
	goto_if_eq VAR_RESULT, FALSE, HollowIsle_House_EventScript_OldWomanNoRoom
	giveitem_msg HollowIsle_House_Text_OldWomanReceived, ITEM_SPELL_TAG
	setflag FLAG_GOT_HOLLOW_SPELL_TAG
	msgbox HollowIsle_House_Text_OldWomanAfter
	release
	end

HollowIsle_House_EventScript_OldWomanAfter::
	msgbox HollowIsle_House_Text_OldWomanAfter
	release
	end

HollowIsle_House_EventScript_OldWomanNoRoom::
	msgbox HollowIsle_House_Text_OldWomanNoRoom
	release
	end

