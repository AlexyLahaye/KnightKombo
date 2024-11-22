#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Enemy/EnemySpawner.h"
#include "MainGameMode.generated.h"

UCLASS()
class KNIGHTKOMBO_API AMainGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMainGameMode();

	// Accès au HUD
	UFUNCTION(BlueprintCallable, Category = "HUD")
	UUserWidget* GetComboHUD() const { return ComboHUDInstance; }

protected:
	virtual void BeginPlay() override;

private:
	// Configurer la caméra pour suivre un acteur spécifique
	void SetupPlayerCamera();

	// Lancer la vague d'ennemis
	void StartEnemyWave();

	// Référence à la classe Blueprint du personnage joueur
	UPROPERTY()
	TSubclassOf<APawn> PlayerCharacterClass;

	// Référence au HUD Combo
	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<class UUserWidget> ComboHUDClass;

	UPROPERTY()
	UUserWidget* ComboHUDInstance;

	// Référence à la classe Blueprint du spawner d'ennemis
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<AEnemySpawner> EnemySpawnerClass;

	// Référence à la classe Blueprint du spawner d'ennemis
	UPROPERTY(EditAnywhere, Category = "Enemy")
	TSubclassOf<AEnemyCharacter> EnemyCharacterClass;

	UPROPERTY()
	AEnemySpawner* EnemySpawnerInstance;
};
