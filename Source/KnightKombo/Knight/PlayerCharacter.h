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
	virtual void BeginPlay() override;
	
	APlayerCharacter();

	void HandleComboInput(const FString& Input);

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY()
	UUserWidget* ComboHUD;

	// Référence au ComboHUD spécifique
	UPROPERTY()
	UWB_ComboHUD* ComboHUDInstance;

private:
	// Animations
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UPaperFlipbookComponent* FlipbookComponent;

	UPROPERTY(EditAnywhere, Category = "Animations")
	class UPaperFlipbook* IdleAnimation;

	UPROPERTY(EditAnywhere, Category = "Animations")
	TMap<FString, UPaperFlipbook*> AttackAnimations;

	// Combos
	TArray<FString> InputBuffer;
	FString ComboInputs[2]; // Stocke les deux dernières touches pressées
	int32 ComboIndex = 0;
	const int32 MaxBufferSize = 2;

	// Gestion de l'update de l'HUD
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UUserWidget> ComboHUDClass;
	
	// Timer pour réinitialiser le HUD
	FTimerHandle ResetTimerHandle;

	// Convertit une chaîne en couleur
	FLinearColor GetColorFromInput(const FString& Input) const;

	// Réinitialise le HUD
	void ResetHUD();

	// Gestion des entrées et animations
	void HandleUpInput();
	void HandleDownInput();
	void HandleRightInput();
	void HandleLeftInput();
	void CheckCombo();
	void PlayAttackAnimation(const FString& AttackName);
	void OnAnimationFinished();
};
