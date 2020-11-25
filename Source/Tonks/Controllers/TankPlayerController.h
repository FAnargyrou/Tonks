// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

class ATankPawn;

UCLASS()
class TONKS_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

protected:
	// Function to bind Input to functions
	virtual void SetupInputComponent() override;

	virtual void OnPossess(APawn* PawnToPossess) override;

private:
	void MoveInput(float Amount);
	void TurnInput(float Amount);
	void RotateInput(float Amount);
	void LookUpInput(float Amount);
	void AimInputPressed();
	void AimInputReleased();
	void FireInput();


	ATankPawn* TankPawn;
};
