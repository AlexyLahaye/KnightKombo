#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class KNIGHTKOMBO_API AEnemyCharacter : public APaperCharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

	// Déplace l'ennemi vers une position
	void MoveToLocation(const FVector& TargetLocation);

	// Gère les dégâts reçus
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	float Health;
};
