// Fill out your copyright notice in the Description page of Project Settings.
#include "MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyAnimInstance.h"
#include "TimerManager.h"
// Sets default values
AMyCharacter::AMyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	MovementComponent->bOrientRotationToMovement = false;
	//MovementComponent->RotationRate = FRotator(0.0f, 450.0f, 0.0f);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->SetWorldRotation(FRotator(-30.0f, 0.0f, 0.0f));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	WalkSpeed = 450.0f;
	SprintSpeed = 1000.0f;

	//RollSpeed = 1200.0f;  // Roll 속도 설정
	//RollDistance = 600.0f;  // Roll 거리 설정

	MaxStamina = 100;
	Stamina = 60;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	MyAnimInstance = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		UEnhancedInputLocalPlayerSubsystem* EnhancedInputSystem;
		EnhancedInputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (EnhancedInputSystem != nullptr)
		{
			EnhancedInputSystem->AddMappingContext(MappingContext, 0);
		}
	}
	Stamina = 60;
	RegenStamina();

}

void AMyCharacter::Move(const FInputActionValue& value)
{
	if (GetController() != nullptr)
	{
		const FVector2D MoveVector2D = value.Get<FVector2D>();

		const FRotator ControllerRotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0.0f, ControllerRotation.Yaw, 0.0f);

		const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Forward, MoveVector2D.X);

		const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Right, MoveVector2D.Y);

		UpdateAnimInstance(MoveVector2D);
	}
}

void AMyCharacter::Rotation(const FInputActionValue& value)
{
	if (GetController() != nullptr)
	{
		FVector2D RotationValue = value.Get<FVector2D >();
		AddControllerYawInput(RotationValue.X);
		AddControllerPitchInput(RotationValue.Y);
	}
}

void AMyCharacter::StartSprint()
{
	if (MyAnimInstance != nullptr)
	{
		MyAnimInstance->SetStartSprint();
	}
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	DrainStamina();
}

void AMyCharacter::EndSprint()
{
	if (MyAnimInstance != nullptr)
	{
		MyAnimInstance->SetEndSprint();
	}
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	RegenStamina();
}

void AMyCharacter::StartRoll(const FInputActionValue& value)
{
	if (MyAnimInstance != nullptr)
	{
		MyAnimInstance->SetStartRolling();
	}
	/*MyAnimInstance->Montage_Play(RollMontage);*/

}


void AMyCharacter::EndRoll(const FInputActionValue& value)
{
	if (MyAnimInstance != nullptr)
	{
		MyAnimInstance->SetEndRolling();
	}
}

void AMyCharacter::DrainStamina()
{
	int NewStamina = Stamina - 1.0f;
	NewStamina = FMath::Clamp(NewStamina, 0.0f, MaxStamina);

	Stamina = NewStamina;

	if (Stamina <= 0.0f)
	{
		EndSprint();
	}
	else if (MyAnimInstance->ReturnIsSprint() == true)
	{
		// If sprinting and Stamina is not 0, set up a timer to call DrainStamina again
		GetWorldTimerManager().SetTimer(SprintDrainTimerHandle, this, &AMyCharacter::OnSprintDrainDelayElapsed, 0.1f, false);
	}
}


void AMyCharacter::RegenStamina()
{
	int NewStamina = Stamina + 1.0f;
	NewStamina = FMath::Clamp(NewStamina, 0.0f, MaxStamina);

	Stamina = NewStamina;


	if (Stamina != MaxStamina)
	{

		if (MyAnimInstance->ReturnIsSprint() == false)
		{
			// If sprinting and Stamina is not 0, set up a timer to call DrainStamina again
			GetWorldTimerManager().SetTimer(SprintRegenTimerHandle, this, &AMyCharacter::OnSprintRegenDelayElapsed, 0.05f, false);
		}
	}
}

void AMyCharacter::OnSprintDrainDelayElapsed()
{
	// Call DrainStamina again after delay
	DrainStamina();
}

void AMyCharacter::OnSprintRegenDelayElapsed()
{
	RegenStamina();
}

void AMyCharacter::UpdateAnimInstance(const FVector2D& MoveVector2D)
{
	if (MyAnimInstance)
	{
		// Set all movement direction variables to false initially
		MyAnimInstance->SetIsStraightMove(false);
		MyAnimInstance->SetIsBackMove(false);
		MyAnimInstance->SetIsLeftMove(false);
		MyAnimInstance->SetIsRightMove(false);

		// Determine which direction key is being pressed
		if (MoveVector2D.X > 0.0f)  // Moving forward
		{
			MyAnimInstance->SetIsStraightMove(true);
		}
		else if (MoveVector2D.X < 0.0f)  // Moving backward
		{
			MyAnimInstance->SetIsBackMove(true);
		}

		if (MoveVector2D.Y < 0.0f)  // Moving left
		{
			MyAnimInstance->SetIsLeftMove(true);
		}
		else if (MoveVector2D.Y > 0.0f)  // Moving right
		{
			MyAnimInstance->SetIsRightMove(true);
		}
	}
}


// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FString StaminaString = FString::Printf(TEXT("Stamina: %d"), Stamina);

	GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Red, StaminaString);

	FRotator CameraRotation = Camera->GetComponentRotation();

	// 캐릭터가 회전할 때, 피치와 롤 회전값은 무시하고 요 값만 사용합니다.
	FRotator TargetRotation(0.0f, CameraRotation.Yaw, 0.0f);

	// 캐릭터의 회전을 카메라 방향으로 설정합니다.
	SetActorRotation(TargetRotation);
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacter::Move);
		EnhancedInputComponent->BindAction(RotationAction, ETriggerEvent::Triggered, this, &AMyCharacter::Rotation);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMyCharacter::Jump);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AMyCharacter::StartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AMyCharacter::EndSprint);
		//EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Triggered, this, &AMyCharacter::StartRoll);
		//EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Completed, this, &AMyCharacter::EndRoll);
	}

}

