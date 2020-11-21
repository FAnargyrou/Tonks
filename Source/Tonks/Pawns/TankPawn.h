// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "TankPawn.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class TONKS_API ATankPawn : public ABasePawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATankPawn();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	FVector MoveDirection;
	FQuat TurnDirection;
	FQuat RotationDirection;
	FQuat LookUpDirection;

	void CalculateMoveInput(float Value);
	void CalculateTurnInput(float Value);
	void CalculateRotateInput(float Value);
	void CalculateLookUpInput(float Value);

	void Move();
	void Turn();
	void Rotate();
	void LookUp();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Tank Movement", meta = (AllowPrivateAccess = "true"))
	float MoveSpeed = 300.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Tank Movement", meta = (AllowPrivateAccess = "true"))
	float TurnSpeed = 150.f;

	float RotateSpeed = 100.f;
	float LookUpSpeed = 100.f;
};
