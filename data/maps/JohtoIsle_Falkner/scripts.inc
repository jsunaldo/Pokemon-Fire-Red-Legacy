JohtoIsle_Falkner_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, JohtoIsle_Falkner_OnTransition
	.byte 0

JohtoIsle_Falkner_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_CROWN_SKYREACH
	end

JohtoIsle_Falkner_EventScript_Leader::
	lock
	faceplayer
	goto_if_set FLAG_BEAT_JOHTO_FALKNER, JohtoIsle_Falkner_EventScript_LeaderPostBattle
	msgbox JohtoIsle_Falkner_Text_LeaderPreBattle, MSGBOX_DEFAULT
	trainerbattle_single TRAINER_JOHTO_FALKNER, JohtoIsle_Falkner_Text_LeaderIntro, JohtoIsle_Falkner_Text_LeaderDefeat, JohtoIsle_Falkner_EventScript_LeaderDefeated
	release
	end

@ Continue script: a plain trainerbattle_single ends the script after
@ the battle, so the after-battle line has to live here.
JohtoIsle_Falkner_EventScript_LeaderDefeated::
	msgbox JohtoIsle_Falkner_Text_LeaderAfter, MSGBOX_DEFAULT
	release
	end

JohtoIsle_Falkner_EventScript_LeaderPostBattle::
	goto_if_set FLAG_E4_TIER3_CLEARED, JohtoIsle_Falkner_EventScript_LeaderRematch100
	msgbox JohtoIsle_Falkner_Text_LeaderPostBattle, MSGBOX_DEFAULT
	release
	end

@ Reforged: this isle's dock is a Seagallop port. The captain sails to
@ VERMILION, ONE-SEVEN ISLAND and every unlocked CROWN ISLE.
JohtoIsle_Falkner_EventScript_Sailor::
	lock
	faceplayer
	setvar VAR_0x8004, SEAGALLOP_CROWN_SKYREACH
	goto EventScript_CrownIsleCaptain
	end

JohtoIsle_Falkner_EventScript_HoennWINONA::
	lock
	faceplayer
	goto_if_set FLAG_BEAT_HOENN_WINONA, JohtoIsle_Falkner_EventScript_HoennWINONAPost
	msgbox JohtoIsle_Falkner_Text_HoennWINONAPre, MSGBOX_DEFAULT
	trainerbattle_single TRAINER_HOENN_WINONA, JohtoIsle_Falkner_Text_HoennWINONAIntro, JohtoIsle_Falkner_Text_HoennWINONADefeat, JohtoIsle_Falkner_EventScript_LeaderPostBattleDefeated
	release
	end

@ Continue script: a plain trainerbattle_single ends the script after
@ the battle, so the after-battle line has to live here.
JohtoIsle_Falkner_EventScript_LeaderPostBattleDefeated::
	msgbox JohtoIsle_Falkner_Text_HoennWINONAAfter, MSGBOX_DEFAULT
	release
	end

JohtoIsle_Falkner_EventScript_HoennWINONAPost::
	goto_if_set FLAG_E4_TIER3_CLEARED, JohtoIsle_Falkner_EventScript_HoennWINONARematch100
	msgbox JohtoIsle_Falkner_Text_HoennWINONAPostBattle, MSGBOX_DEFAULT
	release
	end

@ Reforged: after the final League, every Crown Isles leader rematches at Lv 100
@ (levels set in battle_main.c). Own continue script so the first-win side
@ effects (circuit progress) never re-run.
JohtoIsle_Falkner_EventScript_LeaderRematch100::
	msgbox JohtoIsle_Falkner_Text_CrownRematchAsk, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, JohtoIsle_Falkner_EventScript_CrownRematchNo
	cleartrainerflag TRAINER_JOHTO_FALKNER
	trainerbattle_single TRAINER_JOHTO_FALKNER, JohtoIsle_Falkner_Text_LeaderIntro, JohtoIsle_Falkner_Text_LeaderDefeat, JohtoIsle_Falkner_EventScript_CrownRematchDone
	release
	end

@ Reforged: after the final League, every Crown Isles leader rematches at Lv 100
@ (levels set in battle_main.c). Own continue script so the first-win side
@ effects (circuit progress) never re-run.
JohtoIsle_Falkner_EventScript_HoennWINONARematch100::
	msgbox JohtoIsle_Falkner_Text_CrownRematchAsk, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, JohtoIsle_Falkner_EventScript_CrownRematchNo
	cleartrainerflag TRAINER_HOENN_WINONA
	trainerbattle_single TRAINER_HOENN_WINONA, JohtoIsle_Falkner_Text_HoennWINONAIntro, JohtoIsle_Falkner_Text_HoennWINONADefeat, JohtoIsle_Falkner_EventScript_CrownRematchDone
	release
	end

JohtoIsle_Falkner_EventScript_CrownRematchNo::
	msgbox JohtoIsle_Falkner_Text_CrownRematchNo
	release
	end

JohtoIsle_Falkner_EventScript_CrownRematchDone::
	msgbox JohtoIsle_Falkner_Text_CrownRematchDone
	release
	end
