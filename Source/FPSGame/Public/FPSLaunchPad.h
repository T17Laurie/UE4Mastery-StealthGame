// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSLaunchPad.generated.h"

class UBoxComponent;
class UDecalComponent;
class UArrowComponent;
class UParticleSystemComponent;

UCLASS()
class FPSGAME_API AFPSLaunchPad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSLaunchPad();

	UPROPERTY(EditAnywhere, Category = "Launch Pad")
	UStaticMeshComponent*		LaunchPadMesh;

	UPROPERTY(EditAnywhere, Category = "Launch Pad")
	UDecalComponent*			ArrowDecal;

	UPROPERTY(EditAnywhere, Category = "Launch Pad")
	UBoxComponent*				CollisionBox;

	UPROPERTY(EditAnywhere, Category = "Launch Pad")
	UParticleSystemComponent*	ParticleEffect;

	UPROPERTY(EditAnywhere, Category = "Launch Pad")
	UArrowComponent*			LaunchVector;

	UPROPERTY(EditAnywhere, Category = "Launch Pad")
	float						LaunchSpeed;

	// Tutorial solution for particles
// 	UPROPERTY(EditDefaultsOnly, Category = "Launch Pad")
// 	UParticleSystem* ActivateLaunchPadEffect;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnLaunchpadOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};
