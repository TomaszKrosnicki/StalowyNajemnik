// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerLocationIfSeen.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "AIController.h"


UBTService_PlayerLocationIfSeen::UBTService_PlayerLocationIfSeen()
{
    NodeName = TEXT("Update Player Location If Seen");
}

void UBTService_PlayerLocationIfSeen::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    
    if (PlayerPawn == nullptr)
    {
        return;
    }

    if (OwnerComp.GetAIOwner() == nullptr)
    {
        return;
    }

    if (OwnerComp.GetAIOwner()->LineOfSightTo(PlayerPawn))
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), PlayerPawn);
        OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("IsPlayerSeen"), true);
        AActor* Enemy = OwnerComp.GetOwner();
        if (FVector::Distance(Enemy->GetActorLocation(), PlayerPawn->GetActorLocation()) <= 600.0f)
        {
            OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("IsPlayerInRange"), true);
        }
        else
        {
            OwnerComp.GetBlackboardComponent()->ClearValue(TEXT("IsPlayerInRange"));
        }
    }
    else
    {
        OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
        OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("IsPlayerSeen"), false);
        OwnerComp.GetBlackboardComponent()->ClearValue(TEXT("IsPlayerInRange"));
    }
    
}
