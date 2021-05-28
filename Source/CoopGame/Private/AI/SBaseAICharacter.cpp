// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBaseAICharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SHealthComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include"TimerManager.h"
#include "Perception/PawnSensingComponent.h"
#include "../CoopGame.h"

// Sets default values
ASBaseAICharacter::ASBaseAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// Provide the capsule from absorbing Hits 
	RootComponent = GetCapsuleComponent();
	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);

	//Health Comp (HEALTH, death, ect)
	HealthComponent = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComp"));

	//AI Perception 
	SensingComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component"));

	SightSense = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AI Perception Sight"));
	SensingComponent->ConfigureSense(*SightSense);
	SensingComponent->SetDominantSense(SightSense->GetSenseImplementation());
	
	//Sight config
	SightSense->SightRadius =2000.0f;
	SightSense->LoseSightRadius = (2021.0f);
	SightSense->PeripheralVisionAngleDegrees = 360.0f;
	SightSense->DetectionByAffiliation.bDetectEnemies = true;
	SightSense->DetectionByAffiliation.bDetectNeutrals = true;
	SightSense->DetectionByAffiliation.bDetectFriendlies = false;
	SensingComponent->ConfigureSense(*SightSense);

	//Hearing Setup
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));


	//Modifier
	MeleDamage = 25.0f;
	SprintSpeedModifier = 2.0f;


}

// Called when the game starts or when spawned
void ASBaseAICharacter::BeginPlay()
{
	Super::BeginPlay();
	HealthComponent->OnHealthChanged.AddDynamic(this, &ASBaseAICharacter::OnHealthChanged);


	
}


void ASBaseAICharacter::Mele_Strike(AActor* Target)
{
	/*
	Test Infructueux 1
	if (Anim) {
		GetMesh()->PlayAnimation(Anim, false);
		UE_LOG(LogTemp, Log, TEXT("Animation test  1  ? "));
	}
	*/
	bAttacking = true;
	GetWorld()->GetTimerManager().SetTimer(AttackHandle, this, &ASBaseAICharacter::SetAttacking, 0.2f, false);

	if (MeleStrikeSound) {
		UGameplayStatics::SpawnSoundAttached(MeleStrikeSound,RootComponent );
	}
	/*
	Test Infructueux 2
	if (MeleAnim) {
		PlayAnimMontage(MeleAnim);
		UE_LOG(LogTemp, Log, TEXT("Anime  test 2 ? "));
	}
	*/
	if (Target && Target != this && HealthComponent->GetHealth()>0) {
		ACharacter* TargetCharacter = Cast<ACharacter>(Target);
		if (TargetCharacter) {
			FDamageEvent DmgEvent;
			DmgEvent.DamageTypeClass = nullptr;
			
			Target->TakeDamage(MeleDamage, DmgEvent, GetController(), this);

		}
	}
}

void ASBaseAICharacter::OnHealthChanged(USHealthComponent* HealthComp, float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0.0f && !bDied) {
		//Die!
		bDied = true;
		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		DetachFromControllerPendingDestroy();

		SetLifeSpan(3.0f);

	}
}


// Called every frame
void ASBaseAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASBaseAICharacter::SetAttacking()
{
	bAttacking = false;
}


