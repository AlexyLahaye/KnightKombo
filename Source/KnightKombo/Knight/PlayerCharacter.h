#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "WB_ComboHUD.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class KNIGHTKOMBO_API APlayerCharacter : public APaperCharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	virtual void BeginPlay() override;

	// Gère les entrées des combos
	void HandleComboInput(const FString& Input);

	UPROPERTY()
	UWB_ComboHUD* ComboHUDInstance;

	void HandlePausePressed();

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	// Composants visuels
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UPaperFlipbookComponent* FlipbookComponent;

	// Animations
	UPROPERTY(EditAnywhere, Category = "Animations")
	class UPaperFlipbook* IdleAnimation;

	void PlaySound();

	UPROPERTY(EditAnywhere, Category = "Animations")
	TMap<FString, UPaperFlipbook*> AttackAnimations;

	// Gestion des combos
	TArray<FString> InputBuffer;
	FString ComboInputs[2];
	int32 ComboIndex = 0;
	const int32 MaxBufferSize = 2;

	// Couleur de l'attaque actuelle
	FString CurrentAttackColor;
	FString FirstInput;
	FString SecondInput;

	// HUD
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UUserWidget> ComboHUDClass;

	FTimerHandle ResetTimerHandle;

	// Collision pour les attaques
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* HitBox;

	// Méthodes auxiliaires
	FLinearColor GetColorFromInput(const FString& Input) const;
	void ResetHUD();
	void HandleUpInput();
	void HandleDownInput();
	void HandleRightInput();
	void HandleLeftInput();
	void CheckCombo();
	void PlayAttackAnimation(const FString& AttackName);
	void OnAnimationFinished();
	void HandleDamageToEnemies(); // Gestion de l'attaque.

	// Changer l'animation actuelle
	void SetAnimation(UPaperFlipbook* NewAnimation);
};
