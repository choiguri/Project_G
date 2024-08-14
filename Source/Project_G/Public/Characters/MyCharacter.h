// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CharacterType.h"
#include "MyCharacter.generated.h"

class ACollisionItem;
class UInputMappingContext;
class UInputAction;
class AWeaponItem;

UCLASS()
class PROJECT_G_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// �浹���� item ������ ������ Set�Լ� ����
	void SetOverlapItem(ACollisionItem* _overlapItem) { OverlapItem = _overlapItem; }
	void EquipWeapon();
	void Equip();

	// Anim Montage�� ����� �Լ� ����
	void PlayAttackMontage();
	void PlayEquipMontage();
	void PlayUnEquipMontage();

	// ĳ���� ���� ��������
	ECharacterState GetState() const { return State; }

	// BlueprintCallable ==> ��� ����Ʈ���� ���� ��밡����
	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	// ĳ���� ���� ����
	UFUNCTION(BlueprintCallable)
	void SetIdle();

	UFUNCTION(BlueprintCallable)
	void EquipWeaponMesh();

	UFUNCTION(BlueprintCallable)
	void UnEquipWeaponMesh();

	UFUNCTION(BlueprintCallable)
	void SetAttackCollisionEnable(ECollisionEnabled::Type Type);

	UFUNCTION()
	void HandleOnMontageNotifyBegin(FName a_nNotifyName, const FBranchingPointNotifyPayload& a_pBranchingPayload);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Attack(const FInputActionValue& value);
	/*void Move(const FInputActionValue& value);*/

	UPROPERTY(BlueprintReadWrite)
	AMyCharacter* MyCharacter;

	bool IsAttacking();
	// void lightAttack();
	int m_iComboAttackIndex = 0;


private:
	UPROPERTY(VisibleInstanceOnly)
	ACollisionItem* OverlapItem;

	AWeaponItem* WeaponItem;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	UAnimMontage* EquipMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* MappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "input", meta = (AllowPrivateAccess = "true"))
	UInputAction* GetAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "input", meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "input", meta = (AllowPrivateAccess = "true"))
	UInputAction* EquipAction;

	ECharacterState State;
	ECharacterAction ActionState;
};