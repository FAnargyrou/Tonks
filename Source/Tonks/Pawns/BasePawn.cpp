// Tonks © 2020 Felipe Anargyrou. All Rights Reserved


#include "BasePawn.h"
#include "Components/BoxComponent.h"
#include "Tonks/Actors/ProjectileBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"

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
}

// Called every frame
void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector ActorUpVector = GetActorUpVector();
	float DotProduct = FVector::DotProduct(ActorUpVector, FVector::UpVector);

	// UE_LOG(LogTemp, Warning, TEXT("Dot Product = %f"), DotProduct);
	UE_LOG(LogTemp, Warning, TEXT("Actor Rotation = %s"), *GetActorRotation().ToString());
	UE_LOG(LogTemp, Warning, TEXT("Up Vector = %s"), *GetActorUpVector().ToString());

	FVector Direction;

	if (DotProduct <= 0.f)
	{
		SetActorRotation(FRotator(0.f, 0.f, 0.f));
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
		FRotator Rotation(0.f, GetControlRotation().Yaw, 0.f);
		TurretMesh->SetWorldRotation(Rotation);
	}
}

void ABasePawn::LookUp(FQuat LookUpDirection)
{
	AddControllerPitchInput(LookUpDirection.Rotator().Pitch * -1.f);
	// TODO - Limit pitch input
	if (bIsInAimMode && bIsOnTurn)
	{
		GunMesh->SetWorldRotation(GetControlRotation());
	}
}

void ABasePawn::Fire()
{
	if (ProjectileClass && bIsInAimMode)
	{
		FVector Position = ProjectileSpawnPoint->GetComponentLocation();
		FRotator Rotation = ProjectileSpawnPoint->GetComponentRotation();

		AProjectileBase* Projectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, Position, Rotation);
		Projectile->SetOwner(this);

		APlayerController* Player = Cast<APlayerController>(Controller);
		if (Player)
			Player->SetViewTarget(Projectile);
		MoveMode();
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
		bIsInAimMode = true;
	}
}

// Attaches the SpringArm/Player Camera back to it's original Position and allows player to move around again
void ABasePawn::MoveMode()
{
	if (SpringArm && bIsInAimMode && bIsOnTurn)
	{
		SpringArm->TargetArmLength = OriginalSpringArmLength;
		Controller->SetControlRotation(MoveModeRotation);

		FAttachmentTransformRules Rules(EAttachmentRule::KeepRelative, false);

		SpringArm->AttachToComponent(RootComponent, Rules);
		bIsInAimMode = false;
	}
}
