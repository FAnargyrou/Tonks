// Tonks © 2020 Felipe Anargyrou. All Rights Reserved


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
	InputComponent->BindAction("EndTurn", IE_Pressed, this, &ATankPlayerController::EndTurnInput);
}

void ATankPlayerController::OnPossess(APawn* PawnToPossess)
{
	Super::OnPossess(PawnToPossess);

	if (!PawnToPossess) return;
	
	// Get Possessed pawn rotation and reset Roll to 0.f. 
	// This prevents camera rotation from being glitched if pawn is possessed on an angled surface
	FRotator NewRotation = PawnToPossess->GetActorRotation();
	NewRotation.Roll = 0.f;
	SetControlRotation(NewRotation);

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

void ATankPlayerController::EndTurnInput()
{
	if (TankPawn)
	{
		TankPawn->EndTurnPressed();
	}
}