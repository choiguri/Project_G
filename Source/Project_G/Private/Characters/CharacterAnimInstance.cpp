// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CharacterAnimInstance.h"
#include "Characters/MyCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"


void UCharacterAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();
    MyCharacter = Cast<AMyCharacter>(TryGetPawnOwner());
    /*if (MyCharacter != nullptr)
    {
        MyCharacterMovement = MyCharacter->GetCharacterMovement();
    }*/
}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
    Super::NativeUpdateAnimation(DeltaTime);

    if (MyCharacter != nullptr)
    {
        State = MyCharacter->GetState();
    }
    
    

    /*if (MyCharacterMovement != nullptr)
    {
        MoveLength = UKismetMathLibrary::VSizeXY(MyCharacter->GetVelocity());
        isJump = MyCharacter->GetMovementComponent()->IsFalling();
        State = MyCharacter->GetState();
    }*/
}
