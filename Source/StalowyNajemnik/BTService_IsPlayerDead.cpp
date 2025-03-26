// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_IsPlayerDead.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "HealthComponent.h"

UBTService_IsPlayerDead::UBTService_IsPlayerDead()
{
    NodeName = TEXT("Update Is Player Dead");
}

void UBTService_IsPlayerDead::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (PlayerPawn == nullptr) { return; }

    UHealthComponent* PlayerHealthComp = PlayerPawn->FindComponentByClass<UHealthComponent>();
    if (PlayerHealthComp == nullptr) { return; }

    if (PlayerHealthComp->IsDead())
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsInt(GetSelectedBlackboardKey(), 1);
    }
    else
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsInt(GetSelectedBlackboardKey(), 0);
    }
    
}