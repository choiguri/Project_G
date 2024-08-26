// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "MyCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PawnMovementComponent.h"

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
	
	
	Attacking = false;
	isMoving = false;
	isFar = true;
	
	
	

}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();

	DetectRange->OnComponentBeginOverlap.AddDynamic(this, &AMonster::OnPlayerEnterRange);
	DetectRange->OnComponentEndOverlap.AddDynamic(this, &AMonster::OnPlayerExitRange);

	ActiveState = State::Idle;
}

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	
	TickStateMachine(DeltaTime);
	PrintState();
	
	if (TargetDetected && TargetPlayer && (ActiveState != State::Attack))
	{	
		FVector PlayerLocation = TargetPlayer->GetActorLocation();
		MonsterLocation = GetActorLocation();
		NewLocation = MonsterLocation + GetActorForwardVector() * 140 * DeltaTime;

		// 플레이어 위치와 몬스터 위치를 넣어 회전값을 구할 수 있음 
		FRotator LookPlayer = UKismetMathLibrary::FindLookAtRotation(MonsterLocation, PlayerLocation);
		FRotator Rotation = FRotator(0.0f, LookPlayer.Yaw, 0.0f);

		// 플레이어가 처음 Overlap됐을 때 회전이 너무 빠름 이후에는 부드러움 (수정 필요) or 다른 방법으로 가리기?
		SetActorRotation(Rotation);

		// 플레이어와의 거리가 700 이하이면 정지
		if (FVector::Dist(NewLocation, PlayerLocation) <= 700.0f)
		{
			isFar = false;
			EndMoving();
			
		}
		else
		{
			isFar = true;
			SetActorLocation(NewLocation);
		}
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
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, TEXT("onoverlap"));

			TargetDetected = true;
			TargetPlayer = Player;
			StartMoving();
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
			EndMoving();
			SetState(State::Idle);
		}
	}
}

void AMonster::StartAttack()
{
	if (ActiveState == State::Attack)
		return;
	if (ActiveState != State::Attack)
	{
		isMoving = false;
		Attacking = true;
		SetState(State::Attack);
	} 
	
}

void AMonster::EndAttack()
{
	if (ActiveState != State::Attack)
		return;

	if (isFar == false)
	{
		SetState(State::Attack);
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("isNotFar"));
	}
	if (isFar == true)
	{
		Attacking = false;
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("isFar"));
		SetState(State::Moving);
	}
}

void AMonster::StartMoving()
{
	if (ActiveState == State::Moving)
		return;
	if (ActiveState != State::Moving)
	{
		SetState(State::Moving);
	}
	
}

void AMonster::EndMoving()
{
	if (ActiveState != State::Moving)
		return;
	SetState(State::Attack);
}

void AMonster::Attack()
{
	if (ActiveState != State::Attack)
	{
		return;
	}
	if (ActiveState == State::Attack)
	{
		PlayAttackAnimMontage();
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, TEXT("state ATTack"));
	}

}

void AMonster::Move(float DeltaTime)
{
	if (ActiveState != State::Moving)
	{
		return;
	}
	if (ActiveState == State::Moving && ActiveState !=State::Attack)
	{
		
	}
}

void AMonster::PlayAttackAnimMontage()
{
	if (AttackMontage == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, TEXT("none22"));
		return;
		
	}
	if (SkeletalMesh == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, TEXT("SkeletalMesh is none"));
		return;
	}
	UAnimInstance* AnimInstance = SkeletalMesh->GetAnimInstance();
	if (AnimInstance != nullptr)
	{
		
		AnimInstance->Montage_Play(AttackMontage);
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, TEXT("Playing AttackMontage"));
		

	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, TEXT("none"));
	}
}


void AMonster::TickStateMachine(float DeltaTime)
{
	switch (ActiveState)
	{
	case State::Idle:
		break;
	case State::Attack:
		Attack();
		break;
	case State::Moving:
		Move(DeltaTime);	
		break;
	default:
		break;
	}
}

void AMonster::SetState(State NewState)
{
	// 나중에 죽지 않았으면 이라는 조건 추가 , 죽었을 때의 상태, 애니메이션 추가
	if (ActiveState != NewState)
	{
		ActiveState = NewState;
		switch (ActiveState)
		{
		case State::Idle:
			isMoving = false;
			Attacking = false;
			break;
		case State::Attack:
			Attacking = true;
			isMoving = false;
			StartAttack();
			break;
		case State::Moving:
			isMoving = true;
			Attacking = false;
			StartMoving();
			break;
		default:
			break;
		}
	}
}

// 공격할 때 앞으로 나오면서 때려야할듯? 

// 기본적으로 거리가 일정이상 멀어져있으면 전진, 가까이 있으면 공격, 범위 안에 없으면 대기 상태
// 범위 밖으로 나가니까 공격으로 되어잇음 else 안에 문장 고쳐야할듯


// 디버깅용 State 출력 함수
FString AMonster::EnumToString(State EnumValue)
{
	
	switch (EnumValue)
	{
	case State::Idle:
		return FString("Idle");
	case State::Attack:
		return FString("Attack");
	case State::Moving:
		return FString("Moving");
	default:
		return FString("none");
	}

}
void AMonster::PrintState()
{
	FString EnumString = EnumToString(ActiveState);
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString::Printf(TEXT("State: %s"), *EnumString));
}

// move 일때만 거리를 재서 어택으로 변경함 그래서 move 하고 있는데 캐릭터를 안 움직여서 tick에서 거리가 업뎃이 안되면 
// 공격을 안함 move 함수 밖으로 빼야 될듯 거리 쟤는 것을...