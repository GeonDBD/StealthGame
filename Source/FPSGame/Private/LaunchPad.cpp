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
	// 构造弹射速率
	FRotator LaunchDirection = GetActorRotation();  // 弹射方向
	LaunchDirection.Pitch += LaunchPitchAngle;      // 让Actor不直接按旋转量旋转，而是朝某一角度进行旋转
	FVector LaunchVelocity = LaunchDirection.Vector() * LaunchStrength;  // 将旋转量变为矢量

	AFPSCharacter* OtherCharacter = Cast<AFPSCharacter>(OtherActor);
	if (OtherCharacter)
	{
		OtherCharacter->LaunchCharacter(LaunchVelocity, true, true);

		// 原地生成发射器
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ActicateLaunchPadEffect, GetActorLocation());
	}
	else if (OtherComp && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulse(LaunchVelocity, NAME_None, true);

		// 原地生成发射器
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ActicateLaunchPadEffect, GetActorLocation());
	}
}
