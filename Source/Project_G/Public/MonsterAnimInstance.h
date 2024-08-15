// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MonsterAnimInstance.generated.h"

/**
 * 
 */
class AMonster;
UCLASS()
class PROJECT_G_API UMonsterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public :
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;
	
	

protected:
	UPROPERTY(BlueprintReadWrite)
	AMonster* MyMonster1;
};
