// Tonks © 2020 Felipe Anargyrou. All Rights Reserved


#include "BasePawn.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Tonks/Actors/ProjectileBase.h"
#include "Tonks/GameModes/TonksGameModeBase.h"

// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	RootComponent = BoxComponent;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body Mesh"));
	BodyMesh->SetupAttachment(RootComponent);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BodyMesh);

	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gun Mesh"));
	GunMesh->SetupAttachment(TurretMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(GunMesh);

	AimModePoint = CreateDefaultSubobject<USceneComponent>(TEXT("Aim Mode Point"));
	AimModePoint->SetupAttachment(GunMesh);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void ABasePawn::BeginPlay()
{
	Super::BeginPlay();
	
	OriginalSpringArmLength = SpringArm->TargetArmLength;

	// Sets Controller Rotation's intial Pitch to SpringArm's Relative Pitch 
	// (To ensure a smooth Start in the game, instead of spawning the camera straight at the back of the tank)
	FRotator Rotator = GetControlRotation();
	Rotator.Pitch = SpringArm->GetRelativeRotation().Pitch;
	Controller->SetControlRotation(Rotator);

	GameModeRef = Cast<ATonksGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GameModeRef)
	{
		UE_LOG(LogTemp, Error, TEXT("Game Mode is not set to TonksGameModeBase. Game loop will not work as intended!"));
	}
}

// Called every frame
void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector ActorUpVector = GetActorUpVector();
	float DotProduct = FVector::DotProduct(ActorUpVector, FVector::UpVector);

	if (DotProduct <= 0.f || FMath::IsNearlyZero(DotProduct, FLOAT_NORMAL_THRESH))
	{
		FVector Velocity = BoxComponent->GetComponentVelocity();
		Velocity.Normalize();

		if (Velocity.IsNearlyZero())
		{
			FRotator Rot = GetActorRotation();
			Rot.Normalize();
			SetActorRotation(FRotator(0.f, Rot.Yaw, 0.f));
		}
	}

	if (bIsPreparingFire)
	{
		CurrentProjectileSpeed = FMath::Clamp(CurrentProjectileSpeed + (ProjectileSpeedIncrement * DeltaTime), 0.f, MaxProjectileSpeed);
		if (CurrentProjectileSpeed >= MaxProjectileSpeed)
			ProjectileSpeedIncrement *= -1.f;
		else if (FMath::IsNearlyZero(CurrentProjectileSpeed, FLOAT_NORMAL_THRESH) || CurrentProjectileSpeed < 0.f)
			ProjectileSpeedIncrement *= -1.f;

		UE_LOG(LogTemp, Warning, TEXT("Current Speed = %f"), CurrentProjectileSpeed);

	}
}

// Called to bind functionality to input
void ABasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABasePawn::Move(FVector MoveDirection)
{
	if (!bIsInAimMode && bIsOnTurn && MoveDistance > 0.f)
	{
		AddActorLocalOffset(MoveDirection, false);
		MoveDistance = FMath::Clamp(MoveDistance - MoveDirection.Size(), 0.f, MaxDistance);
	}
}

void ABasePawn::Turn(FQuat TurnDirection)
{
	if (!bIsInAimMode && bIsOnTurn) AddActorLocalRotation(TurnDirection, true);
}

void ABasePawn::Rotate(FQuat RotationDirection)
{
	AddControllerYawInput(RotationDirection.Rotator().Yaw);
	if (bIsInAimMode && bIsOnTurn)
	{
		// Rotates the TurretMesh's Yaw value; We want the Turret to be attached to the base so only Yaw rotation should be valid here
		FRotator ActorRotation = BodyMesh->GetComponentRotation();
		FRotator Rotation(ActorRotation.Pitch, GetControlRotation().Yaw, ActorRotation.Roll);
		TurretMesh->AddLocalRotation(RotationDirection);
	}
}

void ABasePawn::LookUp(FQuat LookUpDirection)
{
	AddControllerPitchInput(LookUpDirection.Rotator().Pitch * -1.f);
	if (bIsInAimMode && bIsOnTurn)
	{
		// Clamps Gun's rotation to prevent it from rotating 360 degrees
		FRotator NewRotation = GunMesh->GetRelativeRotation();
		NewRotation.Pitch += LookUpDirection.Rotator().Pitch; 
		NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch, GunMinPitch, GunMaxPitch);
		GunMesh->SetRelativeRotation(NewRotation);
	}
}

void ABasePawn::PrepareFire()
{
	if (bIsInAimMode && bIsOnTurn)
		bIsPreparingFire = true;
}

void ABasePawn::ReleaseFire()
{
	if (bIsInAimMode && bIsOnTurn)
	{
		bIsPreparingFire = false;
		Fire();
	}
}

void ABasePawn::Fire()
{
	if (ProjectileClass && bIsInAimMode)
	{
		FVector Position = ProjectileSpawnPoint->GetComponentLocation();
		FRotator Rotation = ProjectileSpawnPoint->GetComponentRotation();

		AProjectileBase* Projectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, Position, Rotation);
		Projectile->AddVelocity(CurrentProjectileSpeed);
		// Reset modifier back to 0.f ready for this Pawn's next turn
		CurrentProjectileSpeed = 0.f;
		Projectile->SetOwner(this);

		APlayerController* Player = Cast<APlayerController>(Controller);
		if (Player)
			Player->SetViewTarget(Projectile);
		MoveMode();
		CurrentProjectile = Projectile;
		bIsOnTurn = false;
	}
}

void ABasePawn::SetOnTurn(bool bOnTurn)
{
	bIsOnTurn = bOnTurn;

}

void ABasePawn::ResetMovement()
{
	MoveDistance = MaxDistance;
}

AActor* ABasePawn::GetCurrentProjectile()
{
	return CurrentProjectile;
}

// TODO - Implement FMath::FInterpConstantTo to reproduce a smooth transition between Aim and Move Modes
// Attaches the SpringArm/Player Camera to the AimModePoint Component, which should be set to somewhere around the GunMesh
void ABasePawn::AimMode()
{
	if (SpringArm && !bIsInAimMode && bIsOnTurn)
	{
		SpringArm->TargetArmLength = 0.f;
		MoveModeRotation = GetControlRotation();
		Controller->SetControlRotation(GunMesh->GetComponentRotation());

		FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget , false);

		SpringArm->AttachToComponent(AimModePoint, Rules);
		SpringArm->bUsePawnControlRotation = false;
		bIsInAimMode = true;
	}
}

// Attaches the SpringArm/Player Camera back to it's original Position and allows player to move around again
void ABasePawn::MoveMode()
{
	if (SpringArm && bIsInAimMode && bIsOnTurn && !bIsPreparingFire)
	{
		SpringArm->TargetArmLength = OriginalSpringArmLength;
		Controller->SetControlRotation(MoveModeRotation);

		FAttachmentTransformRules Rules(EAttachmentRule::KeepRelative, false);

		SpringArm->AttachToComponent(RootComponent, Rules);
		SpringArm->bUsePawnControlRotation = true;
		bIsInAimMode = false;
	}
}

// Called if Controller decides to end turn without firing.
void ABasePawn::EndTurn()
{
	if (GameModeRef && !bIsInAimMode && bIsOnTurn)
	{
		SetOnTurn(false);
		GameModeRef->EndTurn();
	}
}
