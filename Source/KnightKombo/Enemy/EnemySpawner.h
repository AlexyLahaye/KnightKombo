#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

UCLASS()
class KNIGHTKOMBO_API AEnemySpawner : public AActor
{
	GENERATED_BODY()

public:
	AEnemySpawner();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SpawnEnemyWave();

	// Classe d'ennemi à faire apparaître
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<class AEnemyCharacter> EnemyClass;

	// Position d'apparition
	UPROPERTY(EditAnywhere, Category = "Spawning")
	FVector SpawnPoint;

	// Nombre d'ennemis par vague
	UPROPERTY(EditAnywhere, Category = "Spawning")
	int32 EnemyCount;
};
