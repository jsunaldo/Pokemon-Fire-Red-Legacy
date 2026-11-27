JohtoIsle_Jasmine_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, JohtoIsle_Jasmine_OnTransition
	.byte 0

JohtoIsle_Jasmine_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_CROWN_FOUNDRY
	end

JohtoIsle_Jasmine_EventScript_Leader::
	lock
	faceplayer
	goto_if_set FLAG_BEAT_JOHTO_JASMINE, JohtoIsle_Jasmine_EventScript_LeaderPostBattle
	msgbox JohtoIsle_Jasmine_Text_LeaderPreBattle, MSGBOX_DEFAULT
	trainerbattle_single TRAINER_JOHTO_JASMINE, JohtoIsle_Jasmine_Text_LeaderIntro, JohtoIsle_Jasmine_Text_LeaderDefeat, JohtoIsle_Jasmine_EventScript_LeaderDefeated
	release
	end

@ Continue script: a plain trainerbattle_single ends the script after
@ the battle, so the after-battle line has to live here.
JohtoIsle_Jasmine_EventScript_LeaderDefeated::
	msgbox JohtoIsle_Jasmine_Text_LeaderAfter, MSGBOX_DEFAULT
	release
	end

JohtoIsle_Jasmine_EventScript_LeaderPostBattle::
	goto_if_set FLAG_E4_TIER3_CLEARED, JohtoIsle_Jasmine_EventScript_LeaderRematch100
	msgbox JohtoIsle_Jasmine_Text_LeaderPostBattle, MSGBOX_DEFAULT
	release
	end

@ Reforged: this isle's dock is a Seagallop port. The captain sails to
@ VERMILION, ONE-SEVEN ISLAND and every unlocked CROWN ISLE.
JohtoIsle_Jasmine_EventScript_Sailor::
	lock
	faceplayer
	setvar VAR_0x8004, SEAGALLOP_CROWN_FOUNDRY
	goto EventScript_CrownIsleCaptain
	end

JohtoIsle_Jasmine_EventScript_HoennROXANNE::
	lock
	faceplayer
	goto_if_set FLAG_BEAT_HOENN_ROXANNE, JohtoIsle_Jasmine_EventScript_HoennROXANNEPost
	msgbox JohtoIsle_Jasmine_Text_HoennROXANNEPre, MSGBOX_DEFAULT
	trainerbattle_single TRAINER_HOENN_ROXANNE, JohtoIsle_Jasmine_Text_HoennROXANNEIntro, JohtoIsle_Jasmine_Text_HoennROXANNEDefeat, JohtoIsle_Jasmine_EventScript_LeaderPostBattleDefeated
	release
	end

@ Continue script: a plain trainerbattle_single ends the script after
@ the battle, so the after-battle line has to live here.
JohtoIsle_Jasmine_EventScript_LeaderPostBattleDefeated::
	msgbox JohtoIsle_Jasmine_Text_HoennROXANNEAfter, MSGBOX_DEFAULT
	release
	end

JohtoIsle_Jasmine_EventScript_HoennROXANNEPost::
	goto_if_set FLAG_E4_TIER3_CLEARED, JohtoIsle_Jasmine_EventScript_HoennROXANNERematch100
	msgbox JohtoIsle_Jasmine_Text_HoennROXANNEPostBattle, MSGBOX_DEFAULT
	release
	end

@ Reforged: after the final League, every Crown Isles leader rematches at Lv 100
@ (levels set in battle_main.c). Own continue script so the first-win side
@ effects (circuit progress) never re-run.
JohtoIsle_Jasmine_EventScript_LeaderRematch100::
	msgbox JohtoIsle_Jasmine_Text_CrownRematchAsk, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, JohtoIsle_Jasmine_EventScript_CrownRematchNo
	cleartrainerflag TRAINER_JOHTO_JASMINE
	trainerbattle_single TRAINER_JOHTO_JASMINE, JohtoIsle_Jasmine_Text_LeaderIntro, JohtoIsle_Jasmine_Text_LeaderDefeat, JohtoIsle_Jasmine_EventScript_CrownRematchDone
	release
	end

@ Reforged: after the final League, every Crown Isles leader rematches at Lv 100
@ (levels set in battle_main.c). Own continue script so the first-win side
@ effects (circuit progress) never re-run.
JohtoIsle_Jasmine_EventScript_HoennROXANNERematch100::
	msgbox JohtoIsle_Jasmine_Text_CrownRematchAsk, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, JohtoIsle_Jasmine_EventScript_CrownRematchNo
	cleartrainerflag TRAINER_HOENN_ROXANNE
	trainerbattle_single TRAINER_HOENN_ROXANNE, JohtoIsle_Jasmine_Text_HoennROXANNEIntro, JohtoIsle_Jasmine_Text_HoennROXANNEDefeat, JohtoIsle_Jasmine_EventScript_CrownRematchDone
	release
	end

JohtoIsle_Jasmine_EventScript_CrownRematchNo::
	msgbox JohtoIsle_Jasmine_Text_CrownRematchNo
	release
	end

JohtoIsle_Jasmine_EventScript_CrownRematchDone::
	msgbox JohtoIsle_Jasmine_Text_CrownRematchDone
	release
	end
