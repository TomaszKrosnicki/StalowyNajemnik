// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraFunctionLibrary.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EEnergyType : uint8
{
	Red,
	Green,
	Blue,
	Count UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EEnergyType, EEnergyType::Count);

UCLASS()
class STALOWYNAJEMNIK_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	void TriggerWeapon();
	void SwitchCurrentEnergyType(int Value);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Projectile",meta = (AllowPrivateAccess = "true"))
	EEnergyType CurrentEnergyType = EEnergyType::Red;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Muzzle")
	UNiagaraSystem* MuzzleEffect;

	int SwitchEnergyInt = 0;
	

};
