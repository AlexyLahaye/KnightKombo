#include "EnemyCharacter.h"

AEnemyCharacter::AEnemyCharacter()
{
	Health = 100.f; // Exemple de valeur par défaut
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyCharacter::TakeDamage(float DamageAmount)
{
	Health -= DamageAmount;
	if (Health <= 0)
	{
		Destroy(); // Détruit l'ennemi quand sa vie est à 0
	}
}
