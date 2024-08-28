// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyAnimInstance.generated.h"

class AMyCharacter;
class UCharacterMovementComponent;

UCLASS()
class PROJECT_G_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly)
	AMyCharacter* MyCharacter;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	UCharacterMovementComponent* MyCharacterMovement;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float MoveLength;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool isJump;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool isCrouching;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool isSprint;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool isRolling;
	
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool isStraightMove = true;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool isBackMove = false;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool isLeftMove = false;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool isRightMove = false;

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	void SetStartSprint();
	void SetEndSprint();
	void SetStartRolling();
	void SetEndRolling();
	bool ReturnIsSprint();

	void SetIsStraightMove(bool Value);
	void SetIsBackMove(bool Value);
	void SetIsLeftMove(bool Value);
	void SetIsRightMove(bool Value);
};
