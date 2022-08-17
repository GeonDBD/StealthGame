// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSObjectiveActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FPSCharacter.h"

// Sets default values
AFPSObjectiveActor::AFPSObjectiveActor()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	// 设置碰撞为无碰撞
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	// 设置碰撞为只查询
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	// 为所有通道设置碰撞响应设置为：忽略
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	// 为一个通道设置碰撞响应设置为：给pawn，重叠
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereComp->SetupAttachment(MeshComp);

	// 
	SetReplicates(true);
}

// Called when the game starts or when spawned
void AFPSObjectiveActor::BeginPlay()
{
	Super::BeginPlay();
	
	// 调用效果函数
	PlayEffects();
}

// 游戏效果
void AFPSObjectiveActor::PlayEffects()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, PickupFX, GetActorLocation());
}

// 重载overlap函数
void AFPSObjectiveActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	// 调用效果函数
	PlayEffects();

	// 
	if (GetLocalRole() == ROLE_Authority)
	{
		// 定义我的角色，并强制装换
		AFPSCharacter* MyCharacter = Cast<AFPSCharacter>(OtherActor);
		if (MyCharacter)
		{
			MyCharacter->bIsCarryingObjective = true;

			Destroy();
		}
	}
}