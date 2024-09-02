// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MyAnimInstance.h"
#include "Characters/MyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UMyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	MyCharacter = Cast<AMyCharacter>(TryGetPawnOwner());
	if (MyCharacter != nullptr)
	{
		MyCharacterMovement = MyCharacter->GetCharacterMovement();
	}
}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	if (MyCharacterMovement != nullptr)
	{
		MoveLength = UKismetMathLibrary::VSizeXY(MyCharacterMovement->Velocity);
		isJump = MyCharacterMovement->IsFalling();
		isCrouching = MyCharacterMovement->IsCrouching();
		static int32 MessageKey = 1;
		FString JumpStateString = FString::Printf(TEXT("isJump: %s"), isJump ? TEXT("True") : TEXT("False"));
		GEngine->AddOnScreenDebugMessage(10, 1.0f, FColor::Red, JumpStateString);
		// Display the debug messages on screen
		//FString StraightMoveText = FString::Printf(TEXT("isStraightMove: %s"), isStraightMove ? TEXT("True") : TEXT("False"));
		//FString BackMoveText = FString::Printf(TEXT("isBackMove: %s"), isBackMove ? TEXT("True") : TEXT("False"));
		//FString LeftMoveText = FString::Printf(TEXT("isLeftMove: %s"), isLeftMove ? TEXT("True") : TEXT("False"));
		//FString RightMoveText = FString::Printf(TEXT("isRightMove: %s"), isRightMove ? TEXT("True") : TEXT("False"));

		//GEngine->AddOnScreenDebugMessage(MessageKey++, 1.0f, FColor::Red, StraightMoveText);
		//GEngine->AddOnScreenDebugMessage(MessageKey++, 1.0f, FColor::Green, BackMoveText);
		//GEngine->AddOnScreenDebugMessage(MessageKey++, 1.0f, FColor::Blue, LeftMoveText);
		//GEngine->AddOnScreenDebugMessage(MessageKey++, 1.0f, FColor::Yellow, RightMoveText);
	}
}

void UMyAnimInstance::SetStartSprint()
{
	isSprint = true;
}

void UMyAnimInstance::SetEndSprint()
{
	isSprint = false;
}
void UMyAnimInstance::SetStartRolling()
{
	isRolling = true;
}

void UMyAnimInstance::SetEndRolling()
{
	isRolling = false;
}

bool UMyAnimInstance::ReturnIsSprint()
{
	return isSprint;
}

void UMyAnimInstance::SetIsStraightMove(bool Value)
{
	isStraightMove = Value;
}

void UMyAnimInstance::SetIsBackMove(bool Value)
{
	isBackMove = Value;
}

void UMyAnimInstance::SetIsLeftMove(bool Value)
{
	isLeftMove = Value;
}

void UMyAnimInstance::SetIsRightMove(bool Value)
{
	isRightMove = Value;
}
