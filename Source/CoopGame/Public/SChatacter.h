// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SChatacter.generated.h"


class ASWeapon;
class UCameraComponent;
class USpringArmComponent;
class USHealthComponent;
UCLASS()
class COOPGAME_API ASChatacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASChatacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);

	void MoveRight (float Value);

	void BeginCrouch();

	void EndCrouch();

	void Init_Jump();

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category="Components")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USHealthComponent* HealthComponent;

	bool bWantsToZoom;
	bool bWantToReload;
	UPROPERTY(EditDefaultsOnly, Category = "Player")
	float ZoomedFOV;

	
	float DefaultFOV;

	void BeginZoom();
	void EndZoom();
	void Relaod();

	UPROPERTY(EditDefaultsOnly, Category = "Player", meta= (ClampMin = 0.1, ClampMax = 100))
	float ZoomInterpSpeed;

	UPROPERTY(Replicated)
	ASWeapon* CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
	TSubclassOf<ASWeapon> StarterWeaponClass;


	UPROPERTY(VisibleDefaultsOnly, Category = "Player")
	FName WeaponAttachSocketName;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UPawnNoiseEmitterComponent* PlayerNoiseEmitter;
	

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player")
	bool bIsReloading;

	UFUNCTION()
	void OnHealthChanged(USHealthComponent* HealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	//Relaod Variables 
	FTimerHandle ReloadHandle; 
	void ReloadEnd(); 
	void Reload();



public:	

	UFUNCTION(BlueprintCallable, Category = "Player")
		void FoundAmmo(int amount);

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player")
	bool bDied;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override;

	UFUNCTION(BlueprintCallable, Category = "Player")
	void StartFire();

	UFUNCTION(BlueprintCallable, Category = "Player")
	void StopFire();

	UFUNCTION(BlueprintCallable, Category = "Player")
	FString  Ammo();

};
