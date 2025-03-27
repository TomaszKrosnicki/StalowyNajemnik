// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.h"
#include "Components/SphereComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Projectile.generated.h"

UCLASS()
class STALOWYNAJEMNIK_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void IgnoreOwnerCollision();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Sphere Collider")
	USphereComponent* ProjectileCollider;

	UPROPERTY(EditDefaultsOnly, Category = "EnergyType")
	EEnergyType ProjectileEnergyType;

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, Category = "Impact")
	UNiagaraSystem* ImpactParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Impact")
	USoundBase* ImpactSound;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float Damage = 20.0f;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float Speed = 3000.0f;

	UPROPERTY(VisibleAnywhere)
	bool bInUse = false;

	UFUNCTION()
	void OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	float GetProjectileDamage() const;

	UFUNCTION(BlueprintCallable)
	EEnergyType GetProjectileEnergyType() const;

	UFUNCTION(BlueprintCallable)
	void ActivateProjectileAtLocation(FVector SpawnLocation, FRotator SpawnRotation, FVector MovementDirection);

	UFUNCTION(BlueprintCallable)
	void DeactivateProjectile();

	UFUNCTION(BlueprintCallable)
	bool GetInUse() const;
};
