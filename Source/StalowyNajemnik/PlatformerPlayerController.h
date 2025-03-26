// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlatformerPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class STALOWYNAJEMNIK_API APlatformerPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> Crosshair;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> HUD;

	UPROPERTY()
	class UUserWidget* CrosshairWidget;

public:
	UFUNCTION(BlueprintCallable)
	void CreateCrosshair();

	UFUNCTION(BlueprintCallable)
	void RemoveCrosshair();
	
};
