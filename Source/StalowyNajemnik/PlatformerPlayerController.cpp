// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformerPlayerController.h"
#include "Blueprint/UserWidget.h"


void APlatformerPlayerController::BeginPlay()
{
    Super::BeginPlay();

    CrosshairWidget = CreateWidget(this, Crosshair);
    RemoveCrosshair();

    class UUserWidget* HUDWidget = CreateWidget(this, HUD);
    HUDWidget->AddToViewport();
}

void APlatformerPlayerController::CreateCrosshair()
{
    CrosshairWidget->AddToViewport();
}

void APlatformerPlayerController::RemoveCrosshair()
{
    CrosshairWidget->RemoveFromViewport();
}
