// Tonks © 2020 Felipe Anargyrou. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TonksGameModeBase.generated.h"

class ABasePawn;
class ATankPlayerController;


UCLASS()
class TONKS_API ATonksGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:

	void ActorDied(AActor* DeadActor);
	void StartTurn();
	void EndTurn();
	
protected:
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintImplementableEvent)
	void GameStart();
	UFUNCTION(BlueprintImplementableEvent)
	void GameOver();

private:
	void HandleGameStart();
	void HandleGameOver();
	void HandleEndTurn();

	int8 TotalTanks = 0;
	int8 CurrentTurnIndex = 0;
	TArray<int32> TurnOrder;
	TArray<ABasePawn*> Tanks;
	ABasePawn* CurrentTank;
	ATankPlayerController* PlayerControllerRef;
	
	FTimerHandle EndTurnTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn Settings", meta = (AllowPrivateAccess = "true"))
	float EndTurnDelay = 5.f;
};
