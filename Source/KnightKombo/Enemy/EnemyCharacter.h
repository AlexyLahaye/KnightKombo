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

	// Méthode pour déplacer l'ennemi vers une cible
	void MoveToLocation(const FVector& TargetLocation);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, Category = "Movement")
	float MoveSpeed;
	
	// Flipbook pour les animations
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UPaperFlipbookComponent* FlipbookComponent;

	UPROPERTY(EditAnywhere, Category = "Animations")
	class UPaperFlipbook* IdleAnimation;

	UPROPERTY(EditAnywhere, Category = "Animations")
	class UPaperFlipbook* WalkAnimation;

	UPROPERTY(EditAnywhere, Category = "Animations")
	class UPaperFlipbook* AttackAnimation;
	
	UPROPERTY(EditAnywhere, Category = "Animations")
	class UPaperFlipbook* DeathAnimation;

	// Changer l'animation actuelle
	void SetAnimation(UPaperFlipbook* NewAnimation);

	UPROPERTY(EditAnywhere, Category = "Appearance")
	TArray<FLinearColor> EnemyColors;

	// Fonction pour définir une couleur aléatoire
	void SetRandomColor();
};