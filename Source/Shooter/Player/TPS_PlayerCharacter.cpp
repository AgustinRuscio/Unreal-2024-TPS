//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#include "TPS_PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

//---------------------------------------------------------------------------------------------------------------------------------------
ATPS_PlayerCharacter::ATPS_PlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArmComp->SetupAttachment(GetMesh());
	SpringArmComp->TargetArmLength = 125.0f;
	SpringArmComp->SocketOffset    = FVector(0.0f, 75.0f, 20.0f);
	SpringArmComp->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
    SpringArmComp->bUsePawnControlRotation = true;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed			  = 250.0f;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComponent->SetupAttachment(SpringArmComp);

	GetMesh()->bReceivesDecals = false;
	bUseControllerRotationYaw  = false;
	
	bCanMove	   = true;
	bCanMoveCamera = true;
	bIsSprinting   = false;
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::MovePlayer(FVector2d Direction)
{
	if(!bCanMove) return;
	
	if(Direction.Y != 0)
	{
		const FVector& CameraForward = CameraComponent->GetForwardVector();
		AddMovementInput(CameraForward, Direction.Y);
	}
	
	if(Direction.X != 0)
	{
		const FVector& CameraRight = CameraComponent->GetRightVector();
		AddMovementInput(CameraRight, Direction.X);
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::MovementStart()
{
	if(!bCanMove) return;
	
	if(TimeLineSpringArm.IsPlaying())
	{
		TimeLineSpringArm.Play();
	}
	else
	{
		TimeLineSpringArm.PlayFromStart();
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::MovementEnd()
{
	if(!bCanMove) return;
	
	GetCharacterMovement()->MaxWalkSpeed = 250.0f;
	
	if(TimeLineSpringArm.IsPlaying())
	{
		TimeLineSpringArm.Reverse();
	}
	else
	{
		TimeLineSpringArm.ReverseFromEnd();
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::SprintStart()
{
	if(!bCanMove) return;
	
	bIsSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::SprintEnd()
{
	if(!bCanMove) return;
	
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = 375.0f;
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::RotateCamera(FVector2d Direction)
{
	if(!bCanMoveCamera) return;
	
	AddControllerYawInput(Direction.X);
	AddControllerPitchInput(Direction.Y);
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	BindTimeLines();
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeLinesTick(DeltaTime);
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::BindTimeLines()
{
	//--- Spring Arm Length
	FOnTimelineFloat SpringArmTargetArmLengthChangedTick;
	SpringArmTargetArmLengthChangedTick.BindUFunction(this, FName("SetSpringArmMovingSettings"));
	TimeLineSpringArm.AddInterpFloat(FloatCurveSpringArmLength, SpringArmTargetArmLengthChangedTick);
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::TimeLinesTick(float DeltaSeconds)
{
	TimeLineSpringArm.TickTimeline(DeltaSeconds);
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::SetSpringArmMovingSettings(float deltaSeconds) const
{
	auto NewLength = FMath::Lerp(125.0f, 200.0f, deltaSeconds);
	SpringArmComp->TargetArmLength = NewLength;
	
	auto NewLSocketOffset = FMath::Lerp(FVector(0.0f, 75.0f, 20.0f), FVector(0.0f, 90.0f, 30.0f), deltaSeconds);
	SpringArmComp->SocketOffset			= NewLSocketOffset;
}