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
        
        GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), AIPawn->GetActorLocation());
    }
    
}

void AEnemyAI::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds); 
}
