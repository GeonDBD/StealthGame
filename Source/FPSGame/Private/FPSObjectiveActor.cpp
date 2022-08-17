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
	// ������ײΪ����ײ
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	// ������ײΪֻ��ѯ
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	// Ϊ����ͨ��������ײ��Ӧ����Ϊ������
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	// Ϊһ��ͨ��������ײ��Ӧ����Ϊ����pawn���ص�
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereComp->SetupAttachment(MeshComp);

	// 
	SetReplicates(true);
}

// Called when the game starts or when spawned
void AFPSObjectiveActor::BeginPlay()
{
	Super::BeginPlay();
	
	// ����Ч������
	PlayEffects();
}

// ��ϷЧ��
void AFPSObjectiveActor::PlayEffects()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, PickupFX, GetActorLocation());
}

// ����overlap����
void AFPSObjectiveActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	// ����Ч������
	PlayEffects();

	// 
	if (GetLocalRole() == ROLE_Authority)
	{
		// �����ҵĽ�ɫ����ǿ��װ��
		AFPSCharacter* MyCharacter = Cast<AFPSCharacter>(OtherActor);
		if (MyCharacter)
		{
			MyCharacter->bIsCarryingObjective = true;

			Destroy();
		}
	}
}