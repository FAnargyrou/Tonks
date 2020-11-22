// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TonksGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class TONKS_API ATonksGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:

	void ActorDied(AActor* DeadActor);

protected:
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintImplementableEvent)
	void GameStart();
	UFUNCTION(BlueprintImplementableEvent)
	void GameOver();

private:

	void HandleGameStart();
	void HandeGameOver();
	
};
