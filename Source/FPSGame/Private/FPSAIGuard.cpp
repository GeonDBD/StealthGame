// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"

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
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();
	
}

// 看见人形体事件触发函数
void AFPSAIGuard::OnPawnSeen(APawn* SeenPawn)
{
	if (SeenPawn == nullptr)
	{
		return;
	}

	// 绘制调试球体
	DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.0f, 12, FColor::Red, false, 5.0f);
}

// 听见声音事件触发函数
void AFPSAIGuard::OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
	// 绘制调试球体
	DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Purple, false, 5.0f);
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}