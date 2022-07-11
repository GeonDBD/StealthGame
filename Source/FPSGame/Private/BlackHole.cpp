// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackHole.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ABlackHole::ABlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	// 禁用碰撞
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;
	
	// SphereComp_1 为黑洞外层
	SphereComp_1 = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp_1"));
	SphereComp_1->SetSphereRadius(3000);
	SphereComp_1->SetupAttachment(MeshComp);
	
	// SphereComp_2 为黑洞内层
	SphereComp_2 = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp_2"));
	SphereComp_2->SetSphereRadius(100);
	// 绑定重叠事件函数：在重叠发生开始时添加动态事件
	SphereComp_2->OnComponentBeginOverlap.AddDynamic(this, &ABlackHole::OverLapSphere);
	SphereComp_2->SetupAttachment(MeshComp);
}

// Called when the game starts or when spawned
void ABlackHole::BeginPlay()
{
	Super::BeginPlay();
	
}

// 销毁球体函数
void ABlackHole::OverLapSphere(UPrimitiveComponent* OverLappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	int32 OtherBoxIndex, bool bFromSweep, const FHitResult& SweepRsult)
{
	if (OtherActor)
	{
		OtherActor->Destroy();
	}
}

// Called every frame
void ABlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// UPrimitiveComponent 指的是一类组件，能拥有变换
	// 设置了一种类型的数组
	TArray<UPrimitiveComponent*> OverLappingComps;

	// 为外层球组件获取与之重叠的组件，存为数组
	SphereComp_1->GetOverlappingComponents(OverLappingComps);

	// OverLappingComps.Num() 数组长度
	int32 OLC_Length = OverLappingComps.Num();
	for (int32 i = 0; i < OLC_Length; i++)
	{
		UPrimitiveComponent* PrimComp = OverLappingComps[i];

		// 判断是否为空组件和是否开启了模拟物理
		if (PrimComp && PrimComp->IsSimulatingPhysics())
		{
			const float SphereRadius = SphereComp_1->GetScaledSphereRadius(); // 获取球体范围
			const float ForceStrength = -3500;  // 径向力强度

			// 对其他物体添加一个径向力
			PrimComp->AddRadialForce(GetActorLocation(), SphereRadius, ForceStrength, ERadialImpulseFalloff::RIF_Constant, true);
		}
	}
}

