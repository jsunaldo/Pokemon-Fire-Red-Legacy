CrownLeague_StevensRoom_MapScripts::
	map_script MAP_SCRIPT_ON_RESUME, CrownLeague_StevensRoom_OnResume
	map_script MAP_SCRIPT_ON_WARP_INTO_MAP_TABLE, CrownLeague_StevensRoom_OnWarp
	map_script MAP_SCRIPT_ON_FRAME_TABLE, CrownLeague_StevensRoom_OnFrame
	.byte 0

CrownLeague_StevensRoom_OnResume::
	setvar VAR_0x8004, 4
	call PokemonLeague_EventScript_DoLightingEffect
	end

CrownLeague_StevensRoom_OnWarp::
	map_script_2 VAR_TEMP_1, 0, CrownLeague_StevensRoom_EventScript_TurnPlayerNorth
	.2byte 0

CrownLeague_StevensRoom_EventScript_TurnPlayerNorth::
	turnobject LOCALID_PLAYER, DIR_NORTH
	end

CrownLeague_StevensRoom_OnFrame::
	map_script_2 VAR_MAP_SCENE_POKEMON_LEAGUE, 41, CrownLeague_StevensRoom_EventScript_EnterRoom
	.2byte 0

@ STEVEN: walk up to him, battle, then back outside to Wyrmspire.
CrownLeague_StevensRoom_EventScript_EnterRoom::
	lockall
	textcolor NPC_TEXT_COLOR_MALE
	setflag FLAG_TEMP_2
	setvar VAR_MAP_SCENE_POKEMON_LEAGUE, 50
	applymovement LOCALID_PLAYER, CrownLeague_StevensRoom_Movement_PlayerEnter
	waitmovement 0
	delay 20
	msgbox CrownLeague_StevensRoom_Text_Intro
	setflag FLAG_TEMP_3
	setflag FLAG_TEMP_5
	savebgm MUS_VICTORY_GYM_LEADER
	trainerbattle_no_intro TRAINER_CROWN_STEVEN, CrownLeague_StevensRoom_Text_Defeat
	clearflag FLAG_TEMP_5
	setvar VAR_MAP_SCENE_POKEMON_LEAGUE, 51
	setflag FLAG_CROWN_LEAGUE_CLEARED
	msgbox CrownLeague_StevensRoom_Text_PostBattle
	closemessage
	delay 20
	setvar VAR_MAP_SCENE_POKEMON_LEAGUE, 0
	setvar VAR_TEMP_1, 1
	warp MAP_JOHTO_ISLE_CLAIR, 0
	waitstate
	releaseall
	end

CrownLeague_StevensRoom_Movement_PlayerEnter::
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end
