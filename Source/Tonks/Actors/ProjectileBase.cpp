// Tonks © 2020 Felipe Anargyrou. All Rights Reserved


#include "ProjectileBase.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Tonks/GameModes/TonksGameModeBase.h"
#include "Tonks/Pawns/BasePawn.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
	RootComponent = ProjectileMesh;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->InitialSpeed = MovementSpeed;
	ProjectileMovement->MaxSpeed = MovementSpeed;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	GameModeRef = Cast<ATonksGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Allows player to rotate around Projectile with it's own camera rotation

	FRotator CameraRotation = GetWorld()->GetFirstPlayerController()->GetControlRotation();
	SpringArm->SetWorldRotation(CameraRotation);
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AActor* MyOwner = GetOwner();
	if (!MyOwner) return;
	UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherActor->GetName());
	FVector Origin;
	ABasePawn* TargetPawn = Cast <ABasePawn>(OtherActor);

	if (TargetPawn)
	{
		Origin = TargetPawn->GetActorLocation();
	}
	else
	{
		Origin = GetActorLocation();
	}

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);

	UGameplayStatics::ApplyRadialDamageWithFalloff(GetWorld(), Damage, MinimumDamage, Origin,
		InnerRadius, OuterRadius, DamageFalloff, DamageType, IgnoreActors, this, MyOwner->GetInstigatorController());
	GameModeRef->EndTurn();
	// Set this as hidden and let GameMode delete this actor after damage dealt is displayed to the player.
	SetActorHiddenInGame(true);
	// MAGIC NUMBER!!! This should be moved to a member variable
	SpringArm->TargetArmLength += 200.f;
}