ChitinIsle_Woods_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, ChitinIsle_Woods_OnTransition
	.byte 0

ChitinIsle_Woods_OnTransition::
	call ChitinIsle_Woods_EventScript_HideLegend_CELEBI
	end

ChitinIsle_Woods_EventScript_ShrineSign::
	msgbox ChitinIsle_Woods_Text_ShrineSign, MSGBOX_SIGN
	end

ChitinIsle_Woods_EventScript_Legend_CELEBI::
	goto_if_questlog EventScript_ReleaseEnd
	special QuestLog_CutRecording
	lock
	faceplayer
	goto_if_eq VAR_LEGEND_CAUGHT_03, 1, ChitinIsle_Woods_EventScript_LegendGone_CELEBI
	waitse
	playmoncry SPECIES_CELEBI, CRY_MODE_ENCOUNTER
	message ChitinIsle_Woods_Text_Legend_CELEBI
	waitmessage
	waitmoncry
	delay 20
	playbgm MUS_ENCOUNTER_GYM_LEADER, 0
	waitbuttonpress
	setwildbattle SPECIES_CELEBI, 50
	setflag FLAG_SYS_SPECIAL_WILD_BATTLE
	special StartLegendaryBattle
	waitstate
	clearflag FLAG_SYS_SPECIAL_WILD_BATTLE
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, ChitinIsle_Woods_EventScript_LegendDone_CELEBI
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, ChitinIsle_Woods_EventScript_LegendDone_CELEBI
	release
	end

ChitinIsle_Woods_EventScript_LegendDone_CELEBI::
	setvar VAR_LEGEND_CAUGHT_03, 1
	removeobject LOCALID_LEGEND_CELEBI
	release
	end

ChitinIsle_Woods_EventScript_LegendGone_CELEBI::
	removeobject LOCALID_LEGEND_CELEBI
	release
	end

ChitinIsle_Woods_EventScript_HideLegend_CELEBI::
	goto_if_eq VAR_LEGEND_CAUGHT_03, 1, ChitinIsle_Woods_EventScript_DoHideLegend_CELEBI
	return

ChitinIsle_Woods_EventScript_DoHideLegend_CELEBI::
	setflag FLAG_TEMP_1F
	return

ChitinIsle_Woods_EventScript_Rick::
	trainerbattle_single TRAINER_CROWN_BUG_CATCHER_RICK, ChitinIsle_Woods_Text_RickIntro, ChitinIsle_Woods_Text_RickDefeat
	msgbox ChitinIsle_Woods_Text_RickPostBattle, MSGBOX_AUTOCLOSE
	end

