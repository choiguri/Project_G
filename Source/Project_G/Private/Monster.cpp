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
	

}

// Called to bind functionality to input
void AMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMonster::OnPlayerEnterRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (OtherActor && (OtherActor != this))
	{
		AMyCharacter* Player = Cast<AMyCharacter>(OtherActor);
		if (Player) {
			GEngine->AddOnScreenDebugMessage(1, 1, FColor::Blue, TEXT("onoverlap"));
		}
		FVector PlayerLocation = Player->GetActorLocation();
		FVector MonsterLocation = GetActorLocation();
		FRotator LookPlayer = UKismetMathLibrary::FindLookAtRotation(MonsterLocation, PlayerLocation);

		SetActorRotation(FRotator(0.0f, LookPlayer.Yaw, 0.0f));
		
	}
	
}

void AMonster::OnPlayerExitRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this))

	{
		AMyCharacter* Player = Cast<AMyCharacter>(OtherActor);
		if (Player) {
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("exit"));
		}
	}
}