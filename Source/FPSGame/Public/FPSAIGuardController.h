// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FPSAIGuardController.generated.h"

/**
 * 
 */
UCLASS()
class FPSGAME_API AFPSAIGuardController : public AAIController
{
	GENERATED_BODY()
	
public:
	AFPSAIGuardController();

	void SetWaypoints(TArray<AActor*>& _Waypoints, float WaitTime = 0.0f);

	bool GoToNextWaypoint();
	void Wait();
	void ContinuePatrol();

	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
	
protected:
	TArray<AActor*>		Waypoints;

	int32				NextWaypointIndex;

	float				WaypointWaitTime;
	FTimerHandle		TimerHandle_WaitAtWaypoint;

private:
	void OnWaypointWaitFinished();
};
