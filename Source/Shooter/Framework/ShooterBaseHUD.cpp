//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#include "ShooterBaseHUD.h"

#include "Blueprint/UserWidget.h"
#include "Shooter/Widgets/PlayerHUD.h"

//---------------------------------------------------------------------------------------------------------------------------------------
void AShooterBaseHUD::TogglePlayerHUD(bool visible)
{
	visible ? PlayerHUD->SetVisibility(ESlateVisibility::Collapsed) : PlayerHUD->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

//---------------------------------------------------------------------------------------------------------------------------------------
void AShooterBaseHUD::UpdateLifeBar(float Value)
{
	PlayerHUD->OnLifeBarUpdate(Value);
}

//---------------------------------------------------------------------------------------------------------------------------------------
void AShooterBaseHUD::StealthKill(bool Value)
{
	PlayerHUD->StealthKillWidget(Value);
}

//---------------------------------------------------------------------------------------------------------------------------------------
void AShooterBaseHUD::DisplayWinningWidget()
{
	CurrentFocusableWidget = WinningHUD;
	FocusWidget();
	
	WinningHUD->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

//---------------------------------------------------------------------------------------------------------------------------------------
void AShooterBaseHUD::DisplayLostWidget()
{
	CurrentFocusableWidget = DeathHUD;
	FocusWidget();
	
	DeathHUD->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

//---------------------------------------------------------------------------------------------------------------------------------------
void AShooterBaseHUD::BeginPlay()
{
	Super::BeginPlay();

	GetOwningPlayerController()->SetInputMode(FInputModeGameOnly());
	
	CreateGameHUD();
	CreateEndGameHUD();
}

//---------------------------------------------------------------------------------------------------------------------------------------
void AShooterBaseHUD::FocusWidget()
{
	GetOwningPlayerController()->SetInputMode(FInputModeGameAndUI().SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock).
																			SetHideCursorDuringCapture(true).SetWidgetToFocus(CurrentFocusableWidget->TakeWidget()));
	GetOwningPlayerController()->SetShowMouseCursor(true);
}

//---------------------------------------------------------------------------------------------------------------------------------------
void AShooterBaseHUD::CreateGameHUD()
{
	PlayerHUD = CreateWidget<UPlayerHUD>(GetWorld(), PlayerHUDWidget);
	PlayerHUD->AddToViewport(1);
}

//---------------------------------------------------------------------------------------------------------------------------------------
void AShooterBaseHUD::CreateEndGameHUD()
{
	WinningHUD = CreateWidget(GetWorld(), WinningWidget, "Winning Widget");
	WinningHUD->AddToViewport(0);
	WinningHUD->SetVisibility(ESlateVisibility::Collapsed);

	DeathHUD = CreateWidget(GetWorld(), DeathWidget, "Death Widget");
	DeathHUD->AddToViewport(0);
	DeathHUD->SetVisibility(ESlateVisibility::Collapsed);
}