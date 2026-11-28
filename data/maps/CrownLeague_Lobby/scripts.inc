CrownLeague_Lobby_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, CrownLeague_Lobby_OnTransition
	.byte 0

@ Walking in resets the run: the League can be challenged again and again.
@ Until the Crown Isles circuit is cleared the guard stands in the doorway.
CrownLeague_Lobby_OnTransition::
	setvar VAR_MAP_SCENE_POKEMON_LEAGUE, 0
	call_if_unset FLAG_JOHTO_CIRCUIT_CLEARED, CrownLeague_Lobby_EventScript_BlockDoor
	end

CrownLeague_Lobby_EventScript_BlockDoor::
	setobjectxyperm LOCALID_CROWN_LOBBY_GUARD, 4, 2
	return

CrownLeague_Lobby_EventScript_Guard::
	lock
	faceplayer
	goto_if_unset FLAG_JOHTO_CIRCUIT_CLEARED, CrownLeague_Lobby_EventScript_GuardClosed
	msgbox CrownLeague_Lobby_Text_GuardOpen
	release
	end

CrownLeague_Lobby_EventScript_GuardClosed::
	msgbox CrownLeague_Lobby_Text_GuardClosed
	release
	end

CrownLeague_Lobby_EventScript_Guide::
	msgbox CrownLeague_Lobby_Text_Guide, MSGBOX_NPC
	end

CrownLeague_Lobby_EventScript_Nurse::
	lock
	faceplayer
	call EventScript_PkmnCenterNurse
	release
	end

CrownLeague_Lobby_EventScript_Clerk::
	goto_if_questlog EventScript_ReleaseEnd
	lock
	faceplayer
	message Text_MayIHelpYou
	waitmessage
	pokemart IndigoPlateau_PokemonCenter_1F_Items
	msgbox Text_PleaseComeAgain
	release
	end
