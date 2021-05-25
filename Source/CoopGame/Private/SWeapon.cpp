// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeapon.h"
#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "../CoopGame.h"
#include "Sound/SoundCue.h"
#include "SChatacter.h"
#include "Chaos/ChaosEngineInterface.h"
#include"TimerManager.h"

static int32 DebugWeaponDrawing = 0;
FAutoConsoleVariableRef CVARDebugWeaponDrawing(
	TEXT("COOP.DebugWeapons"),
	DebugWeaponDrawing,
	TEXT("Draw Debug Lines for Weapons"),
	ECVF_Cheat);
// Sets default values
ASWeapon::ASWeapon()
{
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	MuzzleSocketName = "MuzzleSocket";
	TracerTargetName = "Target";

	BaseDamage = 50.0f;
	BulletSpread = 0.5f;
	RateOfFire = 125;

	MagSize = 7;
	CurrentAmo = 7;
	ReloadTime = 2.0f;
	AmmoHeld = 21;
	SetReplicates(true);

	NetUpdateFrequency = 66.0f;
	MinNetUpdateFrequency = 33.0f;
}

void ASWeapon::BeginPlay()
{
	Super::BeginPlay();

	TimeBetweenShots = 60/RateOfFire;
}


void ASWeapon::Fire()
{

	if (GetLocalRole() < ROLE_Authority) {
		ServerFire();
	}

	//Trace the world , froml pawn eyes to crosshair location
	
	AActor * MyOwner = GetOwner();

	if (MyOwner && CurrentAmo > 0 && !(Cast<ASChatacter>(MyOwner)->bDied)) {
		ASChatacter* Player = Cast<ASChatacter>(MyOwner);
		if (FireSound) {
			UGameplayStatics::SpawnSoundAttached(FireSound, RootComponent);
		}
		

		FVector EyesLocation;
		FRotator EyesRotation;
		MyOwner->GetActorEyesViewPoint(EyesLocation, EyesRotation);

		FVector ShotDirection = EyesRotation.Vector();

		float HalfRad = FMath::DegreesToRadians(BulletSpread);
		ShotDirection = FMath::VRandCone(ShotDirection, HalfRad, HalfRad);

		FVector TraceEnd = EyesLocation + (ShotDirection * 10000);

		FHitResult Hit;
		FCollisionQueryParams QueryParams; 
		QueryParams.AddIgnoredActor(MyOwner);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;
		QueryParams.bReturnPhysicalMaterial = true;
		//Particle Target Param
		FVector TracerEndPoint = TraceEnd;

		EPhysicalSurface SurfaceType = SurfaceType_Default;

		if (GetWorld()->LineTraceSingleByChannel(Hit, EyesLocation, TraceEnd, COLLISION_WEAPON, QueryParams)) {
			//BlockingHitProcessDamage
			AActor* HitActor = Hit.GetActor();

			SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
			float ActualDamage = BaseDamage;
			if (SurfaceType == SURFACE_FLESHVUNERABLE) {
				ActualDamage *= 4.0f;
			}

			UGameplayStatics::ApplyPointDamage(HitActor, ActualDamage, ShotDirection,  Hit, MyOwner->GetInstigatorController(), MyOwner, DamageType);
			CurrentAmo--;
			//UE_LOG(LogTemp, Log, TEXT("Ammo = %d"),CurrentAmo);
			PlayImpactEffects(SurfaceType, Hit.ImpactPoint);
			TracerEndPoint = Hit.ImpactPoint;
			
		}

		if (DebugWeaponDrawing>0) {
			DrawDebugLine(GetWorld(), EyesLocation, TraceEnd, FColor::Red, false, 10.0f, 0, 1.0f);
		}
		
		PlayFireEffects(TracerEndPoint);

		if (GetLocalRole() == ROLE_Authority) {
			HitScanTrace.TraceTo = TracerEndPoint;
			HitScanTrace.SurfaceType = SurfaceType;
		}
		
		LastTimeFired = GetWorld()->GetTimeSeconds();
	}

	if (CurrentAmo <= 0) {
		if (EmptyMagSound) {
			UGameplayStatics::SpawnSoundAttached(EmptyMagSound, RootComponent);
		}
	}

}

void ASWeapon::ServerFire_Implementation()
{
	Fire();
}

bool ASWeapon::ServerFire_Validate()
{
	return true;
}
void ASWeapon::OnRep_HitScanTrace()
{
	//replicate FX
	PlayFireEffects(HitScanTrace.TraceTo);
	PlayImpactEffects(HitScanTrace.SurfaceType,HitScanTrace.TraceTo);

}
void ASWeapon::Reloaded()
{

	//Sond
	if (ReloadingSound) {
		UGameplayStatics::SpawnSoundAttached(ReloadingSound, RootComponent);
	}
	
	if (AmmoHeld> MagSize) {
		CurrentAmo = MagSize;
		AmmoHeld -= MagSize; 
	}
	else {
		CurrentAmo = AmmoHeld;
		AmmoHeld -= AmmoHeld;
	}
	
}


void ASWeapon::StartFire()
{
	float FirstDelay = FMath::Max(LastTimeFired + TimeBetweenShots - GetWorld()->GetTimeSeconds(),0.0f);
	GetWorldTimerManager().SetTimer(TimerHandle_TimeBetweenShots,this,&ASWeapon::Fire, TimeBetweenShots,true,FirstDelay);
}

void ASWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_TimeBetweenShots);
}

int ASWeapon::AmmoCheck()
{
	return CurrentAmo;
}

int ASWeapon::GetAmmoHeld()
{
	return AmmoHeld;
}

void ASWeapon::AddAmmo(int Amount)
{
	AmmoHeld += Amount;
}

float ASWeapon::GetReloadTime()
{
	return ReloadTime;
}


void ASWeapon::PlayFireEffects(FVector TracerEndPoint)
{
	if (MuzzleEffect) {
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);
	}

	if (TracerEffect) {
		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
		UParticleSystemComponent* TracerComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, MuzzleLocation);

		if (TracerComp) {
			TracerComp->SetVectorParameter(TracerTargetName, TracerEndPoint);
		}
	}

	APawn* MyOwner = Cast<APawn>(GetOwner());
	if (MyOwner) {
		APlayerController* PC = Cast<APlayerController>(MyOwner->GetController());

		if (PC) {
			PC->ClientStartCameraShake(FireCamShake);
		}
	}


}

void ASWeapon::PlayImpactEffects(EPhysicalSurface SurfaceType,FVector ImpactPoint)
{
	UParticleSystem* SelectedEffect = nullptr;
	switch (SurfaceType) {
	case SURFACE_FLESHDEFAULT:
	case SURFACE_FLESHVUNERABLE:
		SelectedEffect = FleshImpactEffect;
		break;
	default:
		SelectedEffect = DefaultImpactEffect;
		break;
	}
	if (SelectedEffect) {
		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
		FVector ShotDirectoin = ImpactPoint - MuzzleLocation;
		ShotDirectoin.Normalize();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedEffect, ImpactPoint, ShotDirectoin.Rotation());
	}
}

void ASWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ASWeapon, HitScanTrace,COND_SkipOwner);
}
