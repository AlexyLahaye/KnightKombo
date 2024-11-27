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

	// Fonction pour basculer le menu de pause
	void TogglePauseMenu();

	// Accès au HUD ComboHud
	UFUNCTION(BlueprintCallable, Category = "HUD")
	UUserWidget* GetComboHUD() const { return ComboHUDInstance; }

	// Obtenir le nombre actuel de vies
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	int32 GetPlayerLives() const { return PlayerLives; }

	// Réduire les vies du joueur
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void DecreasePlayerLives();

	// Accès au HUD GameOver
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class UGameOverHUD> GameOverHUDClass;

	// Vérifie si le joueur est en Game Over
	UFUNCTION(BlueprintCallable, Category = "Game")
	void CheckGameOver();
	

protected:
	virtual void BeginPlay() override;

	// Nombre de vies du joueur
	UPROPERTY(BlueprintReadWrite, Category = "Gameplay")
	int32 PlayerLives = 3;

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

	// Classe du HUD PauseMenu
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UUserWidget> PauseMenuHUDClass;

	// Instance du HUD PauseMenu
	UPROPERTY()
	UUserWidget* PauseMenuHUDInstance;
	
};
