#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameMode.generated.h"

UCLASS()
class KNIGHTKOMBO_API AMainGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMainGameMode();

	// Override BeginPlay
	virtual void BeginPlay() override;

private:
	void SetupPlayerCamera();
	void SetupPlayerCharacter();

	// Stocker le chemin du Blueprint du personnage
	UPROPERTY()
	TSubclassOf<APawn> PlayerCharacterClass;
};
