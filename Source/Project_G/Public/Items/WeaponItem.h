// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/CollisionItem.h"
#include "WeaponItem.generated.h"


class UBoxComponent;

UCLASS()
class PROJECT_G_API AWeaponItem : public ACollisionItem
{
	GENERATED_BODY()

private:
	// 소켓에 넣기 위한 스켈레탈 메시
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* SkeletalMesh;
	
	// 무기 피격 판정을 위한 콜리전
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	UBoxComponent* AttackCollisionBox;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;


protected:
	virtual void BeginPlay() override;

	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void BoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	USkeletalMeshComponent* Equip(USceneComponent* Parent, FName SocketName);
	void SetMashSocket(USceneComponent* Parent, FName SocketName);
	void SetAttackBoxCollisionEnable(ECollisionEnabled::Type Type);

	AWeaponItem();
	~AWeaponItem();

};
