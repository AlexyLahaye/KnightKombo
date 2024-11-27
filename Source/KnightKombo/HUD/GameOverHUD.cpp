#include "GameOverHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "../Enemy/EnemyCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"

void UGameOverHUD::RestartGame()
{
	// Nettoyer les références aux ennemis
	TArray<AActor*> AllEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyCharacter::StaticClass(), AllEnemies);
	for (AActor* Enemy : AllEnemies)
	{
		Enemy->Destroy();
	}
	
	UWorld* World = GetWorld();
	if (World)
	{
		// Redémarre le niveau actuel
		UGameplayStatics::OpenLevel(World, FName(*World->GetName()), true); // Recharge le niveau actuel

		// Désactive la souris après redémarrage du jeu
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
		if (PlayerController)
		{
			PlayerController->bShowMouseCursor = false;  // Masquer le curseur
			PlayerController->SetInputMode(FInputModeGameOnly());  // Retour au contrôle du jeu normal
		}
	}
}

void UGameOverHUD::QuitGame()
{
	// Ferme l'application
	if (GEngine)
	{
		GEngine->GameViewport->Viewport->Invalidate();
		FGenericPlatformMisc::RequestExit(true); // Force la fermeture du jeu
	}
}
