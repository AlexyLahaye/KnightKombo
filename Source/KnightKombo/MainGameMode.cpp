#include "MainGameMode.h"
#include "Blueprint/UserWidget.h"
#include "CineCameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/EnemySpawner.h"

AMainGameMode::AMainGameMode()
{
	// Charger le Blueprint du personnage joueur
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_PlayerCharactere"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
		PlayerCharacterClass = PlayerPawnBPClass.Class;
		UE_LOG(LogTemp, Log, TEXT("PlayerCharacterClass successfully set to BP_PlayerCharacter."));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Could not find BP_PlayerCharacter at the specified path."));
	}

	// Charger la classe Blueprint pour le HUD Combo
	static ConstructorHelpers::FClassFinder<UUserWidget> ComboHUD_BP(TEXT("/Game/Blueprints/WB_ComboHUD"));
	if (ComboHUD_BP.Class != nullptr)
	{
		ComboHUDClass = ComboHUD_BP.Class;
		UE_LOG(LogTemp, Log, TEXT("ComboHUDClass successfully set to WB_ComboHUD."));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Could not find WB_ComboHUD at the specified path."));
	}

	// Charger la classe Blueprint pour le spawner d'ennemis
	static ConstructorHelpers::FClassFinder<AEnemySpawner> EnemySpawner_BP(TEXT("/Game/Blueprints/BP_EnemySpawner"));
	if (EnemySpawner_BP.Class != nullptr)
	{
		EnemySpawnerClass = EnemySpawner_BP.Class;
		UE_LOG(LogTemp, Log, TEXT("EnemySpawnerClass successfully set to BP_EnemySpawner."));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Could not find BP_EnemySpawner at the specified path."));
	}
	
}

void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Configuration de la caméra
	SetupPlayerCamera();

	// Création du HUD
	if (ComboHUDClass)
	{
		ComboHUDInstance = CreateWidget<UUserWidget>(GetWorld(), ComboHUDClass);
		if (ComboHUDInstance)
		{
			ComboHUDInstance->AddToViewport();
			UE_LOG(LogTemp, Warning, TEXT("HUD created successfully!"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create the HUD instance!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ComboHUDClass is not set!"));
	}

	// Lancement de la vague d'ennemis
	StartEnemyWave();
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
	UE_LOG(LogTemp, Log, TEXT("Player camera successfully set to MainCamera."));
}

void AMainGameMode::StartEnemyWave()
{
	if (!EnemySpawnerClass)
	{
		UE_LOG(LogTemp, Error, TEXT("EnemySpawnerClass is not set!"));
		return;
	}

	// Recherche l'instance de spawner avec un tag spécifique
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("MainSpawner"), FoundActors);

	if (FoundActors.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("No actors with tag 'MainSpawner' found!"));
		return;
	}

	EnemySpawnerInstance = Cast<AEnemySpawner>(FoundActors[0]);
	if (EnemySpawnerInstance)
	{
		EnemySpawnerInstance->StartSpawning();
		UE_LOG(LogTemp, Log, TEXT("EnemySpawner instance with tag 'MainSpawner' found and spawning started."));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to cast the found actor with tag 'MainSpawner' to AEnemySpawner."));
	}
}

