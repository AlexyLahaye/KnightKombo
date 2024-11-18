#include "MainGameMode.h"
#include "CineCameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();

	SetupPlayerCamera();
}

void AMainGameMode::SetupPlayerCamera()
{
	// Récupérer le PlayerController
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController not found!"));
		return;
	}

	// Trouver la caméra avec le tag "MainCamera"
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("MainCamera"), FoundActors);

	if (FoundActors.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("No actors with tag 'MainCamera' found!"));
		return;
	}

	// Utiliser le premier acteur trouvé
	ACineCameraActor* MainCamera = Cast<ACineCameraActor>(FoundActors[0]);
	if (!MainCamera)
	{
		UE_LOG(LogTemp, Error, TEXT("Found actor is not a CineCameraActor!"));
		return;
	}

	// Définir la caméra comme vue principale
	PlayerController->SetViewTarget(MainCamera);
}
