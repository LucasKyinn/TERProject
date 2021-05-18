// Fill out your copyright notice in the Description page of Project Settings.


#include "SRedBarrel.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ASRedBarrel::ASRedBarrel()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetSimulatePhysics(true);
	RootComponent = MeshComp;
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Radius = 500.0f;
	Dmg = 75.0f;
	CurrentHealth = 100.0f;

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
	RadialForceComp->SetupAttachment(RootComponent);
	RadialForceComp->Radius = Radius;
	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->bAutoActivate = false;
	RadialForceComp->bIgnoreOwningActor = true;
	ExplosionImpulse = 400.0f;
}

// Called when the game starts or when spawned
void ASRedBarrel::BeginPlay()
{
	Super::BeginPlay();
	this->OnTakeAnyDamage.AddDynamic(this, &ASRedBarrel::HandleTakeAnyDamage);	
}

void ASRedBarrel::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.f || bDead) {
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("Health Changed: %s"), *FString::SanitizeFloat(CurrentHealth));
	CurrentHealth = FMath:: Clamp(CurrentHealth - Damage,0.0f,100.0f);
	bDead = CurrentHealth <= 0.0f;
}

void ASRedBarrel::Explode()
{
	//Todo : : Animation 
	if (ExplsionEffect) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplsionEffect, GetActorLocation());
	}
	if (SecondMat) {
		MeshComp->SetMaterial(0, SecondMat);
	}
	

	TArray<AActor*> IgnoredActors;
	UGameplayStatics::ApplyRadialDamage(GetWorld(), Dmg, GetActorLocation(), Radius,UDamageType::StaticClass(), IgnoredActors, this, nullptr, false, ECollisionChannel::ECC_Visibility);
	RadialForceComp->FireImpulse();
}

// Called every frame
void ASRedBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bDead && !bExploding) {
		bExploding = true;
		Explode();
	}
}

