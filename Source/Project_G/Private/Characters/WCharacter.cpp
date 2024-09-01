// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/WCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Items/CollisionItem.h"
#include "Items/WeaponItem.h"
#include "Characters/CharacterType.h"

// Sets default values
AWCharacter::AWCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	OverlapItem = nullptr;
	WeaponItem = nullptr;

	State = ECharacterState::NONE;
	ActionState = ECharacterAction::IDLE;
}

// Called when the game starts or when spawned
void AWCharacter::BeginPlay()
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
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &AWCharacter::HandleOnMontageNotifyBegin);
	}
}

// Called every frame
void AWCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AWCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(GetAction, ETriggerEvent::Started, this, &AWCharacter::EquipWeapon);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AWCharacter::Attack);
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Started, this, &AWCharacter::Equip);

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, TEXT("InputComponent setup complete"));
		}
	}
}

//void AWCharacter::Move(const FInputActionValue& value)
//{
//
//	// Attack일때 움직이지 못하도록 하지만 나한텐 Move가 없어서 필요없음
//	if (ActionState == ECharacterAction::ATTACK)
//		return;
//}


// 장비 줍기 : F키 누름
void AWCharacter::EquipWeapon()
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

// 공격 함수 : 마우스 좌클릭
void AWCharacter::Attack(const FInputActionValue& value)
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
bool AWCharacter::IsAttacking()
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

// 장비하는 함수 : X키 누름
void AWCharacter::Equip()
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

void AWCharacter::HandleOnMontageNotifyBegin(FName a_nNotifyName, const FBranchingPointNotifyPayload& a_pBranchingPayload)
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

// 공격 애니메이션 인스턴스 실행
void AWCharacter::PlayAttackMontage()
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

// 공격 끝 부분 알려줌
void AWCharacter::AttackEnd()
{
	if (ActionState != ECharacterAction::ATTACK)
		return;

	ActionState = ECharacterAction::EQUIP;
}

// 캐릭터를 기본 상태로 정렬
void AWCharacter::SetIdle()
{
	if (ActionState == ECharacterAction::IDLE)
		return;

	ActionState = ECharacterAction::IDLE;
}

// 장비 장착 애니메이션 인스턴스
void AWCharacter::PlayEquipMontage()
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
void AWCharacter::PlayUnEquipMontage()
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


// 아이템 장착 소켓
void AWCharacter::EquipWeaponMesh()
{
	if (WeaponItem != nullptr)
		WeaponItem->SetMashSocket(GetMesh(), FName("SpineSocket"));
}

void AWCharacter::UnEquipWeaponMesh()
{
	if (WeaponItem != nullptr)
		WeaponItem->SetMashSocket(GetMesh(), FName("WeaponSocket"));
}

void AWCharacter::SetAttackCollisionEnable(ECollisionEnabled::Type Type)
{
	if (WeaponItem != nullptr)
		WeaponItem->SetAttackBoxCollisionEnable(Type);
}
