// Tonks © 2020 Felipe Anargyrou. All Rights Reserved


#include "TonksGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Tonks/Pawns/BasePawn.h"
#include "Tonks/Controllers/TankPlayerController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h" // TEMP


void ATonksGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> Actors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABasePawn::StaticClass(), Actors);

	for (AActor* Actor : Actors)
	{
		ABasePawn* Tank = Cast<ABasePawn>(Actor);

		if (Tank) Tanks.Add(Tank);
	}

	TotalTanks = Tanks.Num();
	for (int8 Index = 0; Index < TotalTanks; ++Index)
	{
		TurnOrder.Add(Index);
	}
	for (int32 i = 0; i < TotalTanks - 1; ++i)
	{
		int32 SwapIndex = FMath::RandRange(i, TotalTanks - 1);
		Tanks.Swap(i, SwapIndex);
	}

	PlayerControllerRef = Cast<ATankPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	StartTurn();
}


void ATonksGameModeBase::StartTurn()
{
	if (PlayerControllerRef && Tanks.Num() > 0 && Tanks[CurrentTurnIndex])
	{
		PlayerControllerRef->Possess(Tanks[CurrentTurnIndex]);
		Tanks[CurrentTurnIndex]->SetOnTurn(true);
		Tanks[CurrentTurnIndex]->ResetMovement();
	}
}

void ATonksGameModeBase::EndTurn()
{
	// If Tank Projectile exists then we should set a timer to let the animation play; Otherwise we can just end the turn.
	if (Tanks.Num() > 0 && Tanks[CurrentTurnIndex] && Tanks[CurrentTurnIndex]->GetCurrentProjectile())
	{
		GetWorldTimerManager().SetTimer(EndTurnTimerHandle, this, &ATonksGameModeBase::HandleEndTurn, EndTurnDelay);
	}
	else
	{
		HandleEndTurn();
	}
}

void ATonksGameModeBase::HandleGameStart()
{
}

void ATonksGameModeBase::HandleGameOver()
{
	// To be defined 
}

void ATonksGameModeBase::HandleEndTurn()
{
	if (PlayerControllerRef)
	{
		if (Tanks.Num() > 0 && Tanks[CurrentTurnIndex] && Tanks[CurrentTurnIndex]->GetCurrentProjectile())
			Tanks[CurrentTurnIndex]->GetCurrentProjectile()->Destroy();

		PlayerControllerRef->UnPossess();
		if (++CurrentTurnIndex >= TotalTanks)
			CurrentTurnIndex = 0;

		StartTurn();
	}
}

void ATonksGameModeBase::ActorDied(AActor* DeadActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Actor Died OH NOOO!!"));

	ABasePawn* DeadTank = Cast<ABasePawn>(DeadActor);
	if (DeadTank)
	{
		Tanks.Remove(DeadTank);
	}
	DeadActor->Destroy();
	TotalTanks--;
		
}