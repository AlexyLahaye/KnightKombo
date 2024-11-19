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

	// Map pour les animations d'attaque (clé : couleur, valeur : animation)
	UPROPERTY(EditAnywhere, Category = "Animations")
	TMap<FString, UPaperFlipbook*> AttackAnimations;

	// Buffer pour stocker les entrées utilisateur
	TArray<FString> InputBuffer;

	// Taille maximale du buffer
	const int32 MaxBufferSize = 2;

	// Fonctions pour gérer les entrées
	void HandleUpInput();
	void HandleDownInput();
	void HandleRightInput();
	void HandleLeftInput();
	void HandleComboInput(const FString& Input);

	// Vérifie les combos et joue l'animation correspondante
	void CheckCombo();

	// Joue une animation d'attaque en fonction du nom (clé dans AttackAnimations)
	void PlayAttackAnimation(const FString& AttackName);

	// Retourne à l'animation Idle après une attaque
	void OnAnimationFinished();
};
