#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "EnemyAIController.generated.h"

// Déclaration avancée
struct FPathFollowingResult;

UCLASS()
class KNIGHTKOMBO_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();

	// Lancer une animation d'attaque en boucle toutes les 2 secondes
	void StartAttackCycle();

	// Arrêter les attaques
	void StopAttackCycle();

protected:
	virtual void BeginPlay() override;

	virtual void BeginDestroy() override;

	virtual void OnUnPossess() override;

public:
	
	// Déplace l'ennemi vers une cible spécifique
	UFUNCTION(BlueprintCallable, Category = "AI")
	void MoveToTarget(const FVector& TargetLocation);

private:
	// Gérer la fin du déplacement
	void HandleMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result);

	// Timer pour gérer l'attaque en boucle
	FTimerHandle AttackTimerHandle;
};
