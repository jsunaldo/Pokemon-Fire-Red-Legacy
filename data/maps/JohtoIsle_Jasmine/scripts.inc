JohtoIsle_Jasmine_MapScripts::
	.byte 0

JohtoIsle_Jasmine_EventScript_Leader::
	lock
	faceplayer
	goto_if_set FLAG_BEAT_JOHTO_JASMINE, JohtoIsle_Jasmine_EventScript_LeaderPostBattle
	msgbox JohtoIsle_Jasmine_Text_LeaderPreBattle, MSGBOX_DEFAULT
	trainerbattle_single TRAINER_JOHTO_JASMINE, JohtoIsle_Jasmine_Text_LeaderIntro, JohtoIsle_Jasmine_Text_LeaderDefeat
	msgbox JohtoIsle_Jasmine_Text_LeaderAfter, MSGBOX_DEFAULT
	release
	end

JohtoIsle_Jasmine_EventScript_LeaderPostBattle::
	msgbox JohtoIsle_Jasmine_Text_LeaderPostBattle, MSGBOX_DEFAULT
	release
	end

JohtoIsle_Jasmine_EventScript_Sailor::
	lock
	faceplayer
	goto_if_set FLAG_BEAT_JOHTO_JASMINE, JohtoIsle_Jasmine_EventScript_SailorOnward
	msgbox JohtoIsle_Jasmine_Text_SailorBlocked, MSGBOX_DEFAULT
	goto JohtoIsle_Jasmine_EventScript_SailorReturnPrompt

JohtoIsle_Jasmine_EventScript_SailorOnward::
	msgbox JohtoIsle_Jasmine_Text_SailorOnward, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, JohtoIsle_Jasmine_EventScript_SailNext
	goto JohtoIsle_Jasmine_EventScript_SailorReturnPrompt

JohtoIsle_Jasmine_EventScript_SailNext::
	warp MAP_JOHTO_ISLE_PRYCE, 18, 37
	waitstate
	release
	end

JohtoIsle_Jasmine_EventScript_SailorReturnPrompt::
	msgbox JohtoIsle_Jasmine_Text_SailorReturn, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, JohtoIsle_Jasmine_EventScript_SailorBye
	warp MAP_ONE_ISLAND_HARBOR, 8, 5
	waitstate
	release
	end

JohtoIsle_Jasmine_EventScript_SailorBye::
	release
	end
