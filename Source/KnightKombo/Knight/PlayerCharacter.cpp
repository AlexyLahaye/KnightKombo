#include "PlayerCharacter.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "GameFramework/PlayerController.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"

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

    if (RedAttack.Succeeded()) AttackAnimations.Add("Red", RedAttack.Object);
    if (BlueAttack.Succeeded()) AttackAnimations.Add("Blue", BlueAttack.Object);
    if (YellowAttack.Succeeded()) AttackAnimations.Add("Yellow", YellowAttack.Object);
    if (PurpleAttack.Succeeded()) AttackAnimations.Add("Purple", PurpleAttack.Object);
}

void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
    if (IdleAnimation) FlipbookComponent->SetFlipbook(IdleAnimation);
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
    InputBuffer.Add(Input);
    if (InputBuffer.Num() > 2) InputBuffer.RemoveAt(0);
    CheckCombo();
}

void APlayerCharacter::CheckCombo()
{
    if (InputBuffer.Num() < 2) return;

    FString Combo = InputBuffer[0] + InputBuffer[1];
    if (Combo == "UpRight" || Combo == "RightUp") PlayAttackAnimation("Purple");
    else if (InputBuffer[0] == "Up") PlayAttackAnimation("Red");
    else if (InputBuffer[0] == "Right") PlayAttackAnimation("Blue");
    else if (InputBuffer[0] == "Left") PlayAttackAnimation("Yellow");

    InputBuffer.Empty(); // Réinitialiser après un combo
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

void APlayerCharacter::HandleUpInput() { HandleComboInput("Up"); }
void APlayerCharacter::HandleDownInput() { HandleComboInput("Down"); }
void APlayerCharacter::HandleRightInput() { HandleComboInput("Right"); }
void APlayerCharacter::HandleLeftInput() { HandleComboInput("Left"); }
