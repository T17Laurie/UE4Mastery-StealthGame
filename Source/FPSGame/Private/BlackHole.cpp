// Fill out your copyright notice in the Description page of Project Settings.

#include "BlackHole.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Test change!

// Sets default values
ABlackHole::ABlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GravityStrengthMax = 200;
	GravityStrengthMin = 50;

	BlackHoleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Black hole mesh"));
	BlackHoleMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = Cast<USceneComponent>(BlackHoleMesh);

	GravitySphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Black hole gravity collsion"));
	GravitySphereComp->SetupAttachment(RootComponent);
	GravitySphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	DestructionSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Black hole destruction collsion"));
	DestructionSphereComp->SetupAttachment(RootComponent);
	DestructionSphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	// Tutorial solution
	DestructionSphereComp->OnComponentBeginOverlap.AddDynamic(this, &ABlackHole::OverlapInnerSphere);
}

// Called when the game starts or when spawned
void ABlackHole::BeginPlay()
{
	Super::BeginPlay();
	
}
// Called every frame
void ABlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TutorialSolution();
}

void ABlackHole::MySolution()
{
	TArray<AActor*> affectedActors;

	// Pull in nearby actors
	GravitySphereComp->GetOverlappingActors(affectedActors);

	for (int32 i = 0; i < affectedActors.Num(); ++i)
	{
		if (affectedActors[i]->ActorHasTag(TEXT("AffectedByBlackHole")))
		{
			UStaticMeshComponent* meshComp = Cast< UStaticMeshComponent>(affectedActors[i]->GetComponentByClass(UStaticMeshComponent::StaticClass()));

			if (meshComp != nullptr)
			{
				FVector toBlackHole = GetActorLocation() - meshComp->GetOwner()->GetActorLocation();
				float dist = FMath::Max(toBlackHole.Size(), 0.1f);
				float forceScale = 1.0f - dist / GravitySphereComp->GetScaledSphereRadius();

				FVector force = toBlackHole / dist;
				force *= GravityStrengthMin + ((GravityStrengthMax - GravityStrengthMin) * forceScale);

				meshComp->SetEnableGravity(!DisableGravityOnAffectedActors);
				meshComp->AddImpulse(force);
				//meshComp->AddForce(force);
			}
		}
	}

	// Destroy actors that reach the centre

	affectedActors.Empty();
	DestructionSphereComp->GetOverlappingActors(affectedActors);

	for (int32 i = affectedActors.Num() - 1; i >= 0; --i)
	{
		if (affectedActors[i]->ActorHasTag(TEXT("AffectedByBlackHole")))
		{
			UStaticMeshComponent* meshComp = Cast< UStaticMeshComponent>(affectedActors[i]->GetComponentByClass(UStaticMeshComponent::StaticClass()));

			if (meshComp != nullptr && meshComp != BlackHoleMesh)
			{
				meshComp->GetOwner()->Destroy();
			}
		}
	}
}

void ABlackHole::TutorialSolution()
{
	TArray<UPrimitiveComponent*> OverlappingComps;

	GravitySphereComp->GetOverlappingComponents(OverlappingComps);

	for (int32 i = 0; i < OverlappingComps.Num(); ++i)
	{
		UPrimitiveComponent* PrimComp = OverlappingComps[i];

		if (PrimComp && PrimComp->IsSimulatingPhysics())
		{
			const float sphereRadius = GravitySphereComp->GetScaledSphereRadius();
			const float forceStrength = -GravityStrengthMax;

			PrimComp->AddRadialForce(GetActorLocation(), sphereRadius, forceStrength, ERadialImpulseFalloff::RIF_Constant, true);
		}
	}
}

// Tutorial solution
void ABlackHole::OverlapInnerSphere(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		OtherActor->Destroy();
	}
}