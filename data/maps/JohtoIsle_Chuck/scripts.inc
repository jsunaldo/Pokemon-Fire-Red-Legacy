JohtoIsle_Chuck_MapScripts::
	.byte 0

JohtoIsle_Chuck_EventScript_Leader::
	lock
	faceplayer
	goto_if_set FLAG_BEAT_JOHTO_CHUCK, JohtoIsle_Chuck_EventScript_LeaderPostBattle
	msgbox JohtoIsle_Chuck_Text_LeaderPreBattle, MSGBOX_DEFAULT
	trainerbattle_single TRAINER_JOHTO_CHUCK, JohtoIsle_Chuck_Text_LeaderIntro, JohtoIsle_Chuck_Text_LeaderDefeat, JohtoIsle_Chuck_EventScript_LeaderDefeated
	release
	end

@ Continue script: a plain trainerbattle_single ends the script after
@ the battle, so the after-battle line has to live here.
JohtoIsle_Chuck_EventScript_LeaderDefeated::
	msgbox JohtoIsle_Chuck_Text_LeaderAfter, MSGBOX_DEFAULT
	release
	end

JohtoIsle_Chuck_EventScript_LeaderPostBattle::
	goto_if_set FLAG_E4_TIER3_CLEARED, JohtoIsle_Chuck_EventScript_LeaderRematch100
	msgbox JohtoIsle_Chuck_Text_LeaderPostBattle, MSGBOX_DEFAULT
	release
	end

JohtoIsle_Chuck_EventScript_Sailor::
	lock
	faceplayer
	goto_if_unset FLAG_BEAT_JOHTO_CHUCK, JohtoIsle_Chuck_EventScript_SailorBlocked
	goto_if_unset FLAG_BEAT_HOENN_BRAWLY, JohtoIsle_Chuck_EventScript_SailorBlocked
	goto JohtoIsle_Chuck_EventScript_SailorOnward

JohtoIsle_Chuck_EventScript_SailorBlocked::
	msgbox JohtoIsle_Chuck_Text_SailorBlocked, MSGBOX_DEFAULT
	goto JohtoIsle_Chuck_EventScript_SailorReturnPrompt

JohtoIsle_Chuck_EventScript_SailorOnward::
	msgbox JohtoIsle_Chuck_Text_SailorOnward, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, JohtoIsle_Chuck_EventScript_SailNext
	goto JohtoIsle_Chuck_EventScript_SailorReturnPrompt

JohtoIsle_Chuck_EventScript_SailNext::
	warp MAP_JOHTO_ISLE_JASMINE, 18, 29
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

JohtoIsle_Chuck_EventScript_HoennBRAWLY::
	lock
	faceplayer
	goto_if_set FLAG_BEAT_HOENN_BRAWLY, JohtoIsle_Chuck_EventScript_HoennBRAWLYPost
	msgbox JohtoIsle_Chuck_Text_HoennBRAWLYPre, MSGBOX_DEFAULT
	trainerbattle_single TRAINER_HOENN_BRAWLY, JohtoIsle_Chuck_Text_HoennBRAWLYIntro, JohtoIsle_Chuck_Text_HoennBRAWLYDefeat, JohtoIsle_Chuck_EventScript_LeaderPostBattleDefeated
	release
	end

@ Continue script: a plain trainerbattle_single ends the script after
@ the battle, so the after-battle line has to live here.
JohtoIsle_Chuck_EventScript_LeaderPostBattleDefeated::
	msgbox JohtoIsle_Chuck_Text_HoennBRAWLYAfter, MSGBOX_DEFAULT
	release
	end

JohtoIsle_Chuck_EventScript_HoennBRAWLYPost::
	goto_if_set FLAG_E4_TIER3_CLEARED, JohtoIsle_Chuck_EventScript_HoennBRAWLYRematch100
	msgbox JohtoIsle_Chuck_Text_HoennBRAWLYPostBattle, MSGBOX_DEFAULT
	release
	end

@ Reforged: after the final League, every Crown Isles leader rematches at Lv 100
@ (levels set in battle_main.c). Own continue script so the first-win side
@ effects (circuit progress) never re-run.
JohtoIsle_Chuck_EventScript_LeaderRematch100::
	msgbox JohtoIsle_Chuck_Text_CrownRematchAsk, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, JohtoIsle_Chuck_EventScript_CrownRematchNo
	cleartrainerflag TRAINER_JOHTO_CHUCK
	trainerbattle_single TRAINER_JOHTO_CHUCK, JohtoIsle_Chuck_Text_LeaderIntro, JohtoIsle_Chuck_Text_LeaderDefeat, JohtoIsle_Chuck_EventScript_CrownRematchDone
	release
	end

@ Reforged: after the final League, every Crown Isles leader rematches at Lv 100
@ (levels set in battle_main.c). Own continue script so the first-win side
@ effects (circuit progress) never re-run.
JohtoIsle_Chuck_EventScript_HoennBRAWLYRematch100::
	msgbox JohtoIsle_Chuck_Text_CrownRematchAsk, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, JohtoIsle_Chuck_EventScript_CrownRematchNo
	cleartrainerflag TRAINER_HOENN_BRAWLY
	trainerbattle_single TRAINER_HOENN_BRAWLY, JohtoIsle_Chuck_Text_HoennBRAWLYIntro, JohtoIsle_Chuck_Text_HoennBRAWLYDefeat, JohtoIsle_Chuck_EventScript_CrownRematchDone
	release
	end

JohtoIsle_Chuck_EventScript_CrownRematchNo::
	msgbox JohtoIsle_Chuck_Text_CrownRematchNo
	release
	end

JohtoIsle_Chuck_EventScript_CrownRematchDone::
	msgbox JohtoIsle_Chuck_Text_CrownRematchDone
	release
	end
