// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSAIGuard.generated.h"

class UPawnSensingComponent;

// 定义枚举
UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle,
	Suspicious,
	Alerted
};

UCLASS()
class FPSGAME_API AFPSAIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSAIGuard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 人形体感知组件
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	// 人形体被看见时触发函数
	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);

	// 听到声音时触发函数
	UFUNCTION()
	void OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume);

	// 旋转初始值
	FRotator OriginalRotation;

	// 重置旋转函数
	UFUNCTION()
	void ResetRotation();

	// 定时器
	FTimerHandle TimerHandle_ResetOrentation;

	// 枚举变量
	// 将该变量设置为Rep_Notify，当该变量发生复制时，调用自定义函数
	UPROPERTY(ReplicatedUsing = OnRep_GuardState)
	EAIState GuardState;

	// GuardState复制时调用的自定义函数
	UFUNCTION()
	void OnRep_GuardState();

	// 设置守卫状态函数
	void SetGuardState(EAIState NewState);

	// 当守卫状态改变后函数
	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnStateChanged(EAIState NewState);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
