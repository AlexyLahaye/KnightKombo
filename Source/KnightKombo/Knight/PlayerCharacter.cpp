#include "PlayerCharacter.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "Components/BoxComponent.h"
#include "Blueprint/UserWidget.h"
#include "TimerManager.h"
#include "KnightKombo/MainGameMode.h"

APlayerCharacter::APlayerCharacter()
{
    FlipbookComponent = GetSprite();
    IdleAnimation = nullptr;

    // Charger IdleAnimation
    static ConstructorHelpers::FObjectFinder<UPaperFlipbook> IdleAnimationAsset(TEXT("/Game/Sprite/Knight/idle/FB_Warrior_Sheet-Idle.FB_Warrior_Sheet-Idle"));
    if (IdleAnimationAsset.Succeeded()) IdleAnimation = IdleAnimationAsset.Object;

    // Charger les animations d'attaque
    static ConstructorHelpers::FObjectFinder<UPaperFlipbook> RedAttack(TEXT("/Game/Sprite/Knight/RedA/FB_RedA.FB_RedA"));
    static ConstructorHelpers::FObjectFinder<UPaperFlipbook> BlueAttack(TEXT("/Game/Sprite/Knight/BlueA/FB_BlueA.FB_BlueA"));
    static ConstructorHelpers::FObjectFinder<UPaperFlipbook> YellowAttack(TEXT("/Game/Sprite/Knight/YellowA/FB_YellowA.FB_YellowA"));
    static ConstructorHelpers::FObjectFinder<UPaperFlipbook> PurpleAttack(TEXT("/Game/Sprite/Knight/PurpleA/FB_PurpleA.FB_PurpleA"));
    static ConstructorHelpers::FObjectFinder<UPaperFlipbook> OrangeAttack(TEXT("/Game/Sprite/Knight/OrangeA/FB_OrangeA.FB_OrangeA"));
    static ConstructorHelpers::FObjectFinder<UPaperFlipbook> GreenAttack(TEXT("/Game/Sprite/Knight/GreenA/FB_GreenA.FB_GreenA"));

    if (RedAttack.Succeeded()) AttackAnimations.Add("Red", RedAttack.Object);
    if (BlueAttack.Succeeded()) AttackAnimations.Add("Blue", BlueAttack.Object);
    if (YellowAttack.Succeeded()) AttackAnimations.Add("Yellow", YellowAttack.Object);
    if (PurpleAttack.Succeeded()) AttackAnimations.Add("Purple", PurpleAttack.Object);
    if (OrangeAttack.Succeeded()) AttackAnimations.Add("Orange", OrangeAttack.Object);
    if (GreenAttack.Succeeded()) AttackAnimations.Add("Green", GreenAttack.Object);

    // Créer la HitBox
    HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
    HitBox->SetupAttachment(RootComponent);
    HitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    HitBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    HitBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (IdleAnimation)
		FlipbookComponent->SetFlipbook(IdleAnimation);

	// Récupérer le GameMode et le HUD
	AMainGameMode* GameMode = Cast<AMainGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		ComboHUDInstance = Cast<UWB_ComboHUD>(GameMode->GetComboHUD());
		if (!ComboHUDInstance)
		{
			UE_LOG(LogTemp, Error, TEXT("ComboHUDInstance est nul après récupération depuis le GameMode."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Impossible de récupérer le GameMode !"));
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("UpAttack", IE_Pressed, this, &APlayerCharacter::HandleUpInput);
	PlayerInputComponent->BindAction("DownAttack", IE_Pressed, this, &APlayerCharacter::HandleDownInput);
	PlayerInputComponent->BindAction("RightAttack", IE_Pressed, this, &APlayerCharacter::HandleRightInput);
	PlayerInputComponent->BindAction("LeftAttack", IE_Pressed, this, &APlayerCharacter::HandleLeftInput);
}

void APlayerCharacter::HandleComboInput(const FString& Input)
{
	if (!ComboHUDInstance) return;

	static FString FirstInput;
	static FString SecondInput;

	if (!FirstInput.IsEmpty() && !SecondInput.IsEmpty())
	{
		ResetHUD();
		FirstInput.Empty();
		SecondInput.Empty();
	}

	if (FirstInput.IsEmpty())
	{
		FirstInput = Input;
	}
	else
	{
		SecondInput = Input;
	}

	// Mise à jour des couleurs
	const FLinearColor FirstColor = GetColorFromInput(FirstInput);
	const FLinearColor SecondColor = GetColorFromInput(SecondInput);
	ComboHUDInstance->SetComboColors(FirstColor, SecondColor);

	// Timer pour réinitialiser
	GetWorldTimerManager().ClearTimer(ResetTimerHandle);
	GetWorldTimerManager().SetTimer(ResetTimerHandle, this, &APlayerCharacter::ResetHUD, 2.0f, false);

	// Gestion du buffer et des combos
	ComboInputs[ComboIndex] = Input;
	ComboIndex = (ComboIndex + 1) % MaxBufferSize;

	InputBuffer.Add(Input);
	if (InputBuffer.Num() > MaxBufferSize) InputBuffer.RemoveAt(0);

	CheckCombo();
}

void APlayerCharacter::CheckCombo()
{
	if (InputBuffer.Num() < 2) return;

	FString Combo = InputBuffer[0] + InputBuffer[1];
	if (Combo == "UpRight" || Combo == "RightUp") { CurrentAttackColor = "violet"; PlayAttackAnimation("Purple"); }
	else if (Combo == "LeftRight" || Combo == "RightLeft") {  CurrentAttackColor = "vert"; PlayAttackAnimation("Green"); }
	else if (Combo == "UpLeft" || Combo == "LeftUp") { CurrentAttackColor = "orange"; PlayAttackAnimation("Orange"); }
	else if (Combo == "UpUp") { CurrentAttackColor = "rouge"; PlayAttackAnimation("Red"); }
	else if (Combo == "RightRight") { CurrentAttackColor = "bleu"; PlayAttackAnimation("Blue"); }
	else if (Combo == "LeftLeft") { CurrentAttackColor = "jaune"; PlayAttackAnimation("Yellow"); }

	InputBuffer.Empty(); // Réinitialise après un combo
}

void APlayerCharacter::PlayAttackAnimation(const FString& AttackName)
{
	if (AttackAnimations.Contains(AttackName))
	{
		UPaperFlipbook* SelectedAnimation = AttackAnimations[AttackName];
		if (SelectedAnimation)
		{
			FlipbookComponent->SetFlipbook(SelectedAnimation);
			const float Duration = SelectedAnimation->GetTotalDuration();
			HandleDamageToEnemies();
			GetWorldTimerManager().SetTimerForNextTick([this, Duration]() {
				FTimerHandle TimerHandle;
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APlayerCharacter::OnAnimationFinished, Duration, false);
			});
		}
	}
}

void APlayerCharacter::OnAnimationFinished()
{
	if (IdleAnimation) FlipbookComponent->SetFlipbook(IdleAnimation);
}

FLinearColor APlayerCharacter::GetColorFromInput(const FString& Input) const
{
	if (Input == "Up") return FLinearColor::Red;
	if (Input == "Right") return FLinearColor::Blue;
	if (Input == "Left") return FLinearColor::Yellow;
	return FLinearColor::Transparent; // Par défaut
}

void APlayerCharacter::ResetHUD()
{
	if (ComboHUDInstance)
	{
		ComboHUDInstance->ResetColors();
	}
}

void APlayerCharacter::HandleUpInput() { HandleComboInput("Up"); }
void APlayerCharacter::HandleDownInput() { HandleComboInput("Down"); }
void APlayerCharacter::HandleRightInput() { HandleComboInput("Right"); }
void APlayerCharacter::HandleLeftInput() { HandleComboInput("Left"); }

void APlayerCharacter::HandleDamageToEnemies()
{
	// Vérifier les ennemis en collision avec la HitBox du joueur
	TArray<AActor*> OverlappingActors;
	HitBox->GetOverlappingActors(OverlappingActors, AEnemyCharacter::StaticClass()); // Vérifie uniquement les ennemis

	for (AActor* Actor : OverlappingActors)
	{
		AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(Actor);
		if (Enemy)
		{
			// Appliquer la logique de gestion des dégâts
			Enemy->HandleDamage(CurrentAttackColor);
		}
	}
}