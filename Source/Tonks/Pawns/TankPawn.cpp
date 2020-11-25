// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"

ATankPawn::ATankPawn()
{

}

// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move(MoveDirection);
	Turn(TurnDirection);
	Rotate(RotationDirection);
	LookUp(LookUpDirection);
}

// Called to bind functionality to input
void ATankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ATankPawn::CalculateMoveInput(float Value)
{
	MoveDirection = FVector(Value * MoveSpeed * GetWorld()->DeltaTimeSeconds, 0.f, 0.f);
}

void ATankPawn::CalculateTurnInput(float Value)
{
	float Amount = Value * TurnSpeed * GetWorld()->DeltaTimeSeconds;
	FRotator Turn = FRotator(0.f, Amount, 0.f);
	TurnDirection = FQuat(Turn);
}

void ATankPawn::CalculateRotateInput(float Value)
{
	float Amount = Value * RotateSpeed * GetWorld()->DeltaTimeSeconds;
	FRotator Rotation = FRotator(0.f, Amount, 0.f);
	RotationDirection = FQuat(Rotation);
}

void ATankPawn::CalculateLookUpInput(float Value)
{
	float Amount = Value * LookUpSpeed * GetWorld()->DeltaTimeSeconds;
	FRotator Rotation = FRotator(Amount, 0.f, 0.f);
	LookUpDirection = FQuat(Rotation);
}

void ATankPawn::SetAimMode()
{
	AimMode();
}

void ATankPawn::SetMoveMode()
{
	MoveMode();
}



