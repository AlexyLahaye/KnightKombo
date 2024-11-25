#include "EnemyAIController.h"
#include "EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"
#include "Navigation/PathFollowingComponent.h"

AEnemyAIController::AEnemyAIController()
{
}

void AEnemyAIController::BeginPlay()
{
    Super::BeginPlay();

    // Attache la fonction HandleMoveCompleted pour détecter la fin des déplacements
    if (GetPathFollowingComponent())
    {
        GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &AEnemyAIController::HandleMoveCompleted);
    }
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

void AEnemyAIController::HandleMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
    // Vérifie si le déplacement s'est terminé avec succès
    if (Result.Code == EPathFollowingResult::Success)
    {
        UE_LOG(LogTemp, Log, TEXT("AI has reached its target."));

        // Démarre les attaques après avoir atteint la destination
        StartAttackCycle();
    }
}

void AEnemyAIController::StartAttackCycle()
{
    AEnemyCharacter* ControlledEnemy = Cast<AEnemyCharacter>(GetPawn());
    if (!ControlledEnemy)
    {
        UE_LOG(LogTemp, Warning, TEXT("No controlled enemy found for AIController."));
        return;
    }

    // Lancer immédiatement la première attaque
    ControlledEnemy->PerformAttack();

    // Configurer le timer pour lancer les attaques toutes les X secondes
    GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, [this]()
    {
        // Vérifie si le pawn contrôlé existe toujours
        AEnemyCharacter* CurrentControlledEnemy = Cast<AEnemyCharacter>(GetPawn());
        if (CurrentControlledEnemy)
        {
            CurrentControlledEnemy->PerformAttack();
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Controlled enemy no longer exists, stopping attack cycle."));
            StopAttackCycle(); // Arrête le timer si l'ennemi n'existe plus
        }
    }, 1.0f, true); // Répéter toutes les 1 seconde
}



void AEnemyAIController::StopAttackCycle()
{
    // Arrêter le timer
    GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
}
