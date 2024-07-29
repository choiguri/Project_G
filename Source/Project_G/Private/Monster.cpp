// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "MyCharacter.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AMonster::AMonster()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(Capsule);

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("DragonMesh"));
	SkeletalMesh->SetupAttachment(GetRootComponent());

	DetectRange = CreateDefaultSubobject<USphereComponent>(TEXT("DetectRange"));
	DetectRange->SetupAttachment(GetRootComponent());
	

}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();

	DetectRange->OnComponentBeginOverlap.AddDynamic(this, &AMonster::OnPlayerEnterRange);
	DetectRange->OnComponentEndOverlap.AddDynamic(this, &AMonster::OnPlayerExitRange);
}

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (TargetDetected && TargetPlayer)
	{
		FVector PlayerLocation = TargetPlayer->GetActorLocation();
		FVector MonsterLocation = GetActorLocation();

		// 플레이어 위치와 몬스터 위치를 넣어 회전값을 구할 수 있음
		FRotator LookPlayer = UKismetMathLibrary::FindLookAtRotation(MonsterLocation, PlayerLocation);
		FRotator Rotation = FRotator(0.0f, LookPlayer.Yaw, 0.0f);

		SetActorRotation(Rotation);
		
		// 새로운 Location 지정, 해당방향으로 이동 (플레이어를 향해 앞으로 전진)
		FVector NewLocation = MonsterLocation + GetActorForwardVector() * 140 * DeltaTime;
		
		// 플레이어와의 거리가 700 이하이면 정지
		if(FVector::Dist(NewLocation, PlayerLocation) > 700.0f) SetActorLocation(NewLocation); 
	
	}

}

// Called to bind functionality to input
void AMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// DetectRange에 Overlap 되었을 때
void AMonster::OnPlayerEnterRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	// Overlap된 대상이 OtherActor 이고 this가 아닐 때 실행
	if (OtherActor && (OtherActor != this))
	{
		AMyCharacter* Player = Cast<AMyCharacter>(OtherActor);

		// OtherActor가 Player이면
		if (Player) {
			GEngine->AddOnScreenDebugMessage(1, 1, FColor::Blue, TEXT("onoverlap"));

			TargetDetected = true;
			TargetPlayer = Player;

		}
	}
	
}

// Detect Range에서 나갔을 때
void AMonster::OnPlayerExitRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this))

	{
		AMyCharacter* Player = Cast<AMyCharacter>(OtherActor);
		if (Player) {
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("exit"));

			TargetDetected = false;
			TargetPlayer = nullptr;

		}
	}
}