// Fill out your copyright notice in the Description page of Project Settings.


#include "TonksGameModeBase.h"

void ATonksGameModeBase::BeginPlay()
{
	// Get references here

	// Call HandleGameStart()
}

void ATonksGameModeBase::HandleGameStart()
{
}

void ATonksGameModeBase::HandeGameOver()
{
	// To be defined 
}

void ATonksGameModeBase::ActorDied(AActor* DeadActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Actor Died OH NOOO!!"));
	DeadActor->Destroy();
}