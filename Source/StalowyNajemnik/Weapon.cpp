// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

}

void AWeapon::TriggerWeapon()
{
	FString WeaponName = GetName();
	UE_LOG(LogTemp, Warning, TEXT("Shooting from: %s"), *WeaponName);
	FText DisplayEnumValue;
	UEnum::GetDisplayValueAsText(CurrentEnergyType, DisplayEnumValue);
	UE_LOG(LogTemp, Warning, TEXT("Current Energy Type: %s"), *DisplayEnumValue.ToString());
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
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

