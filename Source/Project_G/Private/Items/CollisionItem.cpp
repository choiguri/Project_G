// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/CollisionItem.h"
#include "Components/SphereComponent.h"
#include "Characters/WCharacter.h"


void ACollisionItem::BeginPlay()
{
	Super::BeginPlay();

	// AddDynamic: 겹쳐졌으면 내가 원하는 함수가 실행되도록 바인딩하는 것
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ACollisionItem::BeginOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ACollisionItem::EndOverlap);
}

void ACollisionItem::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AWCharacter* Character = Cast<AWCharacter>(OtherActor);
	if (Character != nullptr)
	{
		Character->SetOverlapItem(this);

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Overlap detected and item set"));
		}
	}
}

void ACollisionItem::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AWCharacter* Character = Cast<AWCharacter>(OtherActor);
	if (Character != nullptr)
	{
		Character->SetOverlapItem(nullptr);
	}
}

ACollisionItem::ACollisionItem()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereComponent->SetupAttachment(GetRootComponent());
}

ACollisionItem::~ACollisionItem()
{

}