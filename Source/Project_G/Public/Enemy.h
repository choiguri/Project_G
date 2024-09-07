// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

class UCapsuleComponent;
class USphereComponent;

UENUM(BlueprintType)
enum class States : uint8
{
	Idle,
	Moving,
	Attack
};

UCLASS()
class PROJECT_G_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void TickStateMachine();

private:

	UPROPERTY(VisibleAnywhere)
	USphereComponent* DetectRange;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	// Overlap Functions
	UFUNCTION()
	void OnPlayerEnterRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPlayerExitRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Set AttackMontage In BluePrint
	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	UAnimMontage* AttackMontage;

	// PlayAttackMontage Function
	UFUNCTION()
	void PlayAttackMontage();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	States CurrentState;

	UFUNCTION()
	void SetState(States NewState);

	UFUNCTION()
	void Attack();

	UFUNCTION()
	void Move();


};
