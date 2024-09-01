// Fill out your copyright notice in the Description page of Project Settings.
#include "Characters/MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/MyAnimInstance.h"
#include "TimerManager.h"

#include "Items/CollisionItem.h"
#include "Items/WeaponItem.h"
#include "Characters/CharacterType.h"



// Sets default values
AMyCharacter::AMyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;


	///////////////////////////
	// 컨트롤 관련
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
	///////////////////////////

	///////////////////////////
	// 전투 관련
	OverlapItem = nullptr;
	WeaponItem = nullptr;


	State = ECharacterState::NONE;
	ActionState = ECharacterAction::IDLE;
	///////////////////////////
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

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance != nullptr)
	{
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &AMyCharacter::HandleOnMontageNotifyBegin);
	}
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

		// 전투 input 
		EnhancedInputComponent->BindAction(GetAction, ETriggerEvent::Started, this, &AMyCharacter::EquipWeapon);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AMyCharacter::Attack);
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Started, this, &AMyCharacter::Equip);
	}

}

//////////////////////////////////////////////////////////////////
// 전투 관련

// 캐릭터를 기본 상태로 정렬
void AMyCharacter::SetIdle()
{
	if (ActionState == ECharacterAction::IDLE)
		return;

	ActionState = ECharacterAction::IDLE;
}

// 공격 함수 : 마우스 좌클릭
void AMyCharacter::Attack(const FInputActionValue& value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("Attack Started"));

	// 실행 하는 조건
	if (ActionState == ECharacterAction::EQUIP
		&& State == ECharacterState::ONE_HAND_WEAPON)
	{
		PlayAttackMontage();
	}
	else
	{
		return;
	}
}

// Attack 하는지 체크
bool AMyCharacter::IsAttacking()
{
	UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
	if (AnimInst != nullptr)
	{
		if (AnimInst->Montage_IsPlaying(AttackMontage))
		{
			return true;
		}
	}
	return false;
}

// 공격 끝 부분 알려줌
void AMyCharacter::AttackEnd()
{
	if (ActionState != ECharacterAction::ATTACK)
		return;

	ActionState = ECharacterAction::EQUIP;
}

void AMyCharacter::HandleOnMontageNotifyBegin(FName a_nNotifyName, const FBranchingPointNotifyPayload& a_pBranchingPayload)
{
	//콤보 인덱스 감소
	m_iComboAttackIndex--;

	// 0보다 작을 때 몽타주 종료
	if (m_iComboAttackIndex < 0)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Stop(0.4f, AttackMontage);
		}
	}
}

// 장비 줍기 : F키 누름
void AMyCharacter::EquipWeapon()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("EquipWeapon called"));
	}

	if (ActionState != ECharacterAction::IDLE)
		return;

	if (OverlapItem)
	{
		AWeaponItem* weapon = Cast<AWeaponItem>(OverlapItem);

		if (State == ECharacterState::NONE && WeaponItem == nullptr)
		{
			if (weapon != nullptr)
			{
				WeaponItem = weapon;
				OverlapItem = nullptr;
				WeaponItem->Equip(GetMesh(), FName("WeaponSocket"));
				State = ECharacterState::ONE_HAND_WEAPON;
				ActionState = ECharacterAction::EQUIP;
			}

			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Equipped Weapon: %s"), *WeaponItem->GetName()));
			}
		}
		else if (WeaponItem != nullptr)
		{
			WeaponItem = nullptr;

			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No weapon to equip"));
			}
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("No overlapping item"));
		}
	}
}

// 장비하는 함수 : X키 누름
void AMyCharacter::Equip()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("Equip Started"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, *FString::Printf(TEXT("Current State : %d"), static_cast<int32>(State)));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, *FString::Printf(TEXT("Current ActionState : %d"), static_cast<int32>(ActionState)));


	if (ActionState == ECharacterAction::IDLE
		&& State != ECharacterState::ONE_HAND_WEAPON)
		return;

	if (State == ECharacterState::ONE_HAND_WEAPON && ActionState == ECharacterAction::EQUIP)
	{
		PlayEquipMontage();
		State = ECharacterState::UNEQUIP_WEAPON;
		ActionState = ECharacterAction::UNEQUIP;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, *FString::Printf(TEXT("State : %d"), static_cast<int32>(State)));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, *FString::Printf(TEXT("ActionState : %d"), static_cast<int32>(ActionState)));
	}
	else if (State == ECharacterState::UNEQUIP_WEAPON && ActionState == ECharacterAction::UNEQUIP)
	{
		PlayUnEquipMontage();
		State = ECharacterState::ONE_HAND_WEAPON;
		ActionState = ECharacterAction::EQUIP;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, *FString::Printf(TEXT("State : %d"), static_cast<int32>(State)));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, *FString::Printf(TEXT("ActionState : %d"), static_cast<int32>(ActionState)));
	}
}


void AMyCharacter::PlayAttackMontage()
{
	if (AttackMontage == nullptr)
		return;

	if (!IsAttacking())
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			if (AttackMontage != nullptr)
			{
				AnimInstance->Montage_Play(AttackMontage);
			}
		}
	}
	else
	{
		m_iComboAttackIndex = 1;
	}
}

// 장비 장착 애니메이션 인스턴스
void AMyCharacter::PlayEquipMontage()
{
	if (EquipMontage == nullptr)
		return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("PlayEquipMontage Called"));
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(FName("Equip"));
	}
}

// 장비 장착 해제 애니메이션 인스턴스
void AMyCharacter::PlayUnEquipMontage()
{
	if (EquipMontage == nullptr)
		return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("PlayUnEquipMontage Called"));
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(FName("UnEquip"));
	}
}


void AMyCharacter::EquipWeaponMesh()
{
	if (WeaponItem != nullptr)
		WeaponItem->SetMashSocket(GetMesh(), FName("SpineSocket"));
}

void AMyCharacter::UnEquipWeaponMesh()
{
	if (WeaponItem != nullptr)
		WeaponItem->SetMashSocket(GetMesh(), FName("WeaponSocket"));
}

void AMyCharacter::SetAttackCollisionEnable(ECollisionEnabled::Type Type)
{
	if (WeaponItem != nullptr)
		WeaponItem->SetAttackBoxCollisionEnable(Type);
}

//////////////////////////////////////////////////////////////////