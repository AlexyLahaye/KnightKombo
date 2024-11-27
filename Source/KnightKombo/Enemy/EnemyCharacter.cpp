#include "EnemyCharacter.h"
#include "EnemyAIController.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "KnightKombo/MainGameMode.h"
#include "KnightKombo/Knight/PlayerCharacter.h"

AEnemyCharacter::AEnemyCharacter()
{
	FlipbookComponent = GetSprite();

	// Initialiser les couleurs possibles avec leur nom
	ColorNameMap.Add("rouge", FLinearColor::Red);
	ColorNameMap.Add("bleu", FLinearColor::Blue);
	ColorNameMap.Add("vert", FLinearColor::Green);
	ColorNameMap.Add("jaune", FLinearColor::Yellow);
	ColorNameMap.Add("orange", FLinearColor(1.0f, 0.65f, 0.0f)); // Orange
	ColorNameMap.Add("violet", FLinearColor(0.5f, 0.0f, 0.5f));  // Violet

	// Charger les animations de marche et d'attaque
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook> Idle(TEXT("/Game/Sprite/EnemyPurple/idle/FB_EnemyPurple.FB_EnemyPurple"));
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook> Walk(TEXT("/Game/Sprite/EnemyPurple/walk/FB_WalkEnemyPurple.FB_WalkEnemyPurple"));
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook> Attack(TEXT("/Game/Sprite/EnemyPurple/Attack/FB_AttackEnemyPurple.FB_AttackEnemyPurple"));
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook> Death(TEXT("/Game/Sprite/EnemyPurple/Death/FB_DeathEnemyPurple.FB_DeathEnemyPurple"));
	
	if (Idle.Succeeded()) IdleAnimation = Idle.Object;
	if (Walk.Succeeded()) WalkAnimation = Walk.Object;
	if (Attack.Succeeded()) AttackAnimation = Attack.Object;
	if (Death.Succeeded()) DeathAnimation = Death.Object;

	// Assigner l'AIController personnalisé
	AIControllerClass = AEnemyAIController::StaticClass();

	// Auto-possession par l'IA
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	MoveSpeed = FMath::RandRange(300.0f, 350.0f);

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
	}
	
	// Assigner une couleur aléatoire
	SetRandomColor();

	// Récupérer l'instance du joueur
	APlayerCharacter* Player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (Player)
	{
		ComboHUDInstance = Player->ComboHUDInstance;
		if (!ComboHUDInstance)
		{
			UE_LOG(LogTemp, Error, TEXT("EnemyCharacter: Impossible de récupérer ComboHUDInstance depuis PlayerCharacter."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("EnemyCharacter: Impossible de récupérer le PlayerCharacter."));
	}
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Vérifier si le personnage est en mouvement
	const float Speed = GetCharacterMovement()->Velocity.Size();

	if (bIsAttacking || IsDying) // Si l'ennemi attaque ou est en train de mourir, ne rien changer
	{
		return;
	}

	if (Speed > 0.0f && bHasReachedDestination == false)
	{
		// Jouer l'animation de marche si elle n'est pas déjà active
		SetAnimation(WalkAnimation);
	}
	else if (bHasReachedDestination == false)
	{
		// Jouer l'animation Idle si elle n'est pas déjà active
		SetAnimation(IdleAnimation);
	}
}


void AEnemyCharacter::MoveToLocation(const FVector& TargetLocation)
{
	AEnemyAIController* AIController = Cast<AEnemyAIController>(Controller);
	if (AIController)
	{
		AIController->MoveToTarget(TargetLocation);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No valid AIController found for EnemyCharacter."));
	}
}

void AEnemyCharacter::SetAnimation(UPaperFlipbook* NewAnimation)
{
	if (FlipbookComponent && NewAnimation && FlipbookComponent->GetFlipbook() != NewAnimation)
	{
		FlipbookComponent->SetFlipbook(NewAnimation);
	}
}

void AEnemyCharacter::SetRandomColor()
{
	if (ColorNameMap.Num() > 0)
	{
		// Choisir une couleur aléatoire et obtenir son nom
		TArray<FString> ColorNames;
		ColorNameMap.GetKeys(ColorNames);
		int32 RandomIndex = FMath::RandRange(0, ColorNames.Num() - 1);
		RandomColorName = ColorNames[RandomIndex];
		CurrentColor = ColorNameMap[RandomColorName];

		// Mettre à jour la couleur de l'ennemi
		FlipbookComponent->SetSpriteColor(CurrentColor);
	}
}

void AEnemyCharacter::PerformAttack()
{
	if (IsDying || bIsAttacking) // Si l'ennemi est mourant ou attaque déjà, ne rien faire
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy is dying ou attaque déjà."));
		return;
	}

	bIsAttacking = true; // L'ennemi commence à attaquer

	if (AttackAnimation)
	{
		SetAnimation(AttackAnimation);
		UE_LOG(LogTemp, Log, TEXT("Enemy is attacking!"));

		// *** Jouer le son d'attaque ***
		USoundBase* AttackSound = Cast<USoundBase>(StaticLoadObject(
			USoundWave::StaticClass(),
			nullptr,
			TEXT("/Game/Sound/Sword_Slash-Sound_Effect_HD")
		));

		if (AttackSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, AttackSound, GetActorLocation());
			UE_LOG(LogTemp, Log, TEXT("Attack sound played."));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to load attack sound!"));
		}

		// Ajouter un timer unique pour gérer à la fois la réinitialisation et la réduction des vies
		float AttackAnimationDuration = AttackAnimation->GetTotalDuration() - 0.1f;

		GetWorld()->GetTimerManager().SetTimer(
			AttackResetTimerHandle,
			[this]()
			{
				// Réinitialiser l'état d'attaque
				ResetAttackState();

				// Réduire les vies du joueur
				AMainGameMode* GameMode = Cast<AMainGameMode>(GetWorld()->GetAuthGameMode());
				if (GameMode && !IsDying)
				{
					GameMode->DecreasePlayerLives();
				}
			},
			AttackAnimationDuration,
			false // Pas de répétition
		);
	}
}



void AEnemyCharacter::ResetAttackState()
{
	bIsAttacking = false;
	UE_LOG(LogTemp, Log, TEXT("Enemy finished attacking."));
}

void AEnemyCharacter::HandleDamage(FString CurrentAttackColor)
{
	if (CurrentAttackColor == RandomColorName)
	{
		IsDying = true;
		// Jouer l'animation de mort
		SetAnimation(DeathAnimation);

		// Obtenir la durée de l'animation
		float DeathAnimationDuration = DeathAnimation->GetTotalDuration() - 0.04;

		// Arrêter le cycle d'attaque dans l'AIController avant la destruction
		AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController());
		if (AIController)
		{
			AIController->StopAttackCycle();
		}

		// Mettre à jour le score via le HUD
		if (ComboHUDInstance)
		{
			ComboHUDInstance->UpdateScore();
		}

		// *** Jouer le son d'attaque ***
		USoundBase* AttackSound = Cast<USoundBase>(StaticLoadObject(
			USoundWave::StaticClass(),
			nullptr,
			TEXT("/Game/Sound/Zombie_Death__Minecraft__-_Sound_Effect_HD")
		));

		if (AttackSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, AttackSound, GetActorLocation());
			UE_LOG(LogTemp, Log, TEXT("Attack sound played."));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to load attack sound!"));
		}
		// Détruire l'ennemi après la durée de l'animation
		FTimerHandle DestroyTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &AEnemyCharacter::DestroyEnemy, DeathAnimationDuration, false);
	}
}

void AEnemyCharacter::DestroyEnemy()
{
	Destroy();
}
