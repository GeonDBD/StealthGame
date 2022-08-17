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

	// 实例化感知组件
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	// 绑定看见人形体事件
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSeen);

	// 绑定听见声音事件
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnNoiseHeard);

	GuardState = EAIState::Idle;  // 守卫状态初始值
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();
	
	OriginalRotation = GetActorRotation();  // 为旋转初始值初始化赋值
}

// 看见人形体事件触发函数
void AFPSAIGuard::OnPawnSeen(APawn* SeenPawn)
{
	if (SeenPawn == nullptr)
	{
		return;
	}
	// 设置状态
	SetGuardState(EAIState::Alerted);

	// 绘制调试球体
	DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.0f, 12, FColor::Red, false, 5.0f);

	// 获取当前世界的授权游戏模式
	AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GM->CompleteMission(SeenPawn, false);
	}
}

// 听见声音事件触发函数
void AFPSAIGuard::OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
	// 判断是否已在警戒状态下
	if (GuardState == EAIState::Alerted)
	{
		return;
	}

	// 设置状态
	SetGuardState(EAIState::Suspicious);

	// 绘制调试球体
	DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Purple, false, 5.0f);

	// 创建方向变量
	FVector Direction = Location - GetActorLocation();
	Direction.Normalize();  // 使方向向量归一化

	// 根据x的方向变量获取旋转变量，并将pitch和roll设置为0
	FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
	NewLookAt.Pitch = 0.0f;
	NewLookAt.Roll = 0.0f;

	SetActorRotation(NewLookAt);

	// 创建定时器，使AI在3秒后旋转回初始位置
	// 先清除旧定时器，再为此次旋转设置定时器
	GetWorldTimerManager().ClearTimer(TimerHandle_ResetOrentation);
	GetWorldTimerManager().SetTimer(TimerHandle_ResetOrentation, this, &AFPSAIGuard::ResetRotation, 3.0f);
}

// 重置旋转
void AFPSAIGuard::ResetRotation()
{
	if (GuardState == EAIState::Alerted)
	{
		return;
	}
	SetActorRotation(OriginalRotation);
	SetGuardState(EAIState::Idle);
}

// GuardState复制时调用的自定义函数实现
void AFPSAIGuard::OnRep_GuardState()
{
	OnStateChanged(GuardState);
}

// 设置守卫状态
void AFPSAIGuard::SetGuardState(EAIState NewState)
{
	// 判断旧状态是否改变
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

// 获取复制生命周期属性
// 开启PropertyReplication，复制对象属性
void AFPSAIGuard::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPSAIGuard, GuardState);
}