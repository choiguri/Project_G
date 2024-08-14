// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/CollisionItem.h"
#include "Components/SphereComponent.h"
#include "Characters/MyCharacter.h"


void ACollisionItem::BeginPlay()
{
	Super::BeginPlay();

	// AddDynamic: ���������� ���� ���ϴ� �Լ��� ����ǵ��� ���ε��ϴ� ��
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ACollisionItem::BeginOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ACollisionItem::EndOverlap);
}

void ACollisionItem::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMyCharacter* Character = Cast<AMyCharacter>(OtherActor);
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
	AMyCharacter* Character = Cast<AMyCharacter>(OtherActor);
	if (Character != nullptr)
	{
		//�浹�� ������ null������ set��
		Character->SetOverlapItem(nullptr);
	}
}

ACollisionItem::ACollisionItem()
{
	// �浹�� ����� ���� ���� �� �Ҵ�
	/*StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));*/

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereComponent->SetupAttachment(GetRootComponent());
}

ACollisionItem::~ACollisionItem()
{

}