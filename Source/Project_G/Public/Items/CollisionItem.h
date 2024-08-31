// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/MyItem.h"
#include "Components/SphereComponent.h"
#include "CollisionItem.generated.h"

class USphereComponent;

UCLASS()
class PROJECT_G_API ACollisionItem : public AMyItem
{
	GENERATED_BODY()
	
public:

private:

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComponent;

	/*UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMesh;*/

	UFUNCTION()
	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	ACollisionItem();
	~ACollisionItem();
};
