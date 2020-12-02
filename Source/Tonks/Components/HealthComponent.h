// Tonks © 2020 Felipe Anargyrou. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class ATonksGameModeBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TONKS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();
	UFUNCTION(BlueprintCallable)
	void CalculateDamage();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UFUNCTION()
	void TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthPoints", meta = (AllowPrivateAccess = "true"))
	float MaxHealth = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthPoints", meta = (AllowPrivateAccess = "true"))
	float CurrentHealth = 0.f;
	UPROPERTY(BlueprintReadOnly, Category = "HealthPoints", meta = (AllowPrivateAccess = "true"))
	float DamageReceived = 0.f;

	ATonksGameModeBase* GameMode;
};
