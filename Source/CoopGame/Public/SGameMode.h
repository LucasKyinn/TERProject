// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SGameMode.generated.h"

enum class EWaveState : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnActorKilled, AActor*, VictimActor, AActor*, KillerActor, AController*, KillerController );

UCLASS()
class COOPGAME_API ASGameMode : public AGameModeBase
{
	GENERATED_BODY()


protected:

	FTimerHandle TimerHandle_BotSpawner; 
	FTimerHandle TimerHandle_NextWaveStart;

	//BotToSpawn in Wave
	int32 NrOfBotsToSpawn;

	int32 WaveCount; 

	UPROPERTY(EditDefaultsOnly,Category="GameMode")
	float TimeBetweenWaves;
protected:

	//Hook for BP to spawn a single Bot
	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
	void SpawnNewBot();

	void SpawnBotTimerElapsed();

	//StartSpawningBots
	void StartWave();

	//StopSpawningBot
	void EndWave();

	//TimerForNextWave
	void PrepareForNextWave();

	void CheckWaveState();

	void CheckAnyPlayerAlive();

	void GameOver();

	void SetWaveState(EWaveState NewState);

	void RepawnDeadPlayers();

	//PlayerTeamNum
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HealthComponent")
	uint8 PlayerTeamNum;

public:
	
	ASGameMode();

	virtual void StartPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(BluePrintAssignable, Category = "GameMode")
	FOnActorKilled OnActorKilled;
};
