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
	// TO DO:
	// - Muzzle Flash Particle Effect xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
	// - Delay między możliwymi strzałami
	// - przerobic wyciąganie pocisków na object pooling pattern
	// - sprawdzanie czy korzystamy z amunicji oraz wystrzelenie jesli mamy amunicje
	// - dzwięk wystrzału xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
	FVector Location = ProjectileSpawnPoint->GetComponentLocation();
	FRotator Rotation = ProjectileSpawnPoint->GetComponentRotation();
	FVector Direction = UKismetMathLibrary::GetForwardVector(Rotation);

	for (AProjectile* Projectile : RedProjectilePool)
	{
		if (Projectile->GetInUse()) { continue; }
		Projectile->ActivateProjectileAtLocation(Location, Rotation, Direction);

		UNiagaraFunctionLibrary::SpawnSystemAttached(RedMuzzleEffect, ProjectileSpawnPoint, NAME_None, FVector::ZeroVector, FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset, true);
		UGameplayStatics::SpawnSoundAttached(MuzzleSound, ProjectileSpawnPoint, NAME_None, FVector::ZeroVector, FRotator::ZeroRotator);
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
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	InitializeRedPool();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::InitializeRedPool()
{
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

