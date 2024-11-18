#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class KNIGHTKOMBO_API APlayerCharacter : public APaperCharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	// Composant PaperFlipbook pour gérer les animations
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UPaperFlipbookComponent* FlipbookComponent;

	// Animations Flipbook
	UPROPERTY(EditAnywhere, Category = "Animations")
	class UPaperFlipbook* IdleAnimation;

	UPROPERTY(EditAnywhere, Category = "Animations")
	class UPaperFlipbook* UpAttackAnimation;

	// Gestion de l'animation en cours
	void PlayUpAttackAnimation();
	void OnAnimationFinished();
	void HandleSpaceBar();
};
