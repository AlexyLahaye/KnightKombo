#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyCharacter.h"
#include "EnemySpawner.generated.h"

UCLASS()
class KNIGHTKOMBO_API AEnemySpawner : public AActor
{
	GENERATED_BODY()

public:
	AEnemySpawner();

	// Démarre le processus de spawn
	void StartSpawning();

	// Supprime un ennemi et nettoie ses positions assignées
	void RemoveEnemy(AEnemyCharacter* Enemy);

protected:
	virtual void BeginPlay() override;

	// Gère le spawn d'ennemis
	void SpawnEnemy();

	// Calcule une position valide pour un nouvel ennemi
	FVector GetNextSpawnPosition(const FVector& BasePosition);

private:
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<AEnemyCharacter> EnemyCharacterClass;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	FVector TargetPoint;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	float SpawnInterval;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	int32 EnemyCount;

	UPROPERTY() 
	FVector SpawnPoint;

	UPROPERTY()
	TArray<AEnemyCharacter*> ActiveEnemies;

	FTimerHandle SpawnTimerHandle;

	// Distance minimale entre les ennemis
	float MinDistanceBetweenEnemies = 150.0f;

	// Liste des positions déjà assignées
	TArray<FVector> AssignedPositions;
};
