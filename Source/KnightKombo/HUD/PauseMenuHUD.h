#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuHUD.generated.h"

UCLASS()
class KNIGHTKOMBO_API UPauseMenuHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	// Fonction pour reprendre la partie
	UFUNCTION(BlueprintCallable, Category = "PauseMenu")
	void ResumeGame();
	
};
