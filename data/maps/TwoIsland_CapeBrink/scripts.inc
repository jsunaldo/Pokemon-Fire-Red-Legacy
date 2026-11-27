TwoIsland_CapeBrink_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, TwoIsland_CapeBrink_OnTransition
	.byte 0

TwoIsland_CapeBrink_OnTransition::
	call TwoIsland_CapeBrink_EventScript_HideLegend_SUICUNE
	end

TwoIsland_CapeBrink_EventScript_Legend_SUICUNE::
	goto_if_questlog EventScript_ReleaseEnd
	special QuestLog_CutRecording
	lock
	faceplayer
	goto_if_eq VAR_LEGEND_CAUGHT_02, 1, TwoIsland_CapeBrink_EventScript_LegendGone_SUICUNE
	waitse
	playmoncry SPECIES_SUICUNE, CRY_MODE_ENCOUNTER
	message TwoIsland_CapeBrink_Text_Legend_SUICUNE
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
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, TwoIsland_CapeBrink_EventScript_LegendDone_SUICUNE
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, TwoIsland_CapeBrink_EventScript_LegendDone_SUICUNE
	release
	end

TwoIsland_CapeBrink_EventScript_LegendDone_SUICUNE::
	setvar VAR_LEGEND_CAUGHT_02, 1
	removeobject LOCALID_LEGEND_SUICUNE
	release
	end

TwoIsland_CapeBrink_EventScript_LegendGone_SUICUNE::
	removeobject LOCALID_LEGEND_SUICUNE
	release
	end

TwoIsland_CapeBrink_EventScript_HideLegend_SUICUNE::
	goto_if_unset FLAG_SYS_GAME_CLEAR, TwoIsland_CapeBrink_EventScript_DoHideLegend_SUICUNE
	goto_if_eq VAR_LEGEND_CAUGHT_02, 1, TwoIsland_CapeBrink_EventScript_DoHideLegend_SUICUNE
	return

TwoIsland_CapeBrink_EventScript_DoHideLegend_SUICUNE::
	setflag FLAG_TEMP_1F
	return

TwoIsland_CapeBrink_Text_Legend_SUICUNE::
	.string "Suicune!$"
