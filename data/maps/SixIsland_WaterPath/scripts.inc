SixIsland_WaterPath_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, SixIsland_WaterPath_OnTransition
	.byte 0

SixIsland_WaterPath_EventScript_HornWantedSign::
	msgbox SixIsland_WaterPath_Text_WantedUltimateHorn, MSGBOX_SIGN
	end

SixIsland_WaterPath_EventScript_RouteSign::
	msgbox SixIsland_WaterPath_Text_RouteSign, MSGBOX_SIGN
	end

SixIsland_WaterPath_OnTransition::
	call SixIsland_WaterPath_EventScript_HideLegend_SUICUNE
	end

SixIsland_WaterPath_EventScript_Legend_SUICUNE::
	goto_if_questlog EventScript_ReleaseEnd
	special QuestLog_CutRecording
	lock
	faceplayer
	goto_if_eq VAR_LEGEND_CAUGHT_02, 1, SixIsland_WaterPath_EventScript_LegendGone_SUICUNE
	waitse
	playmoncry SPECIES_SUICUNE, CRY_MODE_ENCOUNTER
	message SixIsland_WaterPath_Text_Legend_SUICUNE
	waitmessage
	waitmoncry
	delay 20
	playbgm MUS_ENCOUNTER_GYM_LEADER, 0
	waitbuttonpress
	setwildbattle SPECIES_SUICUNE, 50
	setflag FLAG_SYS_SPECIAL_WILD_BATTLE
	special StartLegendaryBattle
	waitstate
	clearflag FLAG_SYS_SPECIAL_WILD_BATTLE
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, SixIsland_WaterPath_EventScript_LegendDone_SUICUNE
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, SixIsland_WaterPath_EventScript_LegendDone_SUICUNE
	release
	end

SixIsland_WaterPath_EventScript_LegendDone_SUICUNE::
	setvar VAR_LEGEND_CAUGHT_02, 1
	removeobject LOCALID_LEGEND_SUICUNE
	release
	end

SixIsland_WaterPath_EventScript_LegendGone_SUICUNE::
	removeobject LOCALID_LEGEND_SUICUNE
	release
	end

SixIsland_WaterPath_EventScript_HideLegend_SUICUNE::
	goto_if_eq VAR_LEGEND_CAUGHT_02, 1, SixIsland_WaterPath_EventScript_DoHideLegend_SUICUNE
	return

SixIsland_WaterPath_EventScript_DoHideLegend_SUICUNE::
	removeobject LOCALID_LEGEND_SUICUNE
	return
