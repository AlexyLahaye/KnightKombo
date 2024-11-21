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

protected:
	virtual void BeginPlay() override;

public:
	// Propriétés ou méthodes spécifiques
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	float Health;

	void TakeDamage(float DamageAmount);
};
