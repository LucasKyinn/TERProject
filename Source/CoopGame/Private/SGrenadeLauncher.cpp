// Fill out your copyright notice in the Description page of Project Settings.


#include "SGrenadeLauncher.h"
#include "SGLProjectile.h"


void ASGrenadeLauncher::Fire()
{
	UE_LOG(LogTemp, Warning, TEXT("Fire()"));

	AActor* MyOwner = GetOwner();

	if (MyOwner && ProjectileClass) {
		FVector EyesLocation;
		FRotator EyesRotation;
		MyOwner->GetActorEyesViewPoint(EyesLocation, EyesRotation);

		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);


		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, EyesRotation, ActorSpawnParams);
	}


}