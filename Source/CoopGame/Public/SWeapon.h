// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWeapon.generated.h"



class USkeletalMeshComponent;
class UDamageType;
class UParticleSystem;
class UMatineeCameraShake;
class USoundCue;
//Contains info of a single hitscanweapon lignetrace
USTRUCT()
struct FHitScanTrace
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FVector_NetQuantize TraceTo;

	UPROPERTY()
	TEnumAsByte<EPhysicalSurface> SurfaceType;
};

UCLASS()
class COOPGAME_API ASWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASWeapon();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* MeshComp;

	void PlayFireEffects(FVector TracerEndPoint);


	void PlayImpactEffects(EPhysicalSurface SurfaceType, FVector ImpactPoint);

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Weapon")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName MuzzleSocketName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName TracerTargetName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* DefaultImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* FleshImpactEffect;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* TracerEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<UMatineeCameraShake> FireCamShake;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float BaseDamage;   

	virtual void Fire();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerFire();

	FTimerHandle TimerHandle_TimeBetweenShots;

	float LastTimeFired;

	float TimeBetweenShots;

	//Bullet Per minute
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float RateOfFire;

	UPROPERTY(ReplicatedUsing=OnRep_HitScanTrace)
	FHitScanTrace HitScanTrace;

	//Bullet spread in degrees
	UFUNCTION()
	void OnRep_HitScanTrace();

	UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta=(ClampMin=0.3f))
	float BulletSpread;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	USoundCue* FireSound;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	USoundCue* EmptyMagSound;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	int MagSize;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	int CurrentAmo;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float ReloadTime;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float AmmoHeld;
	
public:
	void StartFire();
	void StopFire();
	int AmmoCheck();
	int GetAmmoHeld();
	void AddAmmo(int Amount);
	void Reloaded();
	float GetReloadTime();

};
