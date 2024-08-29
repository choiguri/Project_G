#pragma once

#include "CoreMinimal.h"
#include "CharacterType.generated.h"

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	NONE UMETA(DisplayName = "None"),
	ONE_HAND_WEAPON UMETA(DisplayName = "One Hand Weapon"),
	UNEQUIP_WEAPON UMETA(DisplayName = "Unequip Weapon")
};

UENUM(BlueprintType)
enum class ECharacterAction : uint8
{
	IDLE,
	ATTACK,
	EQUIP,
	UNEQUIP
};