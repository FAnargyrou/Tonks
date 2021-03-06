// Tonks � 2020 Felipe Anargyrou. All Rights Reserved


#include "HealthComponent.h"
#include "Tonks/GameModes/TonksGameModeBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void UHealthComponent::CalculateDamage()
{

	UE_LOG(LogTemp, Warning, TEXT("Damage Received = %f"), DamageReceived);
	CurrentHealth = FMath::Clamp(CurrentHealth - DamageReceived, 0.f, MaxHealth);
	DamageReceived = 0.f;
	if (CurrentHealth <= 0.f)
	{
		if (GameMode)
		{
			GameMode->ActorDied(GetOwner());
		}
	}
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
	GameMode = Cast<ATonksGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);

}

void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage == 0.f) return;

	UE_LOG(LogTemp, Warning, TEXT("%f"), Damage);

	DamageReceived = Damage;

}
