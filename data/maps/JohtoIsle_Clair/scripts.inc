JohtoIsle_Clair_MapScripts::
	.byte 0

JohtoIsle_Clair_EventScript_Leader::
	lock
	faceplayer
	goto_if_set FLAG_BEAT_JOHTO_CLAIR, JohtoIsle_Clair_EventScript_LeaderPostBattle
	msgbox JohtoIsle_Clair_Text_LeaderPreBattle, MSGBOX_DEFAULT
	trainerbattle_single TRAINER_JOHTO_CLAIR, JohtoIsle_Clair_Text_LeaderIntro, JohtoIsle_Clair_Text_LeaderDefeat, JohtoIsle_Clair_EventScript_LeaderDefeated
	release
	end

@ FRLG Legacy v2.3: a plain trainerbattle_single ends the script after the
@ battle, so the circuit check must live in the continue script.
JohtoIsle_Clair_EventScript_LeaderDefeated::
	msgbox JohtoIsle_Clair_Text_LeaderAfter, MSGBOX_DEFAULT
	call JohtoIsle_Clair_EventScript_TryClearCircuit
	release
	end

JohtoIsle_Clair_EventScript_LeaderPostBattle::
	call JohtoIsle_Clair_EventScript_TryClearCircuit
	msgbox JohtoIsle_Clair_Text_LeaderPostBattle, MSGBOX_DEFAULT
	release
	end

JohtoIsle_Clair_EventScript_Sailor::
	lock
	faceplayer
	goto_if_unset FLAG_BEAT_JOHTO_CLAIR, JohtoIsle_Clair_EventScript_SailorBlocked
	goto_if_unset FLAG_BEAT_HOENN_FLANNERY, JohtoIsle_Clair_EventScript_SailorBlocked
	goto JohtoIsle_Clair_EventScript_SailorOnward

JohtoIsle_Clair_EventScript_SailorBlocked::
	msgbox JohtoIsle_Clair_Text_SailorBlocked, MSGBOX_DEFAULT
	goto JohtoIsle_Clair_EventScript_SailorReturnPrompt

JohtoIsle_Clair_EventScript_SailorOnward::
	goto JohtoIsle_Clair_EventScript_SailorReturnPrompt

JohtoIsle_Clair_EventScript_SailorReturnPrompt::
	msgbox JohtoIsle_Clair_Text_SailorReturn, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, JohtoIsle_Clair_EventScript_SailorBye
	warp MAP_ONE_ISLAND_HARBOR, 8, 5
	waitstate
	release
	end

JohtoIsle_Clair_EventScript_SailorBye::
	release
	end

JohtoIsle_Clair_EventScript_HoennFLANNERY::
	lock
	faceplayer
	goto_if_set FLAG_BEAT_HOENN_FLANNERY, JohtoIsle_Clair_EventScript_HoennFLANNERYPost
	msgbox JohtoIsle_Clair_Text_HoennFLANNERYPre, MSGBOX_DEFAULT
	trainerbattle_single TRAINER_HOENN_FLANNERY, JohtoIsle_Clair_Text_HoennFLANNERYIntro, JohtoIsle_Clair_Text_HoennFLANNERYDefeat, JohtoIsle_Clair_EventScript_HoennFLANNERYDefeated
	release
	end

JohtoIsle_Clair_EventScript_HoennFLANNERYDefeated::
	msgbox JohtoIsle_Clair_Text_HoennFLANNERYAfter, MSGBOX_DEFAULT
	call JohtoIsle_Clair_EventScript_TryClearCircuit
	release
	end

JohtoIsle_Clair_EventScript_HoennFLANNERYPost::
	call JohtoIsle_Clair_EventScript_TryClearCircuit
	msgbox JohtoIsle_Clair_Text_HoennFLANNERYPostBattle, MSGBOX_DEFAULT
	release
	end

@ Clears the circuit once both Wyrmspire leaders are beaten. Also runs on
@ re-talk, which repairs v2.2 saves that beat both but never got the flag.
@ Guarded so it can never reset Tier 3 progress a second time.
JohtoIsle_Clair_EventScript_TryClearCircuit::
	goto_if_set FLAG_JOHTO_CIRCUIT_CLEARED, JohtoIsle_Clair_EventScript_TryClearCircuitDone
	goto_if_unset FLAG_BEAT_JOHTO_CLAIR, JohtoIsle_Clair_EventScript_TryClearCircuitDone
	goto_if_unset FLAG_BEAT_HOENN_FLANNERY, JohtoIsle_Clair_EventScript_TryClearCircuitDone
	call JohtoIsle_Clair_EventScript_SetCircuitCleared
JohtoIsle_Clair_EventScript_TryClearCircuitDone::
	return

JohtoIsle_Clair_EventScript_SetCircuitCleared::
	setflag FLAG_JOHTO_CIRCUIT_CLEARED
	msgbox JohtoIsle_Clair_Text_CircuitCleared, MSGBOX_DEFAULT
@ FRLG Legacy: reset the Kanto rematch tracker so the Tier 3 endgame must
@ re-beat all eight leaders (now at Tier 3 levels) before the Tier 3 E4 opens.
	clearflag FLAG_KANTO_REMATCH_BROCK
	clearflag FLAG_KANTO_REMATCH_MISTY
	clearflag FLAG_KANTO_REMATCH_LT_SURGE
	clearflag FLAG_KANTO_REMATCH_ERIKA
	clearflag FLAG_KANTO_REMATCH_KOGA
	clearflag FLAG_KANTO_REMATCH_SABRINA
	clearflag FLAG_KANTO_REMATCH_BLAINE
	clearflag FLAG_KANTO_REMATCH_GIOVANNI
	return
