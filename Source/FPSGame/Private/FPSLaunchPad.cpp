// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSLaunchPad.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/Classes/GameFramework/Character.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"

PRAGMA_DISABLE_OPTIMIZATION

// Sets default values
AFPSLaunchPad::AFPSLaunchPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Launchpad root component"));

	LaunchPadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Launchpad mesh"));
	LaunchPadMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LaunchPadMesh->SetupAttachment(RootComponent);

	ArrowDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("Launchpad decal"));
	ArrowDecal->SetupAttachment(RootComponent);

	ParticleEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Laucnpad particle effect"));
	ParticleEffect->bAutoActivate = false;
	ParticleEffect->SetupAttachment(RootComponent);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Launchpad collision box"));
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionBox->SetupAttachment(RootComponent);

	LaunchVector = CreateDefaultSubobject<UArrowComponent>(TEXT("Launchpad launch vector"));
	LaunchVector->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LaunchVector->SetupAttachment(RootComponent);

	// Set up collision callback
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AFPSLaunchPad::OnLaunchpadOverlapped);
}

// Called when the game starts or when spawned
void AFPSLaunchPad::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFPSLaunchPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFPSLaunchPad::OnLaunchpadOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("Launch pad collision detected!"));

	FVector launchVel = LaunchVector->GetComponentRotation().Vector() * LaunchSpeed;

	ACharacter* character = Cast<ACharacter>(OtherActor);

	if (character != nullptr)
	{
		character->LaunchCharacter(launchVel, false, false);
	}
	else
	{
		if (OtherComp != nullptr && OtherComp->IsSimulatingPhysics())
		{
			OtherComp->AddImpulse(launchVel * 1.5f, NAME_None, true);
		}
	}

	ParticleEffect->Activate();

	// Tutorial solution for particles
//	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ActivateLaunchPadEffect, GetActorLocation());
}

