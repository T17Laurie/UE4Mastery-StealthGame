// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSGameState.h"




void AFPSGameState::MulticastOnMissionComplete_Implementation(APawn* InstigatorPawn, bool bMissionSuccess)
{
	// Iterate through all the pawns in the world (incl. enemies)
	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; It++)
	{
		APawn* pawn = It->Get();

		if (pawn != nullptr && pawn->IsLocallyControlled())
		{
			pawn->DisableInput(nullptr);
		}
	}
}
