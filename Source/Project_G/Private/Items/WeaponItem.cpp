// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/WeaponItem.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Characters/MyCharacter.h"
#include "Kismet/KismetSystemLibrary.h"


void AWeaponItem::BeginPlay()
{
	Super::BeginPlay();
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, SkeletalMesh ? TEXT("SkeletalMesh is valid") : TEXT("SkeletalMesh is null"));
	}

	// AddDynamic : 겹쳐졌으면 먼저 실행하도록 하는거
	AttackCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AWeaponItem::BoxBeginOverlap);
}


void AWeaponItem::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::BeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if (GEngine != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Blue, TEXT("overlap true : ") + OtherActor->GetName());
	}
}

void AWeaponItem::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::EndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	if (GEngine != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Blue, TEXT("overlap false : ") + OtherActor->GetName());
	}
}

void AWeaponItem::BoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GEngine != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(2, 2.f, FColor::Magenta, TEXT("Attack Collsion Overlap"));
	}

	// 이건 너무 어려우니 블루 프린트로 하자
	FHitResult BoxHit;
	UKismetSystemLibrary::BoxTraceSingle(this,
		BoxTraceStart->GetComponentLocation(),	BoxTraceEnd->GetComponentLocation(),
		FVector{2.f, 2.f, 2.f},					BoxTraceStart->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,		false,
		{},										EDrawDebugTrace::ForDuration,
		BoxHit,									true
	);

	if (BoxHit.bBlockingHit == true)
	{
		if (GEngine != nullptr)
			GEngine->AddOnScreenDebugMessage(3, 2.f, FColor::Magenta, BoxHit.ImpactPoint.ToString());
	}
}


USkeletalMeshComponent* AWeaponItem::Equip(USceneComponent* Parent, FName SocketName)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, TEXT("Equip function called"));
	}

	SetMashSocket(Parent, SocketName);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	return SkeletalMesh;
}

void AWeaponItem::SetMashSocket(USceneComponent* Parent, FName SocketName)
{
	SkeletalMesh->AttachToComponent(Parent, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), SocketName);
}

// AttackBoxCollision 선택적으로 활성화 되도록
void AWeaponItem::SetAttackBoxCollisionEnable(ECollisionEnabled::Type Type)
{
	AttackCollisionBox->SetCollisionEnabled(Type);
}


AWeaponItem::AWeaponItem()
{
	USceneComponent* PrevRootComponent = GetRootComponent();
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SetRootComponent(SkeletalMesh);
	if (PrevRootComponent != nullptr)
	{
		PrevRootComponent->DestroyComponent();
	}
	SphereComponent->SetupAttachment(SkeletalMesh);

	AttackCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollision"));
	AttackCollisionBox->SetupAttachment(GetRootComponent());

	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("BoxTraceStart"));
	BoxTraceStart->SetupAttachment(GetRootComponent());

	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("BoxTraceEnd"));
	BoxTraceEnd->SetupAttachment(GetRootComponent());


	AttackCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackCollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	AttackCollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	/*
	QueryOnly는 이 컴포넌트가 물리적 충돌을 계산하지 않지만, 오버랩 이벤트는 발생시킴
	AttackCollisionBox의 모든 충돌 채널에 대한 응답을 Overlap으로 설정
	AttackCollisionBox가 ECC_Pawn 충돌 채널에 대해 응답하지 않도록 설정합니다.
	ECR (ECollisionResponse):충돌 응답을 정의하는 열거형
	ECC (ECollisionChannel):충돌 채널을 정의하는 열거형
	*/
}

AWeaponItem::~AWeaponItem()
{

}


