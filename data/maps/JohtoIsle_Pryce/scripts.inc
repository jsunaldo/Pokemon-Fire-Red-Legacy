JohtoIsle_Pryce_MapScripts::
	.byte 0

JohtoIsle_Pryce_EventScript_Leader::
	lock
	faceplayer
	goto_if_set FLAG_BEAT_JOHTO_PRYCE, JohtoIsle_Pryce_EventScript_LeaderPostBattle
	msgbox JohtoIsle_Pryce_Text_LeaderPreBattle, MSGBOX_DEFAULT
	trainerbattle_single TRAINER_JOHTO_PRYCE, JohtoIsle_Pryce_Text_LeaderIntro, JohtoIsle_Pryce_Text_LeaderDefeat
	msgbox JohtoIsle_Pryce_Text_LeaderAfter, MSGBOX_DEFAULT
	release
	end

JohtoIsle_Pryce_EventScript_LeaderPostBattle::
	msgbox JohtoIsle_Pryce_Text_LeaderPostBattle, MSGBOX_DEFAULT
	release
	end

JohtoIsle_Pryce_EventScript_Sailor::
	lock
	faceplayer
	goto_if_set FLAG_BEAT_JOHTO_PRYCE, JohtoIsle_Pryce_EventScript_SailorOnward
	msgbox JohtoIsle_Pryce_Text_SailorBlocked, MSGBOX_DEFAULT
	goto JohtoIsle_Pryce_EventScript_SailorReturnPrompt

JohtoIsle_Pryce_EventScript_SailorOnward::
	msgbox JohtoIsle_Pryce_Text_SailorOnward, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, JohtoIsle_Pryce_EventScript_SailNext
	goto JohtoIsle_Pryce_EventScript_SailorReturnPrompt

JohtoIsle_Pryce_EventScript_SailNext::
	warp MAP_JOHTO_ISLE_CLAIR, 0, 91
	waitstate
	release
	end

JohtoIsle_Pryce_EventScript_SailorReturnPrompt::
	msgbox JohtoIsle_Pryce_Text_SailorReturn, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, JohtoIsle_Pryce_EventScript_SailorBye
	warp MAP_ONE_ISLAND_HARBOR, 8, 5
	waitstate
	release
	end

JohtoIsle_Pryce_EventScript_SailorBye::
	release
	end
