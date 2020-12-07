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

}


void ATonksGameModeBase::StartTurn()
{
	UE_LOG(LogTemp, Warning, TEXT("Starting Turn"));
	if (PlayerControllerRef && Tanks.Num() > 0 && Tanks[CurrentTurnIndex])
	{
		CurrentTank = Tanks[CurrentTurnIndex];
		if (CurrentTank)
		{
			PlayerControllerRef->Possess(CurrentTank);
			CurrentTank->ResetMovement();
		}
	}
}

void ATonksGameModeBase::EndTurn()
{
	// If Tank Projectile exists then we should set a timer to let the animation play; Otherwise we can just end the turn.
	if (CurrentTank && CurrentTank->GetCurrentProjectile())
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
	if (!PlayerControllerRef)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController is not set to ATankPlayerController. Game Loop will NOT work as intended!"));
	}
	else
	{
		PlayerControllerRef->AddWidget();
	}

	StartTurn();
}

void ATonksGameModeBase::HandleGameOver()
{
	if (Tanks.Num() != 0 && Tanks[0])
		LastTankAlive = Tanks[0];
	GameOver();
}

void ATonksGameModeBase::HandleEndTurn()
{
	UE_LOG(LogTemp, Warning, TEXT("Current Turn: %i"), CurrentTurnIndex);
	if (PlayerControllerRef)
	{
		if (CurrentTank && CurrentTank->GetCurrentProjectile())
			CurrentTank->GetCurrentProjectile()->Destroy();

		PlayerControllerRef->UnPossess();
		if (++CurrentTurnIndex >= TotalTanks)
			CurrentTurnIndex = 0;
		if (TotalTanks <= 1)
		{
			HandleGameOver();
			return;
		}

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
		if (DeadTank == CurrentTank)
		{
			PlayerControllerRef->UnPossess();
			CurrentTank = nullptr;
		}
	}

	DeadActor->Destroy();
	TotalTanks--;
		
}