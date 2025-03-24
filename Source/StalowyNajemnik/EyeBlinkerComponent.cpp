// Fill out your copyright notice in the Description page of Project Settings.


#include "EyeBlinkerComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "HealthComponent.h"
#include "Engine/Texture.h"

// Sets default values for this component's properties
UEyeBlinkerComponent::UEyeBlinkerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEyeBlinkerComponent::BeginPlay()
{
	Super::BeginPlay();

	Mesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
	HealthComp = GetOwner()->FindComponentByClass<UHealthComponent>();

	if(Mesh == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Mesh not found!"));
	}

	Material = Mesh->GetMaterial(MaterialIndex);

	DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
	Mesh->SetMaterial(MaterialIndex, DynamicMaterial);
	bIsClosing = true;
	//bDeathTriggered = false;
	WaitToClose();
	
}

void UEyeBlinkerComponent::WaitToClose()
{
	float Delay = FMath::RandRange(MinDelay, MaxDelay);
	GetWorld()->GetTimerManager().SetTimer(EyesTimerHandle, this, &UEyeBlinkerComponent::Transition, Delay, false);
}

void UEyeBlinkerComponent::Transition()
{
	GetWorld()->GetTimerManager().ClearTimer(EyesTimerHandle);
	DynamicMaterial->SetTextureParameterValue(TEXT("Color"), EyeTransitionColor);
	DynamicMaterial->SetTextureParameterValue(TEXT("Emission"), EyeTransitionEmission);
	if(bIsClosing)
	{
		GetWorld()->GetTimerManager().SetTimer(EyesTimerHandle, this, &UEyeBlinkerComponent::Closed, TransitionDelay, false);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(EyesTimerHandle, this, &UEyeBlinkerComponent::Opened, TransitionDelay, false);
	}
}

void UEyeBlinkerComponent::Closed()
{
	GetWorld()->GetTimerManager().ClearTimer(EyesTimerHandle);
	DynamicMaterial->SetTextureParameterValue(TEXT("Color"), EyeClosedColor);
	DynamicMaterial->SetTextureParameterValue(TEXT("Emission"), EyeClosedEmission);
	bIsClosing = false;
	GetWorld()->GetTimerManager().SetTimer(EyesTimerHandle, this, &UEyeBlinkerComponent::Transition, TransitionDelay, false);
}

void UEyeBlinkerComponent::Opened()
{
	GetWorld()->GetTimerManager().ClearTimer(EyesTimerHandle);
	DynamicMaterial->SetTextureParameterValue(TEXT("Color"), EyeOpenColor);
	DynamicMaterial->SetTextureParameterValue(TEXT("Emission"), EyeOpenEmission);
	bIsClosing = true;
	WaitToClose();
}

void UEyeBlinkerComponent::Dead()
{
	GetWorld()->GetTimerManager().ClearTimer(EyesTimerHandle);
	DynamicMaterial->SetTextureParameterValue(TEXT("Color"), EyeDeadColor);
	DynamicMaterial->SetTextureParameterValue(TEXT("Emission"), EyeDeadEmission);
	//bDeathTriggered = true;
}

// Called every frame
void UEyeBlinkerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/* if(bDeathTriggered) { return; }
	if(HealthComp->IsDead())
	{
		Dead();
	} */
}