#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "WB_ComboHUD.generated.h"

UCLASS()
class KNIGHTKOMBO_API UWB_ComboHUD : public UUserWidget
{
	GENERATED_BODY()

protected:
	// Référence aux images du widget
	UPROPERTY(meta = (BindWidget))
	UImage* FirstInputImage;

	UPROPERTY(meta = (BindWidget))
	UImage* SecondInputImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ScoreTxt;

	int32 CurrentScore = 0;

public:
	// Met à jour les couleurs des images
	void SetComboColors(const FLinearColor& FirstColor, const FLinearColor& SecondColor);

	// Réinitialise les couleurs
	void ResetColors();

	void UpdateScore();
};
