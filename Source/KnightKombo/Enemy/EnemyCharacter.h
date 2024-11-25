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

	// Retourne la couleur actuelle de l'ennemi
	FLinearColor GetCurrentColor() const { return CurrentColor; }

	// Gère la destruction de l'ennemi
	void HandleDamage(FString AttackColor);

	// Permet de savoir si l'ennemi est actuellement en train d'attaquer
	bool IsAttacking() const { return bIsAttacking; }
	void PerformAttack();

	bool IsDying;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	// Indique si l'ennemi est en train d'attaquer
	bool bIsAttacking = false;
	
	FTimerHandle AttackTimerHandle;
	bool bHasReachedDestination;
	
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

	// Couleur actuelle de l'ennemi
	UPROPERTY(VisibleAnywhere, Category = "Appearance")
	FLinearColor CurrentColor;

	// Liste des couleurs possibles pour les ennemis
	UPROPERTY(EditAnywhere, Category = "Appearance")
	TMap<FString, FLinearColor> ColorNameMap;

	FString RandomColorName;
	// Changer l'animation actuelle
	void SetAnimation(UPaperFlipbook* NewAnimation);

	// Fonction pour définir une couleur aléatoire
	void SetRandomColor();
	
	void DestroyEnemy();
	
	// Ajouts pour gérer l'état d'attaque
	void ResetAttackState(); // Réinitialise l'état d'attaque

	FTimerHandle AttackResetTimerHandle; // Handle pour gérer le timer de réinitialisation
};
