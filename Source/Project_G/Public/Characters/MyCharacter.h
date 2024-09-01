// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CharacterType.h"
#include "MyCharacter.generated.h"

// Input 관련
class UInputMappingContext;
class UInputAction;

// 컨트롤 관련
class USpringArmComponent;
class UCameraComponent;
class UMyAnimInstance;
class UAnimMontage;

// 전투 관련
class AWeaponItem;
class ACollisionItem;

UCLASS()
class PROJECT_G_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	UMyAnimInstance* MyAnimInstance;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "input")
	UInputMappingContext* MappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "input")
	UInputAction* RotationAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "input")
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "input")
	UInputAction* SprintAction;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "input")
	//UInputAction* RollAction;
	//UAnimMontage* RollMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float WalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float SprintSpeed;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	//float RollSpeed;  // Roll 속도
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	//float RollDistance;  // Roll 거리

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	int Stamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	int MaxStamina;

	// 전투관련
	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	UAnimMontage* AttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	UAnimMontage* EquipMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "input", meta = (AllowPrivateAccess = "true"))
	UInputAction* GetAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "input", meta = (AllowPrivateAccess = "true"))
	UInputAction* AttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "input", meta = (AllowPrivateAccess = "true"))
	UInputAction* EquipAction;

	UPROPERTY(VisibleInstanceOnly)
	ACollisionItem* OverlapItem;

	AWeaponItem* WeaponItem;

	ECharacterState State;
	ECharacterAction ActionState;

	void Attack(const FInputActionValue& value);
	/*void Move(const FInputActionValue& value);*/

	UPROPERTY(BlueprintReadWrite)
	AMyCharacter* MyCharacter;

	bool IsAttacking();
	// void lightAttack();
	int m_iComboAttackIndex = 0;
	// 전투 관련 끝


	FTimerHandle SprintDrainTimerHandle;
	FTimerHandle SprintRegenTimerHandle;

public:
	AMyCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 전투관련
	// 충돌중인 item 저장할 변수와 Set함수 선언
	void SetOverlapItem(ACollisionItem* _overlapItem) { OverlapItem = _overlapItem; }
	void EquipWeapon();
	void Equip();
	// AnimMontage에 사용할 함수 선언
	void PlayAttackMontage();
	void PlayEquipMontage();
	void PlayUnEquipMontage();
	// 상태 받아오기
	ECharacterState GetState() const { return State; }
	// BlueprintCallable ==> 블루 프린트에서 노드로 사용가능함
	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	// 캐릭터 상태 정리
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
	//전투 관련 끝

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Move(const FInputActionValue& value);
	void Rotation(const FInputActionValue& value);
	void StartRoll(const FInputActionValue& value);
	void EndRoll(const FInputActionValue& value);
	void DrainStamina();
	void RegenStamina();
	void OnSprintDrainDelayElapsed();
	void OnSprintRegenDelayElapsed();
	void UpdateAnimInstance(const FVector2D& MoveVector2D);

	UFUNCTION(BlueprintCallable, Category = "Character")
	void StartSprint();

	UFUNCTION(BlueprintCallable, Category = "Character")
	void EndSprint();
};
