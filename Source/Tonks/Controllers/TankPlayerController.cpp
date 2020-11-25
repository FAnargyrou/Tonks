// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "Tonks/Pawns/TankPawn.h"

void ATankPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("Move", this, &ATankPlayerController::MoveInput);
	InputComponent->BindAxis("Turn", this, &ATankPlayerController::TurnInput);
	InputComponent->BindAxis("Rotate", this, &ATankPlayerController::RotateInput);
	InputComponent->BindAxis("LookUp", this, &ATankPlayerController::LookUpInput);
	InputComponent->BindAction("Aim", IE_Pressed, this, &ATankPlayerController::AimInputPressed);
	InputComponent->BindAction("Aim", IE_Released, this, &ATankPlayerController::AimInputReleased);
	InputComponent->BindAction("Fire", IE_Pressed, this, &ATankPlayerController::FireInput);
}

void ATankPlayerController::OnPossess(APawn* PawnToPossess)
{
	Super::OnPossess(PawnToPossess);

	if (!PawnToPossess) return;

	TankPawn = Cast<ATankPawn>(PawnToPossess);
}

void ATankPlayerController::MoveInput(float Amount)
{
	if (TankPawn)
	{
		TankPawn->CalculateMoveInput(Amount);
	}
}

void ATankPlayerController::TurnInput(float Amount)
{
	if (TankPawn)
	{
		TankPawn->CalculateTurnInput(Amount);
	}
}

void ATankPlayerController::RotateInput(float Amount)
{
	if (TankPawn)
	{
		TankPawn->CalculateRotateInput(Amount);
	}
}

void ATankPlayerController::LookUpInput(float Amount)
{
	if (TankPawn)
	{
		TankPawn->CalculateLookUpInput(Amount);
	}
}

void ATankPlayerController::AimInputPressed()
{
	if (TankPawn)
	{
		TankPawn->SetAimMode();
	}
}

void ATankPlayerController::AimInputReleased()
{
	if (TankPawn)
	{
		TankPawn->SetMoveMode();
	}
}

void ATankPlayerController::FireInput()
{
	if (TankPawn)
	{
		TankPawn->Fire();
	}
}