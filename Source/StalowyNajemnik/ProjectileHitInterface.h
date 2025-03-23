// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ProjectileHitInterface.generated.h"

class AProjectile;

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UProjectileHitInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class STALOWYNAJEMNIK_API IProjectileHitInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, Category = Projectile)
	void OnProjectileHit(AProjectile* Projectile);
};
