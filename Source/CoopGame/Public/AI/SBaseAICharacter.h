// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SBaseAICharacter.generated.h"

class USHealthComponent;
class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;
class USoundCue;

UCLASS()
class COOPGAME_API ASBaseAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASBaseAICharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USHealthComponent* HealthComponent;

	/*
	 Test Infructueux 1 et 2
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Animations")
	UAnimMontage* MeleAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimSequence* Anim;
	*/

	//Perception Component
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UAIPerceptionComponent* SensingComponent;

	UPROPERTY(EditDefaultsOnly , Category = "Components")
	UAISenseConfig_Sight* SightSense;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UAISenseConfig_Hearing* HearingSense;
	///

	UPROPERTY(EditDefaultsOnly, Category = "Attacking")
	float MeleDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float SprintSpeedModifier;


	UFUNCTION(BlueprintCallable, Category = "MeleAttack")
	void Mele_Strike(AActor* Target);

	UPROPERTY(BlueprintReadOnly, Category = "Character")
	bool bAttacking;

	UPROPERTY(BlueprintReadOnly, Category = "Character")
	bool bDied;

	UFUNCTION()
	void OnHealthChanged(USHealthComponent* HealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void SenseStuff(const TArray<AActor*>& testActors);

	
	FTimerHandle AttackHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	USoundCue* MeleStrikeSound;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SetAttacking();
};
