#include "PlayerCharacter.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "GameFramework/PlayerController.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

APlayerCharacter::APlayerCharacter()
{

	// Configuration des paramètres de saut (optionnel)
	GetCharacterMovement()->JumpZVelocity = 600.f; // Hauteur du saut
	GetCharacterMovement()->AirControl = 0.2f;     // Contrôle en l'air
	
	// Configuration du FlipbookComponent
	FlipbookComponent = GetSprite();

	// Initialisation des animations à NULL
	IdleAnimation = nullptr;
	UpAttackAnimation = nullptr;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Charger l'animation Idle au démarrage
	if (IdleAnimation)
	{
		FlipbookComponent->SetFlipbook(IdleAnimation);
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Associer la barre espace à la fonction HandleSpaceBar
	// Associer la barre espace à la fonctionnalité de saut
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	
	// Lier l'entrée de la flèche haut à l'attaque vers le haut
	PlayerInputComponent->BindAction("UpAttack", IE_Pressed, this, &APlayerCharacter::PlayUpAttackAnimation);
}

void APlayerCharacter::PlayUpAttackAnimation()
{
	if (UpAttackAnimation)
	{
		// Change l'animation en cours
		FlipbookComponent->SetFlipbook(UpAttackAnimation);

		// Récupérer la durée de l'animation
		const float AnimationDuration = UpAttackAnimation->GetTotalDuration();  // Utilisation correcte de la durée de l'animation

		// Lancer un timer pour revenir à l'animation Idle après la fin de l'animation
		GetWorldTimerManager().SetTimerForNextTick([this, AnimationDuration]() {
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APlayerCharacter::OnAnimationFinished, AnimationDuration, false);
		});
	}
}


void APlayerCharacter::OnAnimationFinished()
{
	if (IdleAnimation)
	{
		FlipbookComponent->SetFlipbook(IdleAnimation);
	}
}
