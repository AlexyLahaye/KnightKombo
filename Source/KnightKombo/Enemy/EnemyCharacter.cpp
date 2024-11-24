#include "EnemyCharacter.h"
#include "EnemyAIController.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemyCharacter::AEnemyCharacter()
{
	
	FlipbookComponent = GetSprite();

	// Initialiser les couleurs possibles
	EnemyColors = {
		FLinearColor::Red,
		FLinearColor::Blue,
		FLinearColor::Green,
		FLinearColor::Yellow,
		FLinearColor(1.0f, 0.65f, 0.0f), // Orange
		FLinearColor(0.5f, 0.0f, 0.5f)  // Violet
	};

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

	MoveSpeed = FMath::RandRange(100.0f, 150.0f);

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

	if (Speed > 0.0f)
	{
		// Jouer l'animation de marche si elle n'est pas déjà active
		SetAnimation(WalkAnimation);
	}
	else
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
	if (FlipbookComponent && EnemyColors.Num() > 0)
	{
		// Choisir une couleur aléatoire
		int32 RandomIndex = FMath::RandRange(0, EnemyColors.Num() - 1);
		FlipbookComponent->SetSpriteColor(EnemyColors[RandomIndex]);
	}
}
