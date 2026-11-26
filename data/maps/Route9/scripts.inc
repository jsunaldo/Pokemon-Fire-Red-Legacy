Route9_MapScripts::
	.byte 0

@ FRLG Legacy: the road to Rock Tunnel needs 3 badges. With only Brock's and
@ Misty's, the one gym you can reach is Lt. Surge's, so he is always badge 3.
Route9_EventScript_GuardTrigger::
	lockall
	special GetPlayerGymBadgeCount
	goto_if_ge VAR_RESULT, 3, Route9_EventScript_GuardRelease
	applymovement LOCALID_ROUTE9_GUARD, Common_Movement_FacePlayer
	waitmovement 0
	msgbox Route9_Text_GuardStop
	closemessage
	applymovement LOCALID_PLAYER, Route9_Movement_PushBack
	waitmovement 0
	applymovement LOCALID_ROUTE9_GUARD, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
Route9_EventScript_GuardRelease::
	releaseall
	end

Route9_EventScript_Guard::
	lock
	faceplayer
	special GetPlayerGymBadgeCount
	goto_if_ge VAR_RESULT, 3, Route9_EventScript_GuardPass
	msgbox Route9_Text_GuardStop
	release
	end

Route9_EventScript_GuardPass::
	msgbox Route9_Text_GuardPass
	release
	end

Route9_Movement_PushBack::
	walk_left
	step_end

Route9_EventScript_RouteSign::
	msgbox Route9_Text_RouteSign, MSGBOX_SIGN
	end
