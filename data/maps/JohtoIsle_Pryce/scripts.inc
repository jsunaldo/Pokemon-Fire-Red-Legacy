JohtoIsle_Pryce_MapScripts::
	.byte 0

JohtoIsle_Pryce_EventScript_Leader::
	lock
	faceplayer
	goto_if_set FLAG_BEAT_JOHTO_PRYCE, JohtoIsle_Pryce_EventScript_LeaderPostBattle
	msgbox JohtoIsle_Pryce_Text_LeaderPreBattle, MSGBOX_DEFAULT
	trainerbattle_single TRAINER_JOHTO_PRYCE, JohtoIsle_Pryce_Text_LeaderIntro, JohtoIsle_Pryce_Text_LeaderDefeat, JohtoIsle_Pryce_EventScript_LeaderDefeated
	release
	end

@ Continue script: a plain trainerbattle_single ends the script after
@ the battle, so the after-battle line has to live here.
JohtoIsle_Pryce_EventScript_LeaderDefeated::
	msgbox JohtoIsle_Pryce_Text_LeaderAfter, MSGBOX_DEFAULT
	release
	end

JohtoIsle_Pryce_EventScript_LeaderPostBattle::
	goto_if_set FLAG_E4_TIER3_CLEARED, JohtoIsle_Pryce_EventScript_LeaderRematch100
	msgbox JohtoIsle_Pryce_Text_LeaderPostBattle, MSGBOX_DEFAULT
	release
	end

JohtoIsle_Pryce_EventScript_Sailor::
	lock
	faceplayer
	goto_if_unset FLAG_BEAT_JOHTO_PRYCE, JohtoIsle_Pryce_EventScript_SailorBlocked
	goto_if_unset FLAG_BEAT_HOENN_WALLACE, JohtoIsle_Pryce_EventScript_SailorBlocked
	goto JohtoIsle_Pryce_EventScript_SailorOnward

JohtoIsle_Pryce_EventScript_SailorBlocked::
	msgbox JohtoIsle_Pryce_Text_SailorBlocked, MSGBOX_DEFAULT
	goto JohtoIsle_Pryce_EventScript_SailorReturnPrompt

JohtoIsle_Pryce_EventScript_SailorOnward::
	msgbox JohtoIsle_Pryce_Text_SailorOnward, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, JohtoIsle_Pryce_EventScript_SailNext
	goto JohtoIsle_Pryce_EventScript_SailorReturnPrompt

JohtoIsle_Pryce_EventScript_SailNext::
	warp MAP_JOHTO_ISLE_CLAIR, 15, 36
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

JohtoIsle_Pryce_EventScript_HoennWALLACE::
	lock
	faceplayer
	goto_if_set FLAG_BEAT_HOENN_WALLACE, JohtoIsle_Pryce_EventScript_HoennWALLACEPost
	msgbox JohtoIsle_Pryce_Text_HoennWALLACEPre, MSGBOX_DEFAULT
	trainerbattle_single TRAINER_HOENN_WALLACE, JohtoIsle_Pryce_Text_HoennWALLACEIntro, JohtoIsle_Pryce_Text_HoennWALLACEDefeat, JohtoIsle_Pryce_EventScript_LeaderPostBattleDefeated
	release
	end

@ Continue script: a plain trainerbattle_single ends the script after
@ the battle, so the after-battle line has to live here.
JohtoIsle_Pryce_EventScript_LeaderPostBattleDefeated::
	msgbox JohtoIsle_Pryce_Text_HoennWALLACEAfter, MSGBOX_DEFAULT
	release
	end

JohtoIsle_Pryce_EventScript_HoennWALLACEPost::
	goto_if_set FLAG_E4_TIER3_CLEARED, JohtoIsle_Pryce_EventScript_HoennWALLACERematch100
	msgbox JohtoIsle_Pryce_Text_HoennWALLACEPostBattle, MSGBOX_DEFAULT
	release
	end

@ Reforged: after the final League, every Crown Isles leader rematches at Lv 100
@ (levels set in battle_main.c). Own continue script so the first-win side
@ effects (circuit progress) never re-run.
JohtoIsle_Pryce_EventScript_LeaderRematch100::
	msgbox JohtoIsle_Pryce_Text_CrownRematchAsk, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, JohtoIsle_Pryce_EventScript_CrownRematchNo
	cleartrainerflag TRAINER_JOHTO_PRYCE
	trainerbattle_single TRAINER_JOHTO_PRYCE, JohtoIsle_Pryce_Text_LeaderIntro, JohtoIsle_Pryce_Text_LeaderDefeat, JohtoIsle_Pryce_EventScript_CrownRematchDone
	release
	end

@ Reforged: after the final League, every Crown Isles leader rematches at Lv 100
@ (levels set in battle_main.c). Own continue script so the first-win side
@ effects (circuit progress) never re-run.
JohtoIsle_Pryce_EventScript_HoennWALLACERematch100::
	msgbox JohtoIsle_Pryce_Text_CrownRematchAsk, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, JohtoIsle_Pryce_EventScript_CrownRematchNo
	cleartrainerflag TRAINER_HOENN_WALLACE
	trainerbattle_single TRAINER_HOENN_WALLACE, JohtoIsle_Pryce_Text_HoennWALLACEIntro, JohtoIsle_Pryce_Text_HoennWALLACEDefeat, JohtoIsle_Pryce_EventScript_CrownRematchDone
	release
	end

JohtoIsle_Pryce_EventScript_CrownRematchNo::
	msgbox JohtoIsle_Pryce_Text_CrownRematchNo
	release
	end

JohtoIsle_Pryce_EventScript_CrownRematchDone::
	msgbox JohtoIsle_Pryce_Text_CrownRematchDone
	release
	end
