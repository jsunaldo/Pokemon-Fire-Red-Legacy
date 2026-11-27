JohtoIsle_Bugsy_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, JohtoIsle_Bugsy_OnTransition
	.byte 0

JohtoIsle_Bugsy_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_CROWN_CHITIN
	end

JohtoIsle_Bugsy_EventScript_Leader::
	lock
	faceplayer
	goto_if_set FLAG_BEAT_JOHTO_BUGSY, JohtoIsle_Bugsy_EventScript_LeaderPostBattle
	msgbox JohtoIsle_Bugsy_Text_LeaderPreBattle, MSGBOX_DEFAULT
	trainerbattle_single TRAINER_JOHTO_BUGSY, JohtoIsle_Bugsy_Text_LeaderIntro, JohtoIsle_Bugsy_Text_LeaderDefeat, JohtoIsle_Bugsy_EventScript_LeaderDefeated
	release
	end

@ Continue script: a plain trainerbattle_single ends the script after
@ the battle, so the after-battle line has to live here.
JohtoIsle_Bugsy_EventScript_LeaderDefeated::
	msgbox JohtoIsle_Bugsy_Text_LeaderAfter, MSGBOX_DEFAULT
	release
	end

JohtoIsle_Bugsy_EventScript_LeaderPostBattle::
	goto_if_set FLAG_E4_TIER3_CLEARED, JohtoIsle_Bugsy_EventScript_LeaderRematch100
	msgbox JohtoIsle_Bugsy_Text_LeaderPostBattle, MSGBOX_DEFAULT
	release
	end

@ Reforged: this isle's dock is a Seagallop port. The captain sails to
@ VERMILION, ONE-SEVEN ISLAND and every unlocked CROWN ISLE.
JohtoIsle_Bugsy_EventScript_Sailor::
	lock
	faceplayer
	setvar VAR_0x8004, SEAGALLOP_CROWN_CHITIN
	goto EventScript_CrownIsleCaptain
	end

JohtoIsle_Bugsy_EventScript_HoennWATTSON::
	lock
	faceplayer
	goto_if_set FLAG_BEAT_HOENN_WATTSON, JohtoIsle_Bugsy_EventScript_HoennWATTSONPost
	msgbox JohtoIsle_Bugsy_Text_HoennWATTSONPre, MSGBOX_DEFAULT
	trainerbattle_single TRAINER_HOENN_WATTSON, JohtoIsle_Bugsy_Text_HoennWATTSONIntro, JohtoIsle_Bugsy_Text_HoennWATTSONDefeat, JohtoIsle_Bugsy_EventScript_LeaderPostBattleDefeated
	release
	end

@ Continue script: a plain trainerbattle_single ends the script after
@ the battle, so the after-battle line has to live here.
JohtoIsle_Bugsy_EventScript_LeaderPostBattleDefeated::
	msgbox JohtoIsle_Bugsy_Text_HoennWATTSONAfter, MSGBOX_DEFAULT
	release
	end

JohtoIsle_Bugsy_EventScript_HoennWATTSONPost::
	goto_if_set FLAG_E4_TIER3_CLEARED, JohtoIsle_Bugsy_EventScript_HoennWATTSONRematch100
	msgbox JohtoIsle_Bugsy_Text_HoennWATTSONPostBattle, MSGBOX_DEFAULT
	release
	end

@ Reforged: after the final League, every Crown Isles leader rematches at Lv 100
@ (levels set in battle_main.c). Own continue script so the first-win side
@ effects (circuit progress) never re-run.
JohtoIsle_Bugsy_EventScript_LeaderRematch100::
	msgbox JohtoIsle_Bugsy_Text_CrownRematchAsk, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, JohtoIsle_Bugsy_EventScript_CrownRematchNo
	cleartrainerflag TRAINER_JOHTO_BUGSY
	trainerbattle_single TRAINER_JOHTO_BUGSY, JohtoIsle_Bugsy_Text_LeaderIntro, JohtoIsle_Bugsy_Text_LeaderDefeat, JohtoIsle_Bugsy_EventScript_CrownRematchDone
	release
	end

@ Reforged: after the final League, every Crown Isles leader rematches at Lv 100
@ (levels set in battle_main.c). Own continue script so the first-win side
@ effects (circuit progress) never re-run.
JohtoIsle_Bugsy_EventScript_HoennWATTSONRematch100::
	msgbox JohtoIsle_Bugsy_Text_CrownRematchAsk, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, JohtoIsle_Bugsy_EventScript_CrownRematchNo
	cleartrainerflag TRAINER_HOENN_WATTSON
	trainerbattle_single TRAINER_HOENN_WATTSON, JohtoIsle_Bugsy_Text_HoennWATTSONIntro, JohtoIsle_Bugsy_Text_HoennWATTSONDefeat, JohtoIsle_Bugsy_EventScript_CrownRematchDone
	release
	end

JohtoIsle_Bugsy_EventScript_CrownRematchNo::
	msgbox JohtoIsle_Bugsy_Text_CrownRematchNo
	release
	end

JohtoIsle_Bugsy_EventScript_CrownRematchDone::
	msgbox JohtoIsle_Bugsy_Text_CrownRematchDone
	release
	end
