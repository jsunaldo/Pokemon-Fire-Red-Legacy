@ FRLG Legacy: when a Repel wears off, offer to use another of the same kind
EventScript_RepelWoreOff::
	lockall
	checkitem VAR_REPEL_LAST_USED, 1
	goto_if_eq VAR_RESULT, FALSE, EventScript_RepelWoreOff_NoMore
	msgbox Text_RepelWoreOff_UseAnother, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, EventScript_RepelWoreOff_End
	setvar VAR_REPEL_STEP_COUNT, 100
	compare VAR_REPEL_LAST_USED, ITEM_SUPER_REPEL
	call_if_eq EventScript_RepelWoreOff_Super
	compare VAR_REPEL_LAST_USED, ITEM_MAX_REPEL
	call_if_eq EventScript_RepelWoreOff_Max
	bufferitemname STR_VAR_1, VAR_REPEL_LAST_USED
	removeitem VAR_REPEL_LAST_USED, 1
	playse SE_REPEL
	msgbox Text_RepelWoreOff_UsedAnother, MSGBOX_SIGN
	goto EventScript_RepelWoreOff_End

EventScript_RepelWoreOff_Super::
	setvar VAR_REPEL_STEP_COUNT, 200
	return

EventScript_RepelWoreOff_Max::
	setvar VAR_REPEL_STEP_COUNT, 250
	return

EventScript_RepelWoreOff_NoMore::
	msgbox Text_RepelWoreOff, MSGBOX_SIGN
	goto EventScript_RepelWoreOff_End

EventScript_RepelWoreOff_End::
	releaseall
	end

Text_RepelWoreOff::
	.string "REPEL's effect wore off…$"

Text_RepelWoreOff_UseAnother::
	.string "REPEL's effect wore off…\n"
	.string "Use another?$"

Text_RepelWoreOff_UsedAnother::
	.string "{PLAYER} used the {STR_VAR_1}!$"
