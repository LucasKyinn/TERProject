// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPowerupActor.generated.h"

UCLASS()
class COOPGAME_API ASPowerupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPowerupActor();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Powerups" )
	float PowerupInterval;


	UPROPERTY(EditDefaultsOnly, Category = "Powerups")
	int32 TotalNrOfTicks;

	int32 TickProcessed;
	 
	FTimerHandle TimerHandle_PowerupTick;

	UFUNCTION()
	void OnTickPowerup();

	UPROPERTY(ReplicatedUsing=OnRep_PowerUpActive)
	bool bIsPowerUpActive;

	UFUNCTION()
	void 	OnRep_PowerUpActive();

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
	void bIsPowerUpStateChange(bool bNewIsActive);

public:	

	void ActivatepowerUp(AActor* ActivateFor);

	UFUNCTION(BlueprintImplementableEvent, Category="Powerups")
	void OnActivated(AActor* ActivateFor);

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
	void OnExpired();

	UFUNCTION(BlueprintImplementableEvent, Category = "Powerups")
	void OnPowerUpTicked();
};
