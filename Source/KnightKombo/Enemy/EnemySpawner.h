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

	// Lance la vague d'ennemis
	void StartSpawning();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<AEnemyCharacter> EnemyCharacterClass;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	FVector SpawnPoint;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	FVector TargetPoint;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	int32 EnemyCount;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	float SpawnInterval;

private:
	// Liste des ennemis actifs
	TArray<AEnemyCharacter*> ActiveEnemies;
	
	FTimerHandle SpawnTimerHandle;

	void SpawnEnemy();
};
