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
	// ������ײ
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;
	
	// SphereComp_1 Ϊ�ڶ����
	SphereComp_1 = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp_1"));
	SphereComp_1->SetSphereRadius(3000);
	SphereComp_1->SetupAttachment(MeshComp);
	
	// SphereComp_2 Ϊ�ڶ��ڲ�
	SphereComp_2 = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp_2"));
	SphereComp_2->SetSphereRadius(100);
	// ���ص��¼����������ص�������ʼʱ��Ӷ�̬�¼�
	SphereComp_2->OnComponentBeginOverlap.AddDynamic(this, &ABlackHole::OverLapSphere);
	SphereComp_2->SetupAttachment(MeshComp);
}

// Called when the game starts or when spawned
void ABlackHole::BeginPlay()
{
	Super::BeginPlay();
	
}

// �������庯��
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

	// UPrimitiveComponent ָ����һ���������ӵ�б任
	// ������һ�����͵�����
	TArray<UPrimitiveComponent*> OverLappingComps;

	// Ϊ����������ȡ��֮�ص����������Ϊ����
	SphereComp_1->GetOverlappingComponents(OverLappingComps);

	// OverLappingComps.Num() ���鳤��
	int32 OLC_Length = OverLappingComps.Num();
	for (int32 i = 0; i < OLC_Length; i++)
	{
		UPrimitiveComponent* PrimComp = OverLappingComps[i];

		// �ж��Ƿ�Ϊ��������Ƿ�����ģ������
		if (PrimComp && PrimComp->IsSimulatingPhysics())
		{
			const float SphereRadius = SphereComp_1->GetScaledSphereRadius(); // ��ȡ���巶Χ
			const float ForceStrength = -3500;  // ������ǿ��

			// �������������һ��������
			PrimComp->AddRadialForce(GetActorLocation(), SphereRadius, ForceStrength, ERadialImpulseFalloff::RIF_Constant, true);
		}
	}
}

