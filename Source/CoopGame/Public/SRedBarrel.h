// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SRedBarrel.generated.h"


class UStaticMeshComponent;
class UParticleSystem;
class URadialForceComponent;
UCLASS()
class COOPGAME_API ASRedBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASRedBarrel();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UMaterialInterface* SecondMat;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UParticleSystem* ExplsionEffect;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	URadialForceComponent* RadialForceComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditInstanceOnly, Category = "Explosive Barrel")
	float Radius;

	UPROPERTY(EditInstanceOnly, Category = "Explosive Barrel")
	float Dmg;

	UPROPERTY(EditInstanceOnly, Category = "Explosive Barrel")
	float ExplosionImpulse;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (ClampMax = 100.0f))
	float CurrentHealth; //max is 100


	bool bDead;
	bool bExploding;

	UFUNCTION()
	void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	void Explode();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
