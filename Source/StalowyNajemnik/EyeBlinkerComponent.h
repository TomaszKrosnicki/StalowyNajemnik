// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "EyeBlinkerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STALOWYNAJEMNIK_API UEyeBlinkerComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEyeBlinkerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* Mesh;
	UPROPERTY()
	class UMaterialInterface* Material;
	UPROPERTY()
	class UMaterialInstanceDynamic* DynamicMaterial;
	UPROPERTY()
	class UHealthComponent* HealthComp;

	UPROPERTY(EditAnywhere, Category = "ScreenMaterial")
	int MaterialIndex = 2;

	UPROPERTY(EditAnywhere, Category = "Eyes")
	float MinDelay = 3;

	UPROPERTY(EditAnywhere, Category = "Eyes")
	float MaxDelay = 6;

	UPROPERTY(EditAnywhere, Category = "Eyes")
	float TransitionDelay = 0.1;

	UPROPERTY(EditAnywhere, Category = "Eyes|Opened")
	class UTexture* EyeOpenColor;
	UPROPERTY(EditAnywhere, Category = "Eyes|Opened")
	class UTexture* EyeOpenEmission;

	UPROPERTY(EditAnywhere, Category = "Eyes|Transition")
	class UTexture* EyeTransitionColor;
	UPROPERTY(EditAnywhere, Category = "Eyes|Transition")
	class UTexture* EyeTransitionEmission;

	UPROPERTY(EditAnywhere, Category = "Eyes|Closed")
	class UTexture* EyeClosedColor;
	UPROPERTY(EditAnywhere, Category = "Eyes|Closed")
	class UTexture* EyeClosedEmission;

	UPROPERTY(EditAnywhere, Category = "Eyes|Dead")
	class UTexture* EyeDeadColor;
	UPROPERTY(EditAnywhere, Category = "Eyes|Dead")
	class UTexture* EyeDeadEmission;

	bool bIsClosing;
	FTimerHandle EyesTimerHandle;

	void WaitToClose();
	void Transition();
	void Closed();
	void Opened();
	void Dead();
	//bool bDeathTriggered = false;
};