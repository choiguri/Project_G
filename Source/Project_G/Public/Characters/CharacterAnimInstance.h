// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterType.h"
#include "CharacterAnimInstance.generated.h"


class AWCharacter;
//class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class PROJECT_G_API UCharacterAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    virtual void NativeInitializeAnimation() override;
    virtual void NativeUpdateAnimation(float DeltaTime) override;

protected:
    UPROPERTY(BlueprintReadWrite)
    AWCharacter* WCharacter;

    /*UPROPERTY(BlueprintReadOnly, Category = "Movement")
    AMyCharacter* MyCharacter;*/

    /*UPROPERTY(BlueprintReadOnly, Category = "Movement")
    UCharacterMovementComponent* MyCharacterMovement;

    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    float MoveLength;

    UPROPERTY(BlueprintReadOnly, Category = "Movement")
    bool isJump;*/


    UPROPERTY(BlueprintReadOnly, Category = "CharacterState")
    ECharacterState State;
};
