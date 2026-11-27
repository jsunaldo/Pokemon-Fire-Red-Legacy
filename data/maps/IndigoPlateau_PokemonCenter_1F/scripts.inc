IndigoPlateau_PokemonCenter_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, IndigoPlateau_PokemonCenter_1F_OnTransition
	map_script MAP_SCRIPT_ON_RESUME, CableClub_OnResume
	.byte 0

IndigoPlateau_PokemonCenter_1F_OnTransition::
	setrespawn HEAL_LOCATION_INDIGO_PLATEAU
	specialvar VAR_RESULT, IsNationalDexPostgame
	call_if_eq VAR_RESULT, TRUE, IndigoPlateau_PokemonCenter_1F_EventScript_CheckBlockDoor
	end

IndigoPlateau_PokemonCenter_1F_EventScript_CheckBlockDoor::
	goto_if_set FLAG_SYS_CAN_LINK_WITH_RS, EventScript_Return
	setobjectxyperm LOCALID_LEAGUE_DOOR_GUARD, 4, 2
	return

IndigoPlateau_PokemonCenter_1F_EventScript_DoorGuard::
	lock
	faceplayer
	specialvar VAR_RESULT, IsNationalDexPostgame
	goto_if_eq VAR_RESULT, TRUE, IndigoPlateau_PokemonCenter_1F_EventScript_CheckSeviiIslandComplete
	goto IndigoPlateau_PokemonCenter_1F_EventScript_SeviiIslandComplete
	end

IndigoPlateau_PokemonCenter_1F_EventScript_CheckSeviiIslandComplete::
	goto_if_set FLAG_SYS_CAN_LINK_WITH_RS, IndigoPlateau_PokemonCenter_1F_EventScript_SeviiIslandComplete
	msgbox IndigoPlateau_PokemonCenter_1F_Text_LoreleiIsAbsentClosedForTimeBeing
	release
	end

@ Reforged: after you're CHAMPION, the ELITE FOUR rematch only opens once all
@ eight KANTO GYM LEADERS are re-beaten (Lorelei's RematchGate). Say so here,
@ with a count, so nobody walks to Lorelei's room to find out.
IndigoPlateau_PokemonCenter_1F_EventScript_SeviiIslandComplete::
	goto_if_unset FLAG_SYS_GAME_CLEAR, IndigoPlateau_PokemonCenter_1F_EventScript_GoodLuck
	setvar VAR_0x8004, 0
	call_if_set FLAG_KANTO_REMATCH_BROCK, IndigoPlateau_PokemonCenter_1F_EventScript_AddGym
	call_if_set FLAG_KANTO_REMATCH_MISTY, IndigoPlateau_PokemonCenter_1F_EventScript_AddGym
	call_if_set FLAG_KANTO_REMATCH_LT_SURGE, IndigoPlateau_PokemonCenter_1F_EventScript_AddGym
	call_if_set FLAG_KANTO_REMATCH_ERIKA, IndigoPlateau_PokemonCenter_1F_EventScript_AddGym
	call_if_set FLAG_KANTO_REMATCH_KOGA, IndigoPlateau_PokemonCenter_1F_EventScript_AddGym
	call_if_set FLAG_KANTO_REMATCH_SABRINA, IndigoPlateau_PokemonCenter_1F_EventScript_AddGym
	call_if_set FLAG_KANTO_REMATCH_BLAINE, IndigoPlateau_PokemonCenter_1F_EventScript_AddGym
	call_if_set FLAG_KANTO_REMATCH_GIOVANNI, IndigoPlateau_PokemonCenter_1F_EventScript_AddGym
	goto_if_eq VAR_0x8004, 8, IndigoPlateau_PokemonCenter_1F_EventScript_GoodLuck
	buffernumberstring STR_VAR_1, VAR_0x8004
	msgbox IndigoPlateau_PokemonCenter_1F_Text_RematchGymsFirst
	release
	end

IndigoPlateau_PokemonCenter_1F_EventScript_AddGym::
	addvar VAR_0x8004, 1
	return

IndigoPlateau_PokemonCenter_1F_EventScript_GoodLuck::
	msgbox IndigoPlateau_PokemonCenter_1F_Text_FaceEliteFourGoodLuck
	release
	end

IndigoPlateau_PokemonCenter_1F_EventScript_GymGuy::
	msgbox IndigoPlateau_PokemonCenter_1F_Text_GymGuyAdvice, MSGBOX_NPC
	end

IndigoPlateau_PokemonCenter_1F_EventScript_Clerk::
	goto_if_questlog EventScript_ReleaseEnd
	lock
	faceplayer
	message Text_MayIHelpYou
	waitmessage
	pokemart IndigoPlateau_PokemonCenter_1F_Items
	msgbox Text_PleaseComeAgain
	release
	end

	.align 2
IndigoPlateau_PokemonCenter_1F_Items::
	.2byte ITEM_ULTRA_BALL
	.2byte ITEM_HYPER_POTION
	.2byte ITEM_FULL_RESTORE
	.2byte ITEM_REVIVE
	.2byte ITEM_FULL_HEAL
	.2byte ITEM_ELIXIR
	.2byte ITEM_TM13_ICE_BEAM
	.2byte ITEM_TM24_THUNDERBOLT
	.2byte ITEM_TM35_FLAMETHROWER
	.2byte ITEM_TM19_GIGA_DRAIN
	.2byte ITEM_TM22_SOLAR_BEAM
	.2byte ITEM_TM31_BRICK_BREAK
	.2byte ITEM_TM39_ROCK_TOMB
	.2byte ITEM_NONE
	release
	end

IndigoPlateau_PokemonCenter_1F_EventScript_Nurse::
	lock
	faceplayer
	call EventScript_PkmnCenterNurse
	release
	end
