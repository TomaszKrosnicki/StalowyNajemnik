// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "ProjectileHitInterface.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Projectile Collider"));
	RootComponent = ProjectileCollider;

	ProjectileCollider->InitSphereRadius(20.0f);
	ProjectileCollider->SetGenerateOverlapEvents(true);
    ProjectileCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    ProjectileCollider->SetCollisionObjectType(ECC_WorldDynamic);
    ProjectileCollider->SetCollisionResponseToAllChannels(ECR_Overlap);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	ProjectileMovementComponent->MaxSpeed = 2000.f;
	ProjectileMovementComponent->InitialSpeed = 2000.f;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileCollider->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnProjectileBeginOverlap);
	IgnoreOwnerCollision();
}

void AProjectile::IgnoreOwnerCollision()
{
	AActor* OwnerActor = GetOwner();
	if (OwnerActor != nullptr)
	{
		ProjectileCollider->IgnoreActorWhenMoving(OwnerActor, true);
	}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float AProjectile::GetParticleDamage() const
{
	return Damage;
}

void AProjectile::OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr)
	{
		if (OtherActor->GetClass()->ImplementsInterface(UProjectileHitInterface::StaticClass()))
		{
			IProjectileHitInterface::Execute_OnProjectileHit(OtherActor, this);
		}
	}

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactParticle, SweepResult.Location, FRotator::ZeroRotator);
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactSound, SweepResult.Location, FRotator::ZeroRotator);
	
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
}