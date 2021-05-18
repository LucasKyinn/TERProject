// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameState.h"
#include "Net/UnrealNetwork.h"

void ASGameState :: OnRep_WaveState(EWaveState OldState)
{
	WaveStateChanged(WaveState,OldState);
}

void ASGameState::SetWaveState(EWaveState NewState)
{
	EWaveState OldState = WaveState;
	if (GetLocalRole() == ROLE_Authority) {
		WaveState = NewState;
		OnRep_WaveState(WaveState);
	}
}

void ASGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASGameState, WaveState);

}


