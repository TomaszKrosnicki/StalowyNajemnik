// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraFunctionLibrary.h"
#include "Weapon.generated.h"

class AProjectile;

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
	UFUNCTION(BlueprintPure)
	bool GetCanShoot() const;

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

	UPROPERTY(EditDefaultsOnly, Category = "Projectile | Starting")
	EEnergyType StartingEnergyType = EEnergyType::Red;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile | Red")
	TSubclassOf<AProjectile> RedProjectileClass;
	UPROPERTY(EditDefaultsOnly, Category = "Projectile | Green")
	TSubclassOf<AProjectile> GreenProjectileClass;
	UPROPERTY(EditDefaultsOnly, Category = "Projectile | Blue")
	TSubclassOf<AProjectile> BlueProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile | Red")
	int RedProjectileAmount = 10;
	UPROPERTY(EditDefaultsOnly, Category = "Projectile | Green")
	int GreenProjectileAmount = 10;
	UPROPERTY(EditDefaultsOnly, Category = "Projectile | Blue")
	int BlueProjectileAmount = 10;

	UPROPERTY(EditDefaultsOnly, Category = "Muzzle | Red")
	UNiagaraSystem* RedMuzzleEffect;
	UPROPERTY(EditDefaultsOnly, Category = "Muzzle | Green")
	UNiagaraSystem* GreenMuzzleEffect;
	UPROPERTY(EditDefaultsOnly, Category = "Muzzle | Blue")
	UNiagaraSystem* BlueMuzzleEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Muzzle | Sound")
	USoundBase* MuzzleSound;

	UPROPERTY(EditDefaultsOnly, Category = "Switch Sound")
	USoundBase* SwitchSound;

	UPROPERTY(EditAnywhere, Category = "Projectile | Red | Pool")
	TArray<AProjectile*> RedProjectilePool;
	UPROPERTY(EditAnywhere, Category = "Projectile | Green | Pool")
	TArray<AProjectile*> GreenProjectilePool;
	UPROPERTY(EditAnywhere, Category = "Projectile | Blue | Pool")
	TArray<AProjectile*> BlueProjectilePool;

	UPROPERTY(EditAnywhere)
	float ShootingDelay = 0.5f;

	FTimerHandle ShootingTimer;

	bool bCanShoot = true;

	int SwitchEnergyInt = 0;

	void ResetCanShoot();

	void ActivateProjectileFromPool(TArray<AProjectile*>& PoolArray, UNiagaraSystem* MuzzleEffect);

	void InitializeRedPool();
	void InitializeGreenPool();
	void InitializeBluePool();


};
