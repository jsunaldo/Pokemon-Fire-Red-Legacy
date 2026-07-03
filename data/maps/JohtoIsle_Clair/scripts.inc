JohtoIsle_Clair_MapScripts::
	.byte 0

JohtoIsle_Clair_EventScript_Leader::
	lock
	faceplayer
	goto_if_set FLAG_BEAT_JOHTO_CLAIR, JohtoIsle_Clair_EventScript_LeaderPostBattle
	msgbox JohtoIsle_Clair_Text_LeaderPreBattle, MSGBOX_DEFAULT
	trainerbattle_single TRAINER_JOHTO_CLAIR, JohtoIsle_Clair_Text_LeaderIntro, JohtoIsle_Clair_Text_LeaderDefeat
	setflag FLAG_JOHTO_CIRCUIT_CLEARED
	msgbox JohtoIsle_Clair_Text_LeaderAfter, MSGBOX_DEFAULT
	release
	end

JohtoIsle_Clair_EventScript_LeaderPostBattle::
	msgbox JohtoIsle_Clair_Text_LeaderPostBattle, MSGBOX_DEFAULT
	release
	end

JohtoIsle_Clair_EventScript_Sailor::
	lock
	faceplayer
	goto_if_set FLAG_BEAT_JOHTO_CLAIR, JohtoIsle_Clair_EventScript_SailorOnward
	msgbox JohtoIsle_Clair_Text_SailorBlocked, MSGBOX_DEFAULT
	goto JohtoIsle_Clair_EventScript_SailorReturnPrompt

JohtoIsle_Clair_EventScript_SailorOnward::
	goto JohtoIsle_Clair_EventScript_SailorReturnPrompt

JohtoIsle_Clair_EventScript_SailorReturnPrompt::
	msgbox JohtoIsle_Clair_Text_SailorReturn, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, JohtoIsle_Clair_EventScript_SailorBye
	warp MAP_ONE_ISLAND_HARBOR, 8, 5
	waitstate
	release
	end

JohtoIsle_Clair_EventScript_SailorBye::
	release
	end
