#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameFramework/PlayerController.h"
#include "EnemyAIController.generated.h"

UCLASS()
class KNIGHTKOMBO_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();

protected:
	virtual void BeginPlay() override;

public:
	// Déplace l'ennemi vers une cible spécifique
	UFUNCTION(BlueprintCallable, Category = "AI")
	void MoveToTarget(const FVector& TargetLocation);
};
