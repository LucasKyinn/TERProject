// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SAmmoDrop.generated.h"


class USphereComponent;

UCLASS()
class COOPGAME_API ASAmmoDrop : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASAmmoDrop();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		USphereComponent* SphereComp;

public:	

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
