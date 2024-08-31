// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CharacterAnimInstance.h"
#include "Characters/WCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"


void UCharacterAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();
    WCharacter = Cast<AWCharacter>(TryGetPawnOwner());
    /*if (MyCharacter != nullptr)
    {
        MyCharacterMovement = MyCharacter->GetCharacterMovement();
    }*/
}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
    Super::NativeUpdateAnimation(DeltaTime);

    if (WCharacter != nullptr)
    {
        State = WCharacter->GetState();
    }
    
    

    /*if (MyCharacterMovement != nullptr)
    {
        MoveLength = UKismetMathLibrary::VSizeXY(MyCharacter->GetVelocity());
        isJump = MyCharacter->GetMovementComponent()->IsFalling();
        State = MyCharacter->GetState();
    }*/
}
