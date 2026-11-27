JohtoIsle_Whitney_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, JohtoIsle_Whitney_OnTransition
	.byte 0

JohtoIsle_Whitney_OnTransition::
	setworldmapflag FLAG_WORLD_MAP_CROWN_MEADOWMOO
	end

JohtoIsle_Whitney_EventScript_Leader::
	lock
	faceplayer
	goto_if_set FLAG_BEAT_JOHTO_WHITNEY, JohtoIsle_Whitney_EventScript_LeaderPostBattle
	msgbox JohtoIsle_Whitney_Text_LeaderPreBattle, MSGBOX_DEFAULT
	trainerbattle_single TRAINER_JOHTO_WHITNEY, JohtoIsle_Whitney_Text_LeaderIntro, JohtoIsle_Whitney_Text_LeaderDefeat, JohtoIsle_Whitney_EventScript_LeaderDefeated
	release
	end

@ Continue script: a plain trainerbattle_single ends the script after
@ the battle, so the after-battle line has to live here.
JohtoIsle_Whitney_EventScript_LeaderDefeated::
	msgbox JohtoIsle_Whitney_Text_LeaderAfter, MSGBOX_DEFAULT
	release
	end

JohtoIsle_Whitney_EventScript_LeaderPostBattle::
	goto_if_set FLAG_E4_TIER3_CLEARED, JohtoIsle_Whitney_EventScript_LeaderRematch100
	msgbox JohtoIsle_Whitney_Text_LeaderPostBattle, MSGBOX_DEFAULT
	release
	end

@ Reforged: this isle's dock is a Seagallop port. The captain sails to
@ VERMILION, ONE-SEVEN ISLAND and every unlocked CROWN ISLE.
JohtoIsle_Whitney_EventScript_Sailor::
	lock
	faceplayer
	setvar VAR_0x8004, SEAGALLOP_CROWN_MEADOWMOO
	goto EventScript_CrownIsleCaptain
	end

JohtoIsle_Whitney_EventScript_HoennNORMAN::
	lock
	faceplayer
	goto_if_set FLAG_BEAT_HOENN_NORMAN, JohtoIsle_Whitney_EventScript_HoennNORMANPost
	msgbox JohtoIsle_Whitney_Text_HoennNORMANPre, MSGBOX_DEFAULT
	trainerbattle_single TRAINER_HOENN_NORMAN, JohtoIsle_Whitney_Text_HoennNORMANIntro, JohtoIsle_Whitney_Text_HoennNORMANDefeat, JohtoIsle_Whitney_EventScript_LeaderPostBattleDefeated
	release
	end

@ Continue script: a plain trainerbattle_single ends the script after
@ the battle, so the after-battle line has to live here.
JohtoIsle_Whitney_EventScript_LeaderPostBattleDefeated::
	msgbox JohtoIsle_Whitney_Text_HoennNORMANAfter, MSGBOX_DEFAULT
	release
	end

JohtoIsle_Whitney_EventScript_HoennNORMANPost::
	goto_if_set FLAG_E4_TIER3_CLEARED, JohtoIsle_Whitney_EventScript_HoennNORMANRematch100
	msgbox JohtoIsle_Whitney_Text_HoennNORMANPostBattle, MSGBOX_DEFAULT
	release
	end

@ Reforged: after the final League, every Crown Isles leader rematches at Lv 100
@ (levels set in battle_main.c). Own continue script so the first-win side
@ effects (circuit progress) never re-run.
JohtoIsle_Whitney_EventScript_LeaderRematch100::
	msgbox JohtoIsle_Whitney_Text_CrownRematchAsk, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, JohtoIsle_Whitney_EventScript_CrownRematchNo
	cleartrainerflag TRAINER_JOHTO_WHITNEY
	trainerbattle_single TRAINER_JOHTO_WHITNEY, JohtoIsle_Whitney_Text_LeaderIntro, JohtoIsle_Whitney_Text_LeaderDefeat, JohtoIsle_Whitney_EventScript_CrownRematchDone
	release
	end

@ Reforged: after the final League, every Crown Isles leader rematches at Lv 100
@ (levels set in battle_main.c). Own continue script so the first-win side
@ effects (circuit progress) never re-run.
JohtoIsle_Whitney_EventScript_HoennNORMANRematch100::
	msgbox JohtoIsle_Whitney_Text_CrownRematchAsk, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, JohtoIsle_Whitney_EventScript_CrownRematchNo
	cleartrainerflag TRAINER_HOENN_NORMAN
	trainerbattle_single TRAINER_HOENN_NORMAN, JohtoIsle_Whitney_Text_HoennNORMANIntro, JohtoIsle_Whitney_Text_HoennNORMANDefeat, JohtoIsle_Whitney_EventScript_CrownRematchDone
	release
	end

JohtoIsle_Whitney_EventScript_CrownRematchNo::
	msgbox JohtoIsle_Whitney_Text_CrownRematchNo
	release
	end

JohtoIsle_Whitney_EventScript_CrownRematchDone::
	msgbox JohtoIsle_Whitney_Text_CrownRematchDone
	release
	end
