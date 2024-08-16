//----------------------------------------------//
// *Author		: github.com/AgustinRuscio		//
// *UE version	: UE 5.2.1						//
//----------------------------------------------//

#include "TPS_PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

static float CurrentSpringArmLength;
static FVector CurrentSpringArmSocketOffset;

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

	bIsMoving = true;
	
	if(!bIsAiming)
	{
		if(TimeLineSpringArmMoving.IsPlaying())
		{
			TimeLineSpringArmMoving.Play();
		}
		else
		{
			TimeLineSpringArmMoving.PlayFromStart();
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::MovementEnd()
{
	if(!bCanMove) return;
	
	bIsMoving = false;
	
	GetCharacterMovement()->MaxWalkSpeed = 250.0f;

	if(!bIsAiming)
	{
		if(TimeLineSpringArmMoving.IsPlaying())
		{
			TimeLineSpringArmMoving.Reverse();
		}
		else
		{
			TimeLineSpringArmMoving.ReverseFromEnd();
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::SprintStart()
{
	if(!bCanMove || bIsAiming) return;
	
	bIsSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::SprintEnd()
{
	if(!bCanMove|| bIsAiming) return;
	
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
void ATPS_PlayerCharacter::AimStart()
{
	bIsAiming				   = true;
	bUseControllerRotationYaw  = true;
	
	if(TimeLineSpringArmMoving.IsPlaying())
	{
		TimeLineSpringArmMoving.Stop();
	}

	CurrentSpringArmLength = SpringArmComp->TargetArmLength;
	CurrentSpringArmSocketOffset = SpringArmComp->SocketOffset;
	
	if(TimeLineSpringArmAiming.IsPlaying())
	{
		TimeLineSpringArmAiming.Play();
	}
	else
	{
		TimeLineSpringArmAiming.PlayFromStart();
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::AimEnd()
{
	bIsAiming				   = false;
	bUseControllerRotationYaw  = false;
	
	if(TimeLineSpringArmMoving.IsPlaying())
	{
		TimeLineSpringArmMoving.Stop();
	}
	
	CurrentSpringArmLength		 = SpringArmComp->TargetArmLength;
	CurrentSpringArmSocketOffset = SpringArmComp->SocketOffset;
	
	if(TimeLineSpringArmAiming.IsPlaying())
	{
		
		TimeLineSpringArmAiming.Play();
	}
	else
	{
		TimeLineSpringArmAiming.PlayFromStart();
	}
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
	//--- Spring Arm Length Moving
	FOnTimelineFloat SpringArmTargetArmLengthMovingChangedTick;
	SpringArmTargetArmLengthMovingChangedTick.BindUFunction(this, FName("SetSpringArmMovingSettings"));
	TimeLineSpringArmMoving.AddInterpFloat(FloatCurveSpringArmLength, SpringArmTargetArmLengthMovingChangedTick);

	//--- Spring Arm Length Aiming
	FOnTimelineFloat SpringArmTargetArmLengthAimingChangedTick;
	SpringArmTargetArmLengthAimingChangedTick.BindUFunction(this, FName("SetSpringArmAimingSettings"));
	TimeLineSpringArmAiming.AddInterpFloat(FloatCurveSpringArmLength, SpringArmTargetArmLengthAimingChangedTick);
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::TimeLinesTick(float DeltaSeconds)
{
	TimeLineSpringArmMoving.TickTimeline(DeltaSeconds);
	TimeLineSpringArmAiming.TickTimeline(DeltaSeconds);
}

//---------------------------------------------------------------------------------------------------------------------------------------
void ATPS_PlayerCharacter::SetSpringArmMovingSettings(float deltaSeconds) const
{
	auto NewLength = FMath::Lerp(125.0f, 200.0f, deltaSeconds);
	SpringArmComp->TargetArmLength = NewLength;
	
	auto NewLSocketOffset = FMath::Lerp(FVector(0.0f, 75.0f, 20.0f), FVector(0.0f, 90.0f, 30.0f), deltaSeconds);
	SpringArmComp->SocketOffset			= NewLSocketOffset;
}

void ATPS_PlayerCharacter::SetSpringArmAimingSettings(float deltaSeconds) const
{
	if(bIsAiming)
	{
		auto NewLength = FMath::Lerp(CurrentSpringArmLength, 90.0f, deltaSeconds);
		SpringArmComp->TargetArmLength = NewLength;
		
		auto NewLSocketOffset = FMath::Lerp(CurrentSpringArmSocketOffset, FVector(0.0f, 65.0f, 10.0f), deltaSeconds);
		SpringArmComp->SocketOffset			= NewLSocketOffset;
	}
	else
	{
		if(bIsMoving)
		{
			auto NewLength = FMath::Lerp(CurrentSpringArmLength, 200.0f, deltaSeconds);
			SpringArmComp->TargetArmLength = NewLength;
		
			auto NewLSocketOffset = FMath::Lerp(CurrentSpringArmSocketOffset, FVector(0.0f, 90.0f, 30.0f), deltaSeconds);
			SpringArmComp->SocketOffset			= NewLSocketOffset;
		}
		else
		{
			auto NewLength = FMath::Lerp(CurrentSpringArmLength, 125.0f, deltaSeconds);
			SpringArmComp->TargetArmLength = NewLength;
		
			auto NewLSocketOffset = FMath::Lerp(CurrentSpringArmSocketOffset, FVector(0.0f, 75.0f, 20.0f), deltaSeconds);
			SpringArmComp->SocketOffset			= NewLSocketOffset;
		}
	}
}