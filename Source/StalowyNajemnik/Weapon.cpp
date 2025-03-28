// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(Mesh);

}

void AWeapon::TriggerWeapon()
{
	if (bCanShoot == false) { return; }
	bCanShoot = false;
	GetWorldTimerManager().SetTimer(ShootingTimer, this, &AWeapon::ResetCanShoot, ShootingDelay);

	switch (CurrentEnergyType)
	{
	case EEnergyType::Red:
		ActivateProjectileFromPool(RedProjectilePool, RedMuzzleEffect);
		break;
	case EEnergyType::Green:
		ActivateProjectileFromPool(GreenProjectilePool, GreenMuzzleEffect);
		break;
	case EEnergyType::Blue:
		ActivateProjectileFromPool(BlueProjectilePool, BlueMuzzleEffect);
		break;
	default:
		break;
	}
}

void AWeapon::SwitchCurrentEnergyType(int Value)
{
	SwitchEnergyInt += Value;
	if (SwitchEnergyInt > 2) { SwitchEnergyInt = 0; }
	if (SwitchEnergyInt < 0) { SwitchEnergyInt = 2; }

	switch (SwitchEnergyInt)
	{
	case 0:
		CurrentEnergyType = EEnergyType::Red;
		break;
	case 1:
		CurrentEnergyType = EEnergyType::Green;
		break;;
	case 2:
		CurrentEnergyType = EEnergyType::Blue;
		break;
	default:
		break;
	}
	
	UGameplayStatics::SpawnSoundAttached(SwitchSound, ProjectileSpawnPoint, NAME_None, FVector::ZeroVector, FRotator::ZeroRotator);
}

bool AWeapon::GetCanShoot() const
{
	return bCanShoot;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	CurrentEnergyType = StartingEnergyType;

	InitializeRedPool();
	InitializeGreenPool();
	InitializeBluePool();
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::ResetCanShoot()
{
	bCanShoot = true;
	GetWorldTimerManager().ClearTimer(ShootingTimer);
}

void AWeapon::ActivateProjectileFromPool(TArray<AProjectile*>& PoolArray, UNiagaraSystem* MuzzleEffect)
{
	FVector Location = ProjectileSpawnPoint->GetComponentLocation();
	FRotator Rotation = ProjectileSpawnPoint->GetComponentRotation();
	FVector Direction = UKismetMathLibrary::GetForwardVector(Rotation);

	for (AProjectile* Projectile : PoolArray)
	{
		if (Projectile->GetInUse()) { continue; }
		Projectile->ActivateProjectileAtLocation(Location, Rotation, Direction);
		UNiagaraFunctionLibrary::SpawnSystemAttached(MuzzleEffect, ProjectileSpawnPoint, NAME_None, FVector::ZeroVector, FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset, true);
		UGameplayStatics::SpawnSoundAttached(MuzzleSound, ProjectileSpawnPoint, NAME_None, FVector::ZeroVector, FRotator::ZeroRotator);
		break;
	}
}

void AWeapon::InitializeRedPool()
{
	if (RedProjectileAmount <= 0) { return; }
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();

	for (int i = 0; i < RedProjectileAmount; i++)
	{
		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(RedProjectileClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		if (Projectile != nullptr)
		{
			RedProjectilePool.Add(Projectile);
		}
	}
}

void AWeapon::InitializeGreenPool()
{
	if (GreenProjectileAmount <= 0) { return; }
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();

	for (int i = 0; i < GreenProjectileAmount; i++)
	{
		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(GreenProjectileClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		if (Projectile != nullptr)
		{
			GreenProjectilePool.Add(Projectile);
		}
	}
}

void AWeapon::InitializeBluePool()
{
	if (BlueProjectileAmount <= 0) { return; }
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();

	for (int i = 0; i < BlueProjectileAmount; i++)
	{
		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(BlueProjectileClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		if (Projectile != nullptr)
		{
			BlueProjectilePool.Add(Projectile);
		}
	}
}

