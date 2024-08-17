// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacterAnimInstance.h"
#include "MyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UMyCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	MyCharacter = Cast<AMyCharacter>(TryGetPawnOwner());
	if (MyCharacter != nullptr)
	{
		MyCharacterMovement = MyCharacter->GetCharacterMovement();
	}
}

void UMyCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	if (MyCharacterMovement != nullptr)
	{
		MoveLength = UKismetMathLibrary::VSizeXY(MyCharacterMovement->Velocity);
		isJump = MyCharacterMovement->IsFalling();
	}
}
