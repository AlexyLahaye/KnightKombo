#include "WB_ComboHUD.h"

#include "Kismet/GameplayStatics.h"
#include "KnightKombo/MainGameMode.h"

void UWB_ComboHUD::SetComboColors(const FLinearColor& FirstColor, const FLinearColor& SecondColor)
{
	if (FirstInputImage)
	{
		// Modifier la teinte de la première image
		FirstInputImage->SetBrushTintColor(FSlateColor(FirstColor));
	}

	if (SecondInputImage)
	{
		// Modifier la teinte de la deuxième image
		SecondInputImage->SetBrushTintColor(FSlateColor(SecondColor));
	}

	UE_LOG(LogTemp, Log, TEXT("Couleurs mises à jouuuuuur : FirstColor = %s, SecondColor = %s"),
		*FirstColor.ToString(), *SecondColor.ToString());
}

void UWB_ComboHUD::ResetColors()
{
	SetComboColors(FLinearColor::Transparent, FLinearColor::Transparent);
}

void UWB_ComboHUD::UpdateScore()
{
	if (!ScoreTxt) // Vérifiez que le TextBlock est valide
	{
		return;
	}

	// Incrémenter le score
	CurrentScore++;

	// Mettre à jour le texte du TextBlock
	FText NewText = FText::AsNumber(CurrentScore);
	ScoreTxt->SetText(NewText);
}

void UWB_ComboHUD::UpdateLife(int32 CurrentLives)
{
	// Chemin vers l'image d'un cœur vide
	static const FName EmptyHeartPath = TEXT("/Game/Sprite/Knight/EmptySingleHeart.EmptySingleHeart");
	UTexture2D* EmptyHeartTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *EmptyHeartPath.ToString()));

	if (!EmptyHeartTexture)
	{
		UE_LOG(LogTemp, Warning, TEXT("EmptyHeartTexture is not loaded. Check the path."));
		return;
	}

	// Mise à jour des cœurs
	if (Heart1)
	{
		if (CurrentLives < 3)
		{
			Heart1->SetBrushFromTexture(EmptyHeartTexture);
		}
	}

	if (Heart2)
	{
		if (CurrentLives < 2)
		{
			Heart2->SetBrushFromTexture(EmptyHeartTexture);
		}
	}

	if (Heart3)
	{
		if (CurrentLives < 1)
		{
			Heart3->SetBrushFromTexture(EmptyHeartTexture);
		}
	}
}




