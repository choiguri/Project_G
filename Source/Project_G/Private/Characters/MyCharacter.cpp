// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MyCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Items/CollisionItem.h"
#include "Items/WeaponItem.h"
#include "Characters/CharacterType.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	OverlapItem = nullptr;
	WeaponItem = nullptr;

	State = ECharacterState::NONE;
	ActionState = ECharacterAction::IDLE;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		UEnhancedInputLocalPlayerSubsystem* EnhancedInputSystem;
		EnhancedInputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

		if (EnhancedInputSystem != nullptr)
		{
			EnhancedInputSystem->AddMappingContext(MappingContext, 0);
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("MappingContext Success"));
			}
		}
		else
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("EnhancedInputSystem is null"));
			}
		}
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance != nullptr)
	{
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &AMyCharacter::HandleOnMontageNotifyBegin);
	}

}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(GetAction, ETriggerEvent::Started, this, &AMyCharacter::EquipWeapon);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AMyCharacter::Attack);
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Started, this, &AMyCharacter::Equip);

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, TEXT("InputComponent setup complete"));
		}
	}
}

//void AMyCharacter::Move(const FInputActionValue& value)
//{
//
//	// Attack�϶� �������� ���ϵ��� ������ ������ Move�� ��� �ʿ����
//	if (ActionState == ECharacterAction::ATTACK)
//		return;
//}


// ��� �ݱ� : FŰ ����
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
		else if(WeaponItem != nullptr)
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

// ���� �Լ� : ���콺 ��Ŭ��
void AMyCharacter::Attack(const FInputActionValue& value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("Attack Started"));

	// ���� �ϴ� ����
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

// Attack �ϴ��� üũ
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

// ����ϴ� �Լ� : XŰ ����
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

void AMyCharacter::HandleOnMontageNotifyBegin(FName a_nNotifyName, const FBranchingPointNotifyPayload& a_pBranchingPayload)
{
	//�޺� �ε��� ����
	m_iComboAttackIndex--;

	// 0���� ���� �� ��Ÿ�� ����
	if (m_iComboAttackIndex < 0)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Stop(0.4f, AttackMontage);
		}
	}
}

// ���� �ִϸ��̼� �ν��Ͻ� ����
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

// ���� �� �κ� �˷���
void AMyCharacter::AttackEnd()
{
	if (ActionState != ECharacterAction::ATTACK)
		return;

	ActionState = ECharacterAction::EQUIP;
}

// ĳ���͸� �⺻ ���·� ����
void AMyCharacter::SetIdle()
{
	if (ActionState == ECharacterAction::IDLE)
		return;

	ActionState = ECharacterAction::IDLE;
}

// ��� ���� �ִϸ��̼� �ν��Ͻ�
void AMyCharacter::PlayEquipMontage()
{
	if (EquipMontage == nullptr)
		return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("PlayEquipMontage Called"));
		AnimInstance->Montage_JumpToSection(FName("Equip"));
		AnimInstance->Montage_Play(EquipMontage);
	}
}

// ��� ���� ���� �ִϸ��̼� �ν��Ͻ�
void AMyCharacter::PlayUnEquipMontage()
{
	if (EquipMontage == nullptr)
		return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("PlayUnEquipMontage Called"));
		AnimInstance->Montage_JumpToSection(FName("UnEquip"));
		AnimInstance->Montage_Play(EquipMontage);
	}
}


// ������ ���� ����

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
