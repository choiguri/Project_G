// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Monster.generated.h"

class UCapsuleComponent;
class USphereComponent;

UENUM(BlueprintType)
enum class State : uint8
{
	Idle,
	Moving,
	Attack
};

UCLASS()
class PROJECT_G_API AMonster : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMonster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	AActor* TargetPlayer = nullptr;
	
	virtual void TickStateMachine(float DeltaTime);

	void SetState(State NewState);
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* Capsule;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* DetectRange;

public:

	UFUNCTION()
	void OnPlayerEnterRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPlayerExitRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void StartAttack();

	UFUNCTION(BlueprintCallable)
	void EndAttack();

	UFUNCTION()
	void Attack();

	UFUNCTION(BlueprintCallable)
	void StartMoving();

	UFUNCTION(BlueprintCallable)
	void EndMoving();

	UFUNCTION()
	void Move(float DeltaTime);

	UPROPERTY()
	bool TargetDetected = false;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Monster")
	bool Attacking;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Monster")
	bool isMoving;

	UPROPERTY()
	float AttackDamage = 10.0f;

	// attack 시작, 끝, 데미지 충돌 체크?, 데미지에 따른 체력 감소, 에니메이션 추가
	// attck 당했을 때 몬스터 체력 감소

	// 몬스터 rotation 더 자연스럽게? , player를 바라보는 정도가 너무 심함
	// 처음에 overlap되었을 때 순간적으로 rotation 되는 현상 수정해야함
	// box collision이나 collision 할 수 있는 것들을 만들어서 해당 collision과 충돌시 
	// 대미지를 준다 or 대미지를 받는다 일듯
	
	// 몬스터의 공격이 캐릭터에게 대미지를 줬는 지는 어떻게 체크할 것인가?

	// 이후에 공격 패턴 추가 

	void PlayAttackAnimMontage();


	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	UAnimMontage* AttackMontage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Monster")
	State ActiveState;

	FVector MonsterLocation;
	FVector NewLocation;
	bool isFar;


	FString EnumToString(State EnumValue);
	void PrintState();
};
