// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Engine/TargetPoint.h"
#include "FPSGameMode.h"
#include "FPSAIGuardController.h"

#pragma optimize ("", off)


// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnNoiseHeard);

	GuardState = EAIState::Idle;
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();
	
	OriginalRotation = GetActorRotation();

	GuardController = Cast<AFPSAIGuardController>(GetController());

	if (bPatrol && GuardController != nullptr && NavTargets.Num() > 0)
	{
		GuardController->SetWaypoints(NavTargets, PatrolWaitTime);
		if (GuardController->GoToNextWaypoint())
		{
			SetGuardState(EAIState::Patrolling);
		}
	}
}

void AFPSAIGuard::OnPawnSeen(APawn* SeenPawn)
{
	if (SeenPawn == nullptr)
		return;

	DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.0f, 12, FColor::Red, false, 10.0f);

	AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());

	if (GM)
	{
		GM->CompleteMission(SeenPawn, false);
	}

	if (GuardState == EAIState::Patrolling && GuardController != nullptr)
	{
		GuardController->Wait();
	}

	SetGuardState(EAIState::Alerted);
}

void AFPSAIGuard::OnNoiseHeard(APawn* HeardPawn, const FVector& Location, float Volume)
{
	if (GuardState == EAIState::Alerted)
		return;

	if (GuardState == EAIState::Patrolling && GuardController != nullptr)
	{
		GuardController->Wait();
	}

	DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Green, false, 10.0f);

	FVector Direction = Location - GetActorLocation();
	Direction.Normalize();

	FRotator newLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
	newLookAt.Pitch = 0.0f;
	newLookAt.Roll = 0.0f;

	SetActorRotation(newLookAt);

	GetWorldTimerManager().ClearTimer(TimerHandle_ResetOrientation);
	GetWorldTimerManager().SetTimer(TimerHandle_ResetOrientation, this, &AFPSAIGuard::ResetOrientation, 3.0f);

	SetGuardState(EAIState::Suspcious);
}

void AFPSAIGuard::ResetOrientation()
{
	if (GuardState == EAIState::Alerted)
		return;

	SetActorRotation(OriginalRotation);

	if (bPatrol && NavTargets.Num() > 0 && GuardController != nullptr)
	{
		GuardController->ContinuePatrol();
		SetGuardState(EAIState::Patrolling);
	}
	else
	{
		SetGuardState(EAIState::Idle);
	}
}

void AFPSAIGuard::SetGuardState(EAIState newState)
{
	if (GuardState == newState)
		return;

	UE_LOG(LogTemp, Log, TEXT("Setting guard state from %d to %d"), (int32)GuardState, (int32)newState);

	GuardState = newState;

	OnStateChanged(GuardState);
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

