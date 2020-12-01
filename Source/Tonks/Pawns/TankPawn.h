// Tonks © 2020 Felipe Anargyrou. All Rights Reserved

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

	// Variables accessed by TankPlayerController
	void CalculateMoveInput(float Value);
	void CalculateTurnInput(float Value);
	void CalculateRotateInput(float Value);
	void CalculateLookUpInput(float Value);

	void SetAimMode();
	void SetMoveMode();

	void EndTurnPressed();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// INPUT FUNCTIONS





	// VARIABLES

	FVector MoveDirection;
	FQuat TurnDirection;
	FQuat RotationDirection;
	FQuat LookUpDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MoveSpeed = 300.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float TurnSpeed = 150.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float RotateSpeed = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float LookUpSpeed = 100.f;
};
