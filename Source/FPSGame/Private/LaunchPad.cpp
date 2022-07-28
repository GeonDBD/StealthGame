// Fill out your copyright notice in the Description page of Project Settings.


#include "LaunchPad.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "FPSCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALaunchPad::ALaunchPad()
{
	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	OverlapComp->SetBoxExtent(FVector(100.0f, 100.0f, 20.0f));
	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &ALaunchPad::HandleOverlap);
	RootComponent = OverlapComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(OverlapComp);

	LaunchStrength = 1800;
	LaunchPitchAngle = 45.0f;
}

void ALaunchPad::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// ���쵯������
	FRotator LaunchDirection = GetActorRotation();  // ���䷽��
	LaunchDirection.Pitch += LaunchPitchAngle;      // ��Actor��ֱ�Ӱ���ת����ת�����ǳ�ĳһ�ǶȽ�����ת
	FVector LaunchVelocity = LaunchDirection.Vector() * LaunchStrength;  // ����ת����Ϊʸ��

	AFPSCharacter* OtherCharacter = Cast<AFPSCharacter>(OtherActor);
	if (OtherCharacter)
	{
		OtherCharacter->LaunchCharacter(LaunchVelocity, true, true);

		// ԭ�����ɷ�����
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ActicateLaunchPadEffect, GetActorLocation());
	}
	else if (OtherComp && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulse(LaunchVelocity, NAME_None, true);

		// ԭ�����ɷ�����
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ActicateLaunchPadEffect, GetActorLocation());
	}
}
