#include "EnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemyCharacter::AEnemyCharacter()
{
	Health = 100.f; // Exemple de valeur par défaut
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

float AEnemyCharacter::TakeDamage(
	float DamageAmount,
	FDamageEvent const& DamageEvent,
	AController* EventInstigator,
	AActor* DamageCauser)
{
	Health -= DamageAmount;
	if (Health <= 0.f)
	{
		Destroy();
	}
	return DamageAmount;
}

void AEnemyCharacter::MoveToLocation(const FVector& TargetLocation)
{
	FVector CurrentLocation = GetActorLocation();
	SetActorLocation(FMath::VInterpTo(CurrentLocation, TargetLocation, GetWorld()->GetDeltaSeconds(), 2.f));
}
