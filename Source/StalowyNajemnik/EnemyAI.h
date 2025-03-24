// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAI.generated.h"

/**
 * 
 */
UCLASS()
class STALOWYNAJEMNIK_API AEnemyAI : public AAIController
{
	GENERATED_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaSeconds) override;

private:
	//UPROPERTY(EditAnywhere)
	//float AcceptanceRadius = 200.0f;

	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior;
	
};
