// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlackHole.generated.h"

class USphereComponent;

UCLASS()
class FPSGAME_API ABlackHole : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlackHole();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, Category = "Black Hole")
	UStaticMeshComponent* BlackHoleMesh;

	UPROPERTY(EditAnywhere, Category = "Black Hole")
	USphereComponent* GravitySphereComp;

	UPROPERTY(EditAnywhere, Category = "Black Hole")
	USphereComponent* DestructionSphereComp;

	UPROPERTY(EditAnywhere, Category = "Black Hole")
	float GravityStrengthMax;

	UPROPERTY(EditAnywhere, Category = "Black Hole")
	float GravityStrengthMin;

	UPROPERTY(EditAnywhere, Category = "Black Hole")
	bool DisableGravityOnAffectedActors;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Tutorial solution
	UFUNCTION()
	void OverlapInnerSphere(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	void MySolution();
	void TutorialSolution();
	
};
