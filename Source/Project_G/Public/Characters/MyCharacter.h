// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "MyCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UMyAnimInstance;
class UAnimMontage;

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

	FTimerHandle SprintDrainTimerHandle;
	FTimerHandle SprintRegenTimerHandle;

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Move(const FInputActionValue& value);
	void Rotation(const FInputActionValue& value);
	UFUNCTION(BlueprintCallable, Category = "Character")
	void StartSprint();
	UFUNCTION(BlueprintCallable, Category = "Character")
	void EndSprint();
	void StartRoll(const FInputActionValue& value);
	void EndRoll(const FInputActionValue& value);
	void DrainStamina();
	void RegenStamina();
	void OnSprintDrainDelayElapsed();
	void OnSprintRegenDelayElapsed();
	void UpdateAnimInstance(const FVector2D& MoveVector2D);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
