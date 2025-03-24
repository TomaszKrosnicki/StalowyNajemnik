// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"


void AEnemyAI::BeginPlay()
{
    Super::BeginPlay();

    APawn* AIPawn = GetPawn();
    AIPawn->bUseControllerRotationYaw = true;

    if(AIBehavior != nullptr)
    {
        RunBehaviorTree(AIBehavior);

        APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
        
        GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), AIPawn->GetActorLocation());
    }
    
}

void AEnemyAI::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    //MoveToActor(PlayerPawn, 200.0f);

    if (LineOfSightTo(PlayerPawn))
    {
        //SetFocus(PlayerPawn);
        //MoveToActor(PlayerPawn, AcceptanceRadius);
        GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
        GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), PlayerPawn->GetActorLocation());
    }
    else
    {
        //ClearFocus(EAIFocusPriority::Gameplay);
        //StopMovement();
        GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation"));
    }
    
}
