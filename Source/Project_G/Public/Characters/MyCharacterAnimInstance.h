// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyCharacterAnimInstance.generated.h"

class AMyCharacter;
class UCharacterMovementComponent;

UCLASS()
class PROJECT_G_API UMyCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly)
	AMyCharacter* MyCharacter;
	UPROPERTY(BlueprintReadOnly)
	UCharacterMovementComponent* MyCharacterMovement;
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float MoveLength;
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool isJump;
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;
};
