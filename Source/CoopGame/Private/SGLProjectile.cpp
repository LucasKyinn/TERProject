// Fill out your copyright notice in the Description page of Project Settings.


#include "SGLProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "TimerManager.h"
#include "Engine/EngineTypes.h"

// Sets default values
ASGLProjectile::ASGLProjectile()
{
	UE_LOG(LogTemp, Warning, TEXT("Creation proj ?"));


	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(3.0f);
	CollisionComp->SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ASGLProjectile::OnHit);

	RootComponent = CollisionComp;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 2000.0f;
	ProjectileMovement->MaxSpeed = 2000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	InitialLifeSpan = 3.0f;
}


void ASGLProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) 
{
	UE_LOG(LogTemp, Warning, TEXT("OnHit"));
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &ASGLProjectile::DelayedExplosion, 1.0f, false, 1.0f);
	
}

void ASGLProjectile::DelayedExplosion()
{
	UE_LOG(LogTemp, Warning, TEXT("Destroy()"));
	Destroy();
}
