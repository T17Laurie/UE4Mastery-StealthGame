// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSAIGuardController.h"
#include "Engine/World.h"

#pragma optimize ("", off)

AFPSAIGuardController::AFPSAIGuardController()
{
	NextWaypointIndex = 0;
	WaypointWaitTime = 0.0f;
}

void AFPSAIGuardController::SetWaypoints(TArray<AActor*>& _Waypoints, float _WaitTime)
{
	Waypoints = _Waypoints;
	WaypointWaitTime = _WaitTime;
}

bool AFPSAIGuardController::GoToNextWaypoint()
{
	if (Waypoints.Num() == 0)
		return false;

	if (NextWaypointIndex < Waypoints.Num() && Waypoints[NextWaypointIndex] != nullptr)
	{
		MoveToActor(Waypoints[NextWaypointIndex]);
		return true;
	}

	// Tutorial Solution: Instead of having an overridden AIController that calls MoveToActor(), the tutorial solution was to
	//                    call UNavigationSystem::SimpleMoveToActor(GetController(), TargetPoint) directly from FPSAIGuard.
	//					  To stop navigating when the pawn sees/hears something, they call GetController()->StopMovement().
	//					  To detect the pawn arriving at the waypoint they use a distance check in Tick().

	return false;
}

void AFPSAIGuardController::Wait()
{
	if (GetCurrentMoveRequestID() != FAIRequestID::InvalidRequest)
	{
		StopMovement();		
	}

	if (GetWorldTimerManager().IsTimerActive(TimerHandle_WaitAtWaypoint))
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_WaitAtWaypoint);
	}
}

void AFPSAIGuardController::ContinuePatrol()
{
	GoToNextWaypoint();
}

void AFPSAIGuardController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	if (Result.IsSuccess())
	{
		++NextWaypointIndex;

		if (NextWaypointIndex == Waypoints.Num())
		{
			NextWaypointIndex = 0;
		}
	}

	if (WaypointWaitTime > 0.0f)
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_WaitAtWaypoint);
		GetWorldTimerManager().SetTimer(TimerHandle_WaitAtWaypoint, this, &AFPSAIGuardController::OnWaypointWaitFinished, WaypointWaitTime);
	}
	else
	{
		OnWaypointWaitFinished();
	}
}

void AFPSAIGuardController::OnWaypointWaitFinished()
{
	GoToNextWaypoint();
}
