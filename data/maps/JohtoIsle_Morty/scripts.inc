JohtoIsle_Morty_MapScripts::
	.byte 0

JohtoIsle_Morty_EventScript_Leader::
	lock
	faceplayer
	goto_if_set FLAG_BEAT_JOHTO_MORTY, JohtoIsle_Morty_EventScript_LeaderPostBattle
	msgbox JohtoIsle_Morty_Text_LeaderPreBattle, MSGBOX_DEFAULT
	trainerbattle_single TRAINER_JOHTO_MORTY, JohtoIsle_Morty_Text_LeaderIntro, JohtoIsle_Morty_Text_LeaderDefeat, JohtoIsle_Morty_EventScript_LeaderDefeated
	release
	end

@ Continue script: a plain trainerbattle_single ends the script after
@ the battle, so the after-battle line has to live here.
JohtoIsle_Morty_EventScript_LeaderDefeated::
	msgbox JohtoIsle_Morty_Text_LeaderAfter, MSGBOX_DEFAULT
	release
	end

JohtoIsle_Morty_EventScript_LeaderPostBattle::
	goto_if_set FLAG_E4_TIER3_CLEARED, JohtoIsle_Morty_EventScript_LeaderRematch100
	msgbox JohtoIsle_Morty_Text_LeaderPostBattle, MSGBOX_DEFAULT
	release
	end

JohtoIsle_Morty_EventScript_Sailor::
	lock
	faceplayer
	goto_if_unset FLAG_BEAT_JOHTO_MORTY, JohtoIsle_Morty_EventScript_SailorBlocked
	goto_if_unset FLAG_BEAT_HOENN_TATE_LIZA, JohtoIsle_Morty_EventScript_SailorBlocked
	goto JohtoIsle_Morty_EventScript_SailorOnward

JohtoIsle_Morty_EventScript_SailorBlocked::
	msgbox JohtoIsle_Morty_Text_SailorBlocked, MSGBOX_DEFAULT
	goto JohtoIsle_Morty_EventScript_SailorReturnPrompt

JohtoIsle_Morty_EventScript_SailorOnward::
	msgbox JohtoIsle_Morty_Text_SailorOnward, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, JohtoIsle_Morty_EventScript_SailNext
	goto JohtoIsle_Morty_EventScript_SailorReturnPrompt

JohtoIsle_Morty_EventScript_SailNext::
	warp MAP_JOHTO_ISLE_CHUCK, 16, 41
	waitstate
	release
	end

JohtoIsle_Morty_EventScript_SailorReturnPrompt::
	msgbox JohtoIsle_Morty_Text_SailorReturn, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, JohtoIsle_Morty_EventScript_SailorBye
	warp MAP_ONE_ISLAND_HARBOR, 8, 5
	waitstate
	release
	end

JohtoIsle_Morty_EventScript_SailorBye::
	release
	end

JohtoIsle_Morty_EventScript_HoennTATELIZA::
	lock
	faceplayer
	goto_if_set FLAG_BEAT_HOENN_TATE_LIZA, JohtoIsle_Morty_EventScript_HoennTATELIZAPost
	msgbox JohtoIsle_Morty_Text_HoennTATELIZAPre, MSGBOX_DEFAULT
	trainerbattle_single TRAINER_HOENN_TATE_LIZA, JohtoIsle_Morty_Text_HoennTATELIZAIntro, JohtoIsle_Morty_Text_HoennTATELIZADefeat, JohtoIsle_Morty_EventScript_LeaderPostBattleDefeated
	release
	end

@ Continue script: a plain trainerbattle_single ends the script after
@ the battle, so the after-battle line has to live here.
JohtoIsle_Morty_EventScript_LeaderPostBattleDefeated::
	msgbox JohtoIsle_Morty_Text_HoennTATELIZAAfter, MSGBOX_DEFAULT
	release
	end

JohtoIsle_Morty_EventScript_HoennTATELIZAPost::
	goto_if_set FLAG_E4_TIER3_CLEARED, JohtoIsle_Morty_EventScript_HoennTATELIZARematch100
	msgbox JohtoIsle_Morty_Text_HoennTATELIZAPostBattle, MSGBOX_DEFAULT
	release
	end

@ Reforged: after the final League, every Crown Isles leader rematches at Lv 100
@ (levels set in battle_main.c). Own continue script so the first-win side
@ effects (circuit progress) never re-run.
JohtoIsle_Morty_EventScript_LeaderRematch100::
	msgbox JohtoIsle_Morty_Text_CrownRematchAsk, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, JohtoIsle_Morty_EventScript_CrownRematchNo
	cleartrainerflag TRAINER_JOHTO_MORTY
	trainerbattle_single TRAINER_JOHTO_MORTY, JohtoIsle_Morty_Text_LeaderIntro, JohtoIsle_Morty_Text_LeaderDefeat, JohtoIsle_Morty_EventScript_CrownRematchDone
	release
	end

@ Reforged: after the final League, every Crown Isles leader rematches at Lv 100
@ (levels set in battle_main.c). Own continue script so the first-win side
@ effects (circuit progress) never re-run.
JohtoIsle_Morty_EventScript_HoennTATELIZARematch100::
	msgbox JohtoIsle_Morty_Text_CrownRematchAsk, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, JohtoIsle_Morty_EventScript_CrownRematchNo
	cleartrainerflag TRAINER_HOENN_TATE_LIZA
	trainerbattle_single TRAINER_HOENN_TATE_LIZA, JohtoIsle_Morty_Text_HoennTATELIZAIntro, JohtoIsle_Morty_Text_HoennTATELIZADefeat, JohtoIsle_Morty_EventScript_CrownRematchDone
	release
	end

JohtoIsle_Morty_EventScript_CrownRematchNo::
	msgbox JohtoIsle_Morty_Text_CrownRematchNo
	release
	end

JohtoIsle_Morty_EventScript_CrownRematchDone::
	msgbox JohtoIsle_Morty_Text_CrownRematchDone
	release
	end
