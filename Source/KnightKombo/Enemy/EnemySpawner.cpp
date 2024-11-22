#include "EnemySpawner.h"
#include "TimerManager.h"

AEnemySpawner::AEnemySpawner()
{
	static ConstructorHelpers::FClassFinder<AEnemyCharacter> EnemyBPClass(TEXT("/Game/Blueprints/BP_EnemyCharacter"));
	if (EnemyBPClass.Class != nullptr)
	{
		EnemyCharacterClass = EnemyBPClass.Class;
		UE_LOG(LogTemp, Log, TEXT("BP_EnemyCharacter successfully loaded in Spawner."));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find BP_EnemyCharacter."));
	}
	
	PrimaryActorTick.bCanEverTick = false;

	TargetPoint = FVector(-1693, -1200, 1760);
	SpawnInterval = 1.0f;
	EnemyCount = 5;
}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	if (!EnemyCharacterClass)
	{
		UE_LOG(LogTemp, Error, TEXT("EnemyCharacterClass is null in BeginPlay! Check the setup."));
	}
}

void AEnemySpawner::StartSpawning()
{
	if (!EnemyCharacterClass) return;

	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AEnemySpawner::SpawnEnemy, SpawnInterval, true);
}

void AEnemySpawner::SpawnEnemy()
{
	UE_LOG(LogTemp, Log, TEXT("SpawnEnemy() called."));

	if (EnemyCount <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyCount is 0 or less. Stopping spawn."));
		// Arrête le timer quand tous les ennemis sont apparus
		GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
		return;
	}

	if (!EnemyCharacterClass)
	{
		UE_LOG(LogTemp, Error, TEXT("EnemyCharacterClass is not set! Cannot spawn enemy."));
		return;
	}

	// Spawne un ennemi
	AEnemyCharacter* NewEnemy = GetWorld()->SpawnActor<AEnemyCharacter>(EnemyCharacterClass, SpawnPoint, FRotator::ZeroRotator);
	if (NewEnemy)
	{
		UE_LOG(LogTemp, Log, TEXT("Enemy spawned successfully at location: %s"), *SpawnPoint.ToString());
		ActiveEnemies.Add(NewEnemy);

		// Déplace l'ennemi vers la position cible
		NewEnemy->MoveToLocation(TargetPoint);
		UE_LOG(LogTemp, Log, TEXT("Enemy moving to target point: %s"), *TargetPoint.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn enemy at location: %s"), *SpawnPoint.ToString());
	}

	EnemyCount--;
	UE_LOG(LogTemp, Log, TEXT("Enemies left to spawn: %d"), EnemyCount);
}

