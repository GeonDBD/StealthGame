// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "FPSGameMode.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ʵ������֪���
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	// �󶨿����������¼�
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSeen);

	// �����������¼�
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnNoiseHeard);

	GuardState = EAIState::Idle;  // ����״̬��ʼֵ
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();
	
	OriginalRotation = GetActorRotation();  // Ϊ��ת��ʼֵ��ʼ����ֵ
}

// �����������¼���������
void AFPSAIGuard::OnPawnSeen(APawn* SeenPawn)
{
	if (SeenPawn == nullptr)
	{
		return;
	}
	// ����״̬
	SetGuardState(EAIState::Alerted);

	// ���Ƶ�������
	DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.0f, 12, FColor::Red, false, 5.0f);

	// ��ȡ��ǰ�������Ȩ��Ϸģʽ
	AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GM->CompleteMission(SeenPawn, false);
	}
}

// ���������¼���������
void AFPSAIGuard::OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
	// �ж��Ƿ����ھ���״̬��
	if (GuardState == EAIState::Alerted)
	{
		return;
	}

	// ����״̬
	SetGuardState(EAIState::Suspicious);

	// ���Ƶ�������
	DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Purple, false, 5.0f);

	// �����������
	FVector Direction = Location - GetActorLocation();
	Direction.Normalize();  // ʹ����������һ��

	// ����x�ķ��������ȡ��ת����������pitch��roll����Ϊ0
	FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
	NewLookAt.Pitch = 0.0f;
	NewLookAt.Roll = 0.0f;

	SetActorRotation(NewLookAt);

	// ������ʱ����ʹAI��3�����ת�س�ʼλ��
	// ������ɶ�ʱ������Ϊ�˴���ת���ö�ʱ��
	GetWorldTimerManager().ClearTimer(TimerHandle_ResetOrentation);
	GetWorldTimerManager().SetTimer(TimerHandle_ResetOrentation, this, &AFPSAIGuard::ResetRotation, 3.0f);
}

// ������ת
void AFPSAIGuard::ResetRotation()
{
	if (GuardState == EAIState::Alerted)
	{
		return;
	}
	SetActorRotation(OriginalRotation);
	SetGuardState(EAIState::Idle);
}

// GuardState����ʱ���õ��Զ��庯��ʵ��
void AFPSAIGuard::OnRep_GuardState()
{
	OnStateChanged(GuardState);
}

// ��������״̬
void AFPSAIGuard::SetGuardState(EAIState NewState)
{
	// �жϾ�״̬�Ƿ�ı�
	if (GuardState == NewState)
	{
		return;
	}
	GuardState = NewState;

	OnRep_GuardState();
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// ��ȡ����������������
// ����PropertyReplication�����ƶ�������
void AFPSAIGuard::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPSAIGuard, GuardState);
}