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

	PlayerInputComponent->BindAxis("Move", this, &ATankPawn::CalculateMoveInput);
	PlayerInputComponent->BindAxis("Turn", this, &ATankPawn::CalculateTurnInput);
	PlayerInputComponent->BindAxis("Rotate", this, &ATankPawn::CalculateRotateInput);
	PlayerInputComponent->BindAxis("LookUp", this, &ATankPawn::CalculateLookUpInput);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ATankPawn::Fire);
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ATankPawn::SetAimMode);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &ATankPawn::SetMoveMode);
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



