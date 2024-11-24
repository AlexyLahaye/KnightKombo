#include "EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "NavigationSystem.h" // Pour le système de navigation

AEnemyAIController::AEnemyAIController()
{
}

void AEnemyAIController::BeginPlay()
{
    Super::BeginPlay();
}

void AEnemyAIController::MoveToTarget(const FVector& TargetLocation)
{
    if (!GetPawn())
    {
        UE_LOG(LogTemp, Error, TEXT("AIController has no pawn to control!"));
        return;
    }

    // Appel direct à la fonction MoveToLocation simplifiée
    MoveToLocation(TargetLocation, -1.f, true, true, false, true, 0, true);

    UE_LOG(LogTemp, Log, TEXT("AI is attempting to move to target location: %s"), *TargetLocation.ToString());
}
