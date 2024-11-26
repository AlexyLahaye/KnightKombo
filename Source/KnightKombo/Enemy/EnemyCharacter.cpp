#include "EnemyCharacter.h"
#include "EnemyAIController.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "GameFramework/CharacterMovementComponent.h"

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
	if (IsDying) // Si l'ennemi est mourant, il ne peut pas attaquer
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy is dying."));
		return;
	}

	bIsAttacking = true; // L'ennemi commence à attaquer

	if (AttackAnimation)
	{
		SetAnimation(AttackAnimation);
		UE_LOG(LogTemp, Log, TEXT("Enemy is attacking!"));

		// Ajouter un timer pour réinitialiser bIsAttacking après la durée de l'animation
		float AttackAnimationDuration = AttackAnimation->GetTotalDuration();
		GetWorld()->GetTimerManager().SetTimer(
			AttackResetTimerHandle, // Handle du timer
			this,                   // Objet propriétaire de la méthode
			&AEnemyCharacter::ResetAttackState, // Méthode à appeler
			AttackAnimationDuration, // Durée du délai
			false                    // Pas de répétition
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

		// Détruire l'ennemi après la durée de l'animation
		FTimerHandle DestroyTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &AEnemyCharacter::DestroyEnemy, DeathAnimationDuration, false);
		
	}
}

void AEnemyCharacter::DestroyEnemy()
{
	Destroy();
}
