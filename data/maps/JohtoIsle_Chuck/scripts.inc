JohtoIsle_Chuck_MapScripts::
	.byte 0

JohtoIsle_Chuck_EventScript_Leader::
	lock
	faceplayer
	goto_if_set FLAG_BEAT_JOHTO_CHUCK, JohtoIsle_Chuck_EventScript_LeaderPostBattle
	msgbox JohtoIsle_Chuck_Text_LeaderPreBattle, MSGBOX_DEFAULT
	trainerbattle_single TRAINER_JOHTO_CHUCK, JohtoIsle_Chuck_Text_LeaderIntro, JohtoIsle_Chuck_Text_LeaderDefeat
	msgbox JohtoIsle_Chuck_Text_LeaderAfter, MSGBOX_DEFAULT
	release
	end

JohtoIsle_Chuck_EventScript_LeaderPostBattle::
	msgbox JohtoIsle_Chuck_Text_LeaderPostBattle, MSGBOX_DEFAULT
	release
	end

JohtoIsle_Chuck_EventScript_Sailor::
	lock
	faceplayer
	goto_if_set FLAG_BEAT_JOHTO_CHUCK, JohtoIsle_Chuck_EventScript_SailorOnward
	msgbox JohtoIsle_Chuck_Text_SailorBlocked, MSGBOX_DEFAULT
	goto JohtoIsle_Chuck_EventScript_SailorReturnPrompt

JohtoIsle_Chuck_EventScript_SailorOnward::
	msgbox JohtoIsle_Chuck_Text_SailorOnward, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, JohtoIsle_Chuck_EventScript_SailNext
	goto JohtoIsle_Chuck_EventScript_SailorReturnPrompt

JohtoIsle_Chuck_EventScript_SailNext::
	warp MAP_JOHTO_ISLE_JASMINE, 27, 48
	waitstate
	release
	end

JohtoIsle_Chuck_EventScript_SailorReturnPrompt::
	msgbox JohtoIsle_Chuck_Text_SailorReturn, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, JohtoIsle_Chuck_EventScript_SailorBye
	warp MAP_ONE_ISLAND_HARBOR, 8, 5
	waitstate
	release
	end

JohtoIsle_Chuck_EventScript_SailorBye::
	release
	end
