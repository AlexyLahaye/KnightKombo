// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenuHUD.h"

#include "Kismet/GameplayStatics.h"

void UPauseMenuHUD::ResumeGame()
{
	UWorld* World = GetWorld();
	if (World)
	{
		// Retirer le menu de pause
		RemoveFromParent();

		// Désactiver la pause
		UGameplayStatics::SetGamePaused(World, false);

		// Réactiver l'entrée du jeu
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
		if (PlayerController)
		{
			PlayerController->bShowMouseCursor = false;  // Masquer le curseur
			PlayerController->SetInputMode(FInputModeGameOnly());  // Retour au contrôle du jeu
		}
	}
}
