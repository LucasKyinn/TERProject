// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerupActor.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASPowerupActor::ASPowerupActor()
{
	PowerupInterval = 0.0f;
	TotalNrOfTicks = 0;

	bIsPowerUpActive = false;

	SetReplicates(true);
}

void ASPowerupActor::OnTickPowerup()
{
	TickProcessed++;

	OnPowerUpTicked();
	if (TickProcessed >= TotalNrOfTicks) {
		OnExpired();
		bIsPowerUpActive = false;
		OnRep_PowerUpActive();

		GetWorldTimerManager().ClearTimer(TimerHandle_PowerupTick);
	}
}

void ASPowerupActor::OnRep_PowerUpActive()
{
	bIsPowerUpStateChange(bIsPowerUpActive);
}

void ASPowerupActor::ActivatepowerUp(AActor* ActivateFor)
{
	OnActivated(ActivateFor);
	bIsPowerUpActive = true ;
	OnRep_PowerUpActive();
	if (PowerupInterval > 0.0f) {
		GetWorldTimerManager().SetTimer(TimerHandle_PowerupTick, this, &ASPowerupActor::OnTickPowerup, PowerupInterval, true);
	}
	else {
		OnTickPowerup();
	}
}

void ASPowerupActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASPowerupActor, bIsPowerUpActive);

}

