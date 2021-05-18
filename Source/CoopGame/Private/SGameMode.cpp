// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameMode.h"
#include "TimerManager.h"
#include "SGameState.h"
#include "SPlayerState.h"
#include "Components/SHealthComponent.h"

ASGameMode::ASGameMode()
{
	TimeBetweenWaves = 2.0f;

	GameStateClass = ASGameState::StaticClass(); 
	PlayerStateClass = ASPlayerState::StaticClass();
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;
	//Default team is 0, have to be setup in Blueprint
	PlayerTeamNum = 0;
}
void ASGameMode::StartWave()
{
	WaveCount++;
	NrOfBotsToSpawn = 2 * WaveCount;
	GetWorldTimerManager().SetTimer(TimerHandle_BotSpawner, this, &ASGameMode::SpawnBotTimerElapsed,1.0f,true,0.0f);

	SetWaveState(EWaveState::WaveInProgress);
}

void ASGameMode::EndWave()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_BotSpawner);
	SetWaveState(EWaveState::WaitingToComplete);
}

void ASGameMode::PrepareForNextWave()
{
	GetWorldTimerManager().SetTimer(TimerHandle_NextWaveStart, this, &ASGameMode::StartWave, TimeBetweenWaves, false);
	SetWaveState(EWaveState::WaitingToStart);
	RepawnDeadPlayers();
}

void ASGameMode::CheckWaveState()
{
	bool bIsPreparingForWave = GetWorldTimerManager().IsTimerActive(TimerHandle_NextWaveStart);
	
	if (NrOfBotsToSpawn > 0 || bIsPreparingForWave) {
		return;
	}
	bool bIsAnyBotAlive = false;

	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It;It++) {
		APawn* TestPawn = It->Get();
		if (TestPawn == nullptr || TestPawn->IsPlayerControlled()) {
			continue;
		}
		USHealthComponent* HealthComp = Cast<USHealthComponent>(TestPawn->GetComponentByClass(USHealthComponent::StaticClass()));
		if (HealthComp && HealthComp->GetHealth() >0.0f && HealthComp->TeamNum != PlayerTeamNum) {
			bIsAnyBotAlive = true;
			break;
		}
	}
	if (!bIsAnyBotAlive) {
		SetWaveState(EWaveState::WaveComplete);
		PrepareForNextWave();
	}
}

void ASGameMode::CheckAnyPlayerAlive()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It) {
		APlayerController* PC = It->Get();
		if (PC && PC->GetPawn()) {
			APawn* MyPawn = PC->GetPawn();
			USHealthComponent* HealthComp = Cast<USHealthComponent>(MyPawn->GetComponentByClass(USHealthComponent::StaticClass()));
			if (ensure(HealthComp) && HealthComp->GetHealth() > 0.0f) {
				//Someone Alive and Well
				return;
			}
		}
	}
	//Dead
	GameOver();
}

void ASGameMode::GameOver()
{
	EndWave();
	//Todo finish the map 'gameover'
	UE_LOG(LogTemp,Log,TEXT("GAME OVER, Everybody died ! "))
	SetWaveState(EWaveState::GameOver);
}

void ASGameMode::SetWaveState(EWaveState NewState)
{
	ASGameState* GS = GetGameState<ASGameState>();
	if (ensureAlways(GS)) {
		GS->SetWaveState( NewState);

	}
}

void ASGameMode::RepawnDeadPlayers()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It) {
		APlayerController* PC = It->Get();
		if (PC && PC->GetPawn()== nullptr) {
			RestartPlayer(PC);
		}
	}
}
 


void ASGameMode::StartPlay()
{
	Super::StartPlay();
	PrepareForNextWave();

}

void ASGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	CheckWaveState();
	CheckAnyPlayerAlive();
}

void ASGameMode::SpawnBotTimerElapsed()
{
	SpawnNewBot();
	NrOfBotsToSpawn--;
	if (NrOfBotsToSpawn <= 0) {
		EndWave();
	}
}