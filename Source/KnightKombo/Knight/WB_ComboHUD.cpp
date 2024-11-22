﻿#include "WB_ComboHUD.h"

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
