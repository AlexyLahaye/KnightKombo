#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverHUD.generated.h"

UCLASS()
class KNIGHTKOMBO_API UGameOverHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	// Fonction pour relancer la partie
	UFUNCTION(BlueprintCallable, Category = "GameOver")
	void RestartGame();

	// Fonction pour quitter le jeu
	UFUNCTION(BlueprintCallable, Category = "GameOver")
	void QuitGame();
};
