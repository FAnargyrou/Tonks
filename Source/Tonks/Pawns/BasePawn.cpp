// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "Components/CapsuleComponent.h"
#include "Tonks/Actors/ProjectileBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

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

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void ABasePawn::BeginPlay()
{
	Super::BeginPlay();
	
	OriginalSpringArmLength = SpringArm->TargetArmLength;
	OriginalSpringArmRotation = SpringArm->GetRelativeRotation();
}

// Called every frame
void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Controller->SetControlRotation(GunMesh->GetComponentRotation());
}

// Called to bind functionality to input
void ABasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABasePawn::Move(FVector MoveDirection)
{
	if (!bIsInAimMode) AddActorLocalOffset(MoveDirection, true);
}

void ABasePawn::Turn(FQuat TurnDirection)
{
	if (!bIsInAimMode) AddActorLocalRotation(TurnDirection, true);
}

void ABasePawn::Rotate(FQuat RotationDirection)
{
	if (bIsInAimMode) TurretMesh->AddRelativeRotation(RotationDirection);
}

void ABasePawn::LookUp(FQuat LookUpDirection)
{
	// TODO - Limit pitch input
	if (bIsInAimMode) GunMesh->AddRelativeRotation(LookUpDirection);
}

void ABasePawn::Fire()
{
	if (ProjectileClass && bIsInAimMode)
	{
		FVector Position = ProjectileSpawnPoint->GetComponentLocation();
		FRotator Rotation = ProjectileSpawnPoint->GetComponentRotation();

		AProjectileBase* Projectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, Position, Rotation);
		Projectile->SetOwner(this);
	}
}

void ABasePawn::AimMode()
{
	if (SpringArm && !bIsInAimMode)
	{
		// TODO - Store SpringArm's original values so it can be safely reverted to Original Settings when coming out of Aim Mode
		SpringArm->TargetArmLength = 0.f;
		SpringArm->bUsePawnControlRotation = true;

		FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget , false);

		SpringArm->AttachToComponent(AimModePoint, Rules);
		bIsInAimMode = true;
	}
}

void ABasePawn::MoveMode()
{
	if (SpringArm && bIsInAimMode)
	{
		SpringArm->TargetArmLength = OriginalSpringArmLength;
		SpringArm->bUsePawnControlRotation = false;
		SpringArm->SetRelativeRotation(OriginalSpringArmRotation);

		FAttachmentTransformRules Rules(EAttachmentRule::KeepRelative, false);

		SpringArm->AttachToComponent(RootComponent, Rules);
		bIsInAimMode = false;
	}
}
