// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "MyCharacter.h"


AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DetectRange = CreateDefaultSubobject<USphereComponent>(TEXT("DetectRange"));
	DetectRange->SetupAttachment(GetRootComponent());
}


void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	// DetectRange Overlap Function Binding
	DetectRange->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnPlayerEnterRange);
	DetectRange->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnPlayerExitRange);

	CurrentState = States::Idle;
}

void AEnemy::TickStateMachine()
{
	switch (CurrentState)
	{
	case States::Idle:
		break;
	case States::Moving:
		Move();
		break;
	case States::Attack:
		Attack();
		break;
	default:
		break;
	}
}


void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickStateMachine();

}


void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Target BeginOverlap Function
void AEnemy::OnPlayerEnterRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		AMyCharacter* Target = Cast<AMyCharacter>(OtherActor);
		if (Target && GetWorld())
		{
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, TEXT("BeginOverlap"));
			
		}
	}
}

// Target EndOverlap Function
void AEnemy::OnPlayerExitRange(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, TEXT("EndOverlap"));
}

// PlayAttackMontage
void AEnemy::PlayAttackMontage()
{
	UAnimInstance* instance = GetMesh()->GetAnimInstance();
	if (instance != nullptr)
	{
		instance->Montage_Play(AttackMontage);
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("PlayAttackMontage"));
	}
}

void AEnemy::SetState(States NewState)
{
	if (CurrentState != NewState)
	{
		CurrentState = NewState;
	}
	
}

void AEnemy::Attack()
{
	if (CurrentState != States::Attack)
		return;
	PlayAttackMontage();
}

void AEnemy::Move()
{
	if (CurrentState != States::Moving)
		return;
	SetState(States::Attack);
}

