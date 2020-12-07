// Tonks © 2020 Felipe Anargyrou. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePawn.generated.h"

class UBoxComponent;
class AProjectileBase;
class USpringArmComponent;
class UCameraComponent;
class ATonksGameModeBase;

UCLASS()
class TONKS_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasePawn();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void InitiateTurn();

	void PrepareFire();
	void ReleaseFire();
	void Fire();
	void ResetMovement();

	AActor* GetCurrentProjectile();
	FString GetPlayerName();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Move(FVector MoveDirection);
	void Turn(FQuat TurnDirection);
	void Rotate(FQuat RotationDirection);
	void LookUp(FQuat LookUpDirection);
	void AimMode();
	void MoveMode();
	void EndTurn();

private:

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// COMPONENTS

	// Mesh Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UBoxComponent* BoxComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* BodyMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* TurretMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* GunMesh;

	

	// Camera Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UCameraComponent* Camera;

	// Dummy Components

	// Points to where the Projectile will spawn when player Fires
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		USceneComponent* ProjectileSpawnPoint;

	// Points to where the camera will go when the player goes into Aim Mode (When holding Right Mouse Click)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		USceneComponent* AimModePoint;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// VARIABLES

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile Settings", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AProjectileBase> ProjectileClass;

	// Spring Arm Length used in MoveMode; Obtained from whatever value is set in the Blueprint
	float OriginalSpringArmLength = 0.f;

	// Boolean to determine if Player is in Move or Rotate/Aim mode (ie. hold mouse right click)
	bool bIsInAimMode = false;

	// Boolean to dermine whether Pawn is on their Turn or Not
	bool bIsOnTurn = false;

	// To ensure a smooth transition between Aim Mode and Move Mode cameras, we're using this variable to store
	// the Controller's original Rotation before moving into Aim Mode;
	// In Aim Mode, the Player Controller rotation gets replaced by the GunMesh's WorldRotation
	FRotator MoveModeRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MaxDistance = 1500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MoveDistance = 1500.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Aim", meta = (AllowPrivateAccess = "true"))
	float GunMinPitch = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aim", meta = (AllowPrivateAccess = "true"))
	float GunMaxPitch = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Settings", meta = (AllowPrivateAccess = "true"))
	float MaxProjectileSpeed = 3000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Settings", meta = (AllowPrivateAccess = "true"))
	float CurrentProjectileSpeed = 0.f;
	float ProjectileSpeedIncrement = 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile Settings", meta = (AllowPrivateAccess = "true"))
	bool bIsPreparingFire = false;

	ATonksGameModeBase* GameModeRef;
	AProjectileBase* CurrentProjectile;

	UPROPERTY(BlueprintReadWrite, Category = "Player", meta = (AllowPrivateAccess = "true"))
	FString PlayerName = "Default Name";
};
