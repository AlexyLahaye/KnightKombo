#include "MainGameMode.h"
#include "CineCameraActor.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "UObject/ConstructorHelpers.h"
#include "Knight/WB_ComboHUD.h"
#include "Enemy/EnemyCharacter.h"

AMainGameMode::AMainGameMode()
{
	// Charger le Blueprint du personnage joueur
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_PlayerCharactere"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
		PlayerCharacterClass = PlayerPawnBPClass.Class;
		UE_LOG(LogTemp, Log, TEXT("PlayerCharacter set successfully to BP_PlayerCharacter."));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Could not find BP_PlayerCharacter at the specified path."));
	}
	
	// Charger la classe Blueprint pour le ComboHUD
	static ConstructorHelpers::FClassFinder<UWB_ComboHUD> ComboHUD_BP(TEXT("/Game/Blueprints/WB_ComboHUD"));
	if (ComboHUD_BP.Class != nullptr)
	{
		ComboHUDClass = ComboHUD_BP.Class;
		UE_LOG(LogTemp, Log, TEXT("ComboHUDClass set successfully to WB_ComboHUD."));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Could not find WB_ComboHUD at the specified path."));
	}
	
}

void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();

	SetupPlayerCamera();
	SetupPlayerCharacter();

	if (ComboHUDClass)
	{
		ComboHUD = CreateWidget<UWB_ComboHUD>(GetWorld(), ComboHUDClass);
		if (ComboHUD)
		{
			ComboHUD->AddToViewport();
			UE_LOG(LogTemp, Warning, TEXT("HUD créé avec succès !"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Échec de création du HUD !"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ComboHUDClass n'est pas défini !"));
	}
}

void AMainGameMode::SetupPlayerCamera()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController not found!"));
		return;
	}

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("MainCamera"), FoundActors);

	if (FoundActors.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("No actors with tag 'MainCamera' found!"));
		return;
	}

	ACineCameraActor* MainCamera = Cast<ACineCameraActor>(FoundActors[0]);
	if (!MainCamera)
	{
		UE_LOG(LogTemp, Error, TEXT("Found actor is not a CineCameraActor!"));
		return;
	}

	PlayerController->SetViewTarget(MainCamera);
}

void AMainGameMode::SetupPlayerCharacter()
{
	// Cette fonction pourrait contenir d'autres configurations spécifiques au personnage
	if (!DefaultPawnClass)
	{
		UE_LOG(LogTemp, Error, TEXT("DefaultPawnClass is not set!"));
	}
}
