#include "EnemySpawner.h"
#include "EnemyCharacter.h"
#include "Engine/World.h"

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	// Valeurs par défaut
	SpawnPoint = FVector(0.f, 0.f, 0.f);
	EnemyCount = 5;
}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemySpawner::SpawnEnemyWave()
{
	if (!EnemyClass) return;

	for (int32 i = 0; i < EnemyCount; ++i)
	{
		// Ajouter un décalage pour éviter que tous les ennemis spawnent au même endroit
		FVector Offset(i * 50.f, 0.f, 0.f); 
		FVector SpawnLocation = SpawnPoint + Offset;

		// Faire apparaître un ennemi
		GetWorld()->SpawnActor<AEnemyCharacter>(EnemyClass, SpawnLocation, FRotator::ZeroRotator);
	}
}
