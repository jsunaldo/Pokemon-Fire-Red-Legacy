HollowIsle_BellTower_Roof_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, HollowIsle_BellTower_Roof_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, HollowIsle_BellTower_Roof_OnResume
	.byte 0

HollowIsle_BellTower_Roof_OnTransition::
	call_if_set FLAG_FOUGHT_HO_OH, HollowIsle_BellTower_Roof_EventScript_HideHoOh
	call_if_unset FLAG_FOUGHT_HO_OH, HollowIsle_BellTower_Roof_EventScript_TryShowHoOh
	end

HollowIsle_BellTower_Roof_EventScript_HideHoOh::
	setvar VAR_TEMP_1, 1
	setflag FLAG_HIDE_HO_OH
	return

HollowIsle_BellTower_Roof_EventScript_TryShowHoOh::
	setvar VAR_TEMP_1, 1
	goto_if_set FLAG_HO_OH_FLEW_AWAY, EventScript_Return
	setvar VAR_TEMP_1, 0
	clearflag FLAG_HIDE_HO_OH
	return

HollowIsle_BellTower_Roof_OnResume::
	call_if_set FLAG_SYS_SPECIAL_WILD_BATTLE, HollowIsle_BellTower_Roof_EventScript_TryRemoveHoOh
	end

HollowIsle_BellTower_Roof_EventScript_TryRemoveHoOh::
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_ne VAR_RESULT, B_OUTCOME_CAUGHT, EventScript_Return
	removeobject LOCALID_BELL_TOWER_HO_OH
	return

HollowIsle_BellTower_Roof_EventScript_HoOh::
	goto_if_questlog EventScript_ReleaseEnd
	special QuestLog_CutRecording
	lockall
	setvar VAR_TEMP_1, 1
	special SpawnCameraObject
	setvar VAR_0x8004, 3  @ Num loops
	setvar VAR_0x8005, 35 @ Delay
	special LoopWingFlapSound
	applymovement LOCALID_BELL_TOWER_HO_OH, HollowIsle_BellTower_Roof_Movement_HoOhAppear
	applymovement LOCALID_CAMERA, HollowIsle_BellTower_Roof_Movement_CameraPanUp
	waitmovement 0
	delay 50
	setweather 0
	doweather
	waitse
	playmoncry SPECIES_HO_OH, CRY_MODE_ENCOUNTER
	delay 30
	waitmoncry
	delay 60
	setvar VAR_0x8004, 3  @ Num loops
	setvar VAR_0x8005, 35 @ Delay
	special LoopWingFlapSound
	applymovement LOCALID_CAMERA, HollowIsle_BellTower_Roof_Movement_CameraPanDown
	applymovement LOCALID_BELL_TOWER_HO_OH, HollowIsle_BellTower_Roof_Movement_HoOhApproach
	waitmovement 0
	special RemoveCameraObject
	seteventmon SPECIES_HO_OH, 70
	setflag FLAG_SYS_SPECIAL_WILD_BATTLE
	special StartLegendaryBattle
	waitstate
	clearflag FLAG_SYS_SPECIAL_WILD_BATTLE
	setvar VAR_LAST_TALKED, LOCALID_BELL_TOWER_HO_OH
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_WON, HollowIsle_BellTower_Roof_EventScript_DefeatedHoOh
	goto_if_eq VAR_RESULT, B_OUTCOME_RAN, HollowIsle_BellTower_Roof_EventScript_RanFromHoOh
	goto_if_eq VAR_RESULT, B_OUTCOME_PLAYER_TELEPORTED, HollowIsle_BellTower_Roof_EventScript_RanFromHoOh
	setflag FLAG_FOUGHT_HO_OH
	releaseall
	end

HollowIsle_BellTower_Roof_EventScript_DefeatedHoOh::
	setflag FLAG_HO_OH_FLEW_AWAY
	setvar VAR_0x8004, SPECIES_HO_OH
	goto EventScript_MonFlewAway
	end

HollowIsle_BellTower_Roof_EventScript_RanFromHoOh::
	setvar VAR_0x8004, SPECIES_HO_OH
	goto EventScript_MonFlewAway
	end

HollowIsle_BellTower_Roof_Movement_CameraPanUp:
	walk_up
	walk_up
	walk_up
	step_end

HollowIsle_BellTower_Roof_Movement_CameraPanDown:
	delay_16
	delay_16
	walk_down
	walk_down
	walk_down
	step_end

HollowIsle_BellTower_Roof_Movement_HoOhApproach:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_in_place_down
	delay_16
	delay_16
	step_end

HollowIsle_BellTower_Roof_Movement_HoOhAppear:
	delay_16
	delay_16
	walk_in_place_down
	walk_in_place_down
	walk_in_place_down
	walk_in_place_down
	step_end
