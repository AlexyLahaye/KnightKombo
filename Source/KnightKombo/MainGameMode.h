#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameMode.generated.h"

UCLASS()
class KNIGHTKOMBO_API AMainGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	// Référence au Widget ComboHUD créé
	UPROPERTY(BlueprintReadOnly, Category = "HUD")
	UUserWidget* ComboHUD;

public:

	AMainGameMode();

	// Override BeginPlay
	virtual void BeginPlay() override;


	// Permet d'obtenir une référence au HUD
	UUserWidget* GetComboHUD() const { return ComboHUD; }
	
private:
	void SetupPlayerCamera();
	void SetupPlayerCharacter();

	// Stocker le chemin du Blueprint du personnage
	UPROPERTY()
	TSubclassOf<APawn> PlayerCharacterClass;

	// Classe du Widget Blueprint du ComboHUD
	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<class UWB_ComboHUD> ComboHUDClass;
};
