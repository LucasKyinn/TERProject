// Fill out your copyright notice in the Description page of Project Settings.


#include "SChatacter.h"
#include "Net/UnrealNetwork.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "..\Public\SChatacter.h"
#include "GameFramework/PawnMovementComponent.h"
#include "SWeapon.h"
#include "Components/CapsuleComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "../CoopGame.h"
#include "Components/SHealthComponent.h"


// Sets default values
ASChatacter::ASChatacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch=true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);

	HealthComponent = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComp"));

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	ZoomedFOV = 70.0f;
	ZoomInterpSpeed = 20.0f;

	WeaponAttachSocketName = "WeaponSocket";

	//Noise Emmitter Comp
	PlayerNoiseEmitter = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("Noise Emitter"));
	PlayerNoiseEmitter->SetAutoActivate(true); //?
}

// Called when the game starts or when spawned
void ASChatacter::BeginPlay()
{
	Super::BeginPlay();

	DefaultFOV = CameraComp->FieldOfView;
	HealthComponent->OnHealthChanged.AddDynamic(this, &ASChatacter::OnHealthChanged);

	if (GetLocalRole() == ROLE_Authority) {
		//Spawn default Weapon 
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		CurrentWeapon = GetWorld()->SpawnActor<ASWeapon>(StarterWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		if (CurrentWeapon) {
			CurrentWeapon->SetOwner(this);
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);
		}
	}
	
}

void ASChatacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);
}

void ASChatacter::MoveRight(float Value) 
{
	AddMovementInput(GetActorRightVector() * Value);
}

void ASChatacter::BeginCrouch()
{
	Crouch();
}

void ASChatacter::EndCrouch()
{
	UnCrouch();
}

void ASChatacter::Init_Jump()
{
	Jump();
}

void ASChatacter::BeginZoom()
{
	bWantsToZoom = true;
}

void ASChatacter::EndZoom()
{
	bWantsToZoom = false;
}

void ASChatacter::Reload()
{
	bWantToReload = true;
}

void ASChatacter::FoundAmmo(int amount)
{
	CurrentWeapon->AddAmmo(amount);
}

void ASChatacter::StartFire()
{
	if (CurrentWeapon && !bIsReloading) {
		PlayerNoiseEmitter->MakeNoise(this, 1.0f, GetActorLocation());
		CurrentWeapon->StartFire();
	}
}

void ASChatacter::StopFire()
{
	if (CurrentWeapon) {
		CurrentWeapon->StopFire();
	}
}

FString  ASChatacter::Ammo()
{
	FString Temp = FString::FromInt(CurrentWeapon->GetAmmoHeld());
	Temp += " / ";
	Temp+= FString::FromInt(CurrentWeapon->AmmoCheck());
	return Temp;
}

void ASChatacter::OnHealthChanged(USHealthComponent* HealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0.0f && !bDied) {
		
		//Die!
		bDied = true;
		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		DetachFromControllerPendingDestroy();

		SetLifeSpan(10.0f);

	}
}

void ASChatacter::ReloadEnd()
{
	//Seting Ammo back to Max 
	if (CurrentWeapon) {
		CurrentWeapon->Reloaded();
	}
	bIsReloading = false;
	bWantToReload = false;
	//Not Needed for non Looping TimerHandle ?
	GetWorldTimerManager().ClearTimer(ReloadHandle);
}

// Called every frame
void ASChatacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float TargetFOV = bWantsToZoom ? ZoomedFOV : DefaultFOV;
	float NewFOV = FMath::FInterpTo(CameraComp->FieldOfView, TargetFOV, DeltaTime, ZoomInterpSpeed);
	CameraComp->SetFieldOfView(NewFOV);
	if (bWantToReload && !bIsReloading && CurrentWeapon->GetAmmoHeld() >0) {
		bIsReloading = true;
		GetWorldTimerManager().SetTimer(ReloadHandle, this, &ASChatacter::ReloadEnd, CurrentWeapon->GetReloadTime(), false);
	}
}

// Called to bind functionality to input
void ASChatacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASChatacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASChatacter::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &ASChatacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &ASChatacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ASChatacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ASChatacter::EndCrouch);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASChatacter::Init_Jump);

	PlayerInputComponent->BindAction("ADS", IE_Pressed, this, &ASChatacter::BeginZoom);
	PlayerInputComponent->BindAction("ADS", IE_Released, this, &ASChatacter::EndZoom);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASChatacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ASChatacter::StopFire);

	PlayerInputComponent->BindAction("Reload", IE_Released, this, &ASChatacter::Reload);
}

FVector ASChatacter::GetPawnViewLocation() const
{
	if (CameraComp) {
		return CameraComp->GetComponentLocation();
	}
	return Super::GetPawnViewLocation();
}

void ASChatacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASChatacter, CurrentWeapon);
	DOREPLIFETIME(ASChatacter, bDied);
	
}