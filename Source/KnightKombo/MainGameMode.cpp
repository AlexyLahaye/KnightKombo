#include "MainGameMode.h"
#include "CineCameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "UObject/ConstructorHelpers.h"

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
}

void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();

	SetupPlayerCamera();
	SetupPlayerCharacter();
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
