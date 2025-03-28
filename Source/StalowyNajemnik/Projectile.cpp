// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "ProjectileHitInterface.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MainCharacter.h"
#include "Components/CapsuleComponent.h"

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
	ProjectileMovementComponent->MaxSpeed = 3000.f;
	ProjectileMovementComponent->InitialSpeed = 3000.f;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	WeaponOwner = GetOwner();
	OwnerCharacter = Cast<AMainCharacter>(WeaponOwner);
	if (OwnerCharacter != nullptr)
	{
		OwnerCharacter->GetCapsuleComponent()->IgnoreActorWhenMoving(this, true);
		OwnerCharacter->GetCapsuleComponent()->MoveIgnoreActors.Add(this);
	}
	ProjectileCollider->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnProjectileBeginOverlap);
	IgnoreOwnerCollision();
	ProjectileMovementComponent->MaxSpeed = Speed;
	ProjectileMovementComponent->InitialSpeed = Speed;
	DeactivateProjectile();
}

void AProjectile::IgnoreOwnerCollision()
{
	if (WeaponOwner != nullptr)
	{
		SetOwner(WeaponOwner);
		ProjectileCollider->IgnoreActorWhenMoving(WeaponOwner, true);
		ProjectileCollider->MoveIgnoreActors.Add(WeaponOwner);
	}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float AProjectile::GetProjectileDamage() const
{
	return Damage;
}

EEnergyType AProjectile::GetProjectileEnergyType() const
{
	return ProjectileEnergyType;
}

void AProjectile::ActivateProjectileAtLocation(FVector SpawnLocation, FRotator SpawnRotation, FVector MovementDirection)
{
	bInUse = true;
	SetActorLocation(SpawnLocation);
	SetActorRotation(SpawnRotation);
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
	ProjectileMovementComponent->Velocity = MovementDirection * Speed;
	IgnoreOwnerCollision();
}

void AProjectile::DeactivateProjectile()
{
	bInUse = false;
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
	ProjectileMovementComponent->Velocity = FVector::ZeroVector;
	IgnoreOwnerCollision();
}

bool AProjectile::GetInUse() const
{
	return bInUse;
}

void AProjectile::OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr)
	{
		if (OtherActor->GetClass()->ImplementsInterface(UProjectileHitInterface::StaticClass()))
		{
			IProjectileHitInterface::Execute_OnProjectileHit(OtherActor, this, ProjectileEnergyType, Damage);
		}
	}

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactParticle, SweepResult.Location, FRotator::ZeroRotator);
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactSound, SweepResult.Location, FRotator::ZeroRotator);
	
	DeactivateProjectile();
}