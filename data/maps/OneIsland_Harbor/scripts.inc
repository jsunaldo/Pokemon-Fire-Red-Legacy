OneIsland_Harbor_MapScripts::
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, OneIsland_Harbor_OnWarp
	map_script MAP_SCRIPT_ON_FRAME_TABLE, OneIsland_Harbor_OnFrame
	.byte 0

OneIsland_Harbor_OnWarp::
	map_script_2 VAR_MAP_SCENE_ONE_ISLAND_HARBOR, 1, OneIsland_Harbor_EventScript_TurnPlayerNorth
	.2byte 0

OneIsland_Harbor_EventScript_TurnPlayerNorth::
	turnobject LOCALID_PLAYER, DIR_NORTH
	end

OneIsland_Harbor_OnFrame::
	map_script_2 VAR_MAP_SCENE_ONE_ISLAND_HARBOR, 1, OneIsland_Harbor_EventScript_PlayerEnterHarborFirstTime
	.2byte 0

OneIsland_Harbor_EventScript_PlayerEnterHarborFirstTime::
	lockall
	textcolor NPC_TEXT_COLOR_MALE
	applymovement LOCALID_PLAYER, OneIsland_Harbor_Movement_PlayerExitHarbor
	waitmovement 0
	setvar VAR_MAP_SCENE_ONE_ISLAND_HARBOR, 2
	warp MAP_ONE_ISLAND, 12, 18
	waitstate
	releaseall
	end

OneIsland_Harbor_Movement_PlayerExitHarbor::
	walk_up
	walk_up
	walk_up
	step_end

OneIsland_Harbor_EventScript_Sailor::
	lock
	faceplayer
	message Text_WhereDoYouWantToSail
	waitmessage
	setvar VAR_0x8004, SEAGALLOP_ONE_ISLAND
	goto EventScript_ChooseDestFromOneIsland
	end

OneIsland_Harbor_EventScript_JohtoFerry::
	lock
	faceplayer
	goto_if_unset FLAG_E4_REMATCH_CLEARED, OneIsland_Harbor_EventScript_JohtoFerryClosed
	msgbox OneIsland_Harbor_Text_JohtoFerryAsk, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, OneIsland_Harbor_EventScript_JohtoFerryBye
	msgbox OneIsland_Harbor_Text_JohtoFerryGo, MSGBOX_DEFAULT
	warp MAP_JOHTO_ISLE_FALKNER, 9, 34
	waitstate
	release
	end

OneIsland_Harbor_EventScript_JohtoFerryClosed::
	msgbox OneIsland_Harbor_Text_JohtoFerryClosed, MSGBOX_DEFAULT
	release
	end

OneIsland_Harbor_EventScript_JohtoFerryBye::
	release
	end

OneIsland_Harbor_Text_JohtoFerryAsk::
	.string "Ahoy, CHAMPION! Word is you've\n"
	.string "bested the ELITE FOUR anew.\p"
	.string "Out west lie the JOHTO ISLES,\n"
	.string "eight GYM LEADERS, eight trials.\p"
	.string "Set sail for SKYREACH ISLE?$"

OneIsland_Harbor_Text_JohtoFerryGo::
	.string "Then hold fast, we sail!$"

OneIsland_Harbor_Text_JohtoFerryClosed::
	.string "The JOHTO ISLES open only to a\n"
	.string "true CHAMPION.\p"
	.string "Come back once you've bested the\n"
	.string "ELITE FOUR a second time.$"
