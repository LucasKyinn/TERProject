// Fill out your copyright notice in the Description page of Project Settings.


#include "SAmmoDrop.h"
#include "Components/SphereComponent.h"
#include "SChatacter.h"


// Sets default values
ASAmmoDrop::ASAmmoDrop()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetSphereRadius(75.0f);
	RootComponent = SphereComp;

	FActorSpawnParameters SpawnParams;

}

// Called when the game starts or when spawned
void ASAmmoDrop::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASAmmoDrop::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	APawn* Pawn = Cast<APawn>(OtherActor);
	if (Pawn && Pawn->IsPlayerControlled()) {
		ASChatacter* Player = Cast<ASChatacter>(Pawn);

		Player->FoundAmmo(FMath::RandRange(2,4));
		Destroy();
	}
}

