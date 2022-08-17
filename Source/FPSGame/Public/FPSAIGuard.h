// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSAIGuard.generated.h"

class UPawnSensingComponent;

// ����ö��
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

	// �������֪���
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	// �����屻����ʱ��������
	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);

	// ��������ʱ��������
	UFUNCTION()
	void OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume);

	// ��ת��ʼֵ
	FRotator OriginalRotation;

	// ������ת����
	UFUNCTION()
	void ResetRotation();

	// ��ʱ��
	FTimerHandle TimerHandle_ResetOrentation;

	// ö�ٱ���
	// ���ñ�������ΪRep_Notify�����ñ�����������ʱ�������Զ��庯��
	UPROPERTY(ReplicatedUsing = OnRep_GuardState)
	EAIState GuardState;

	// GuardState����ʱ���õ��Զ��庯��
	UFUNCTION()
	void OnRep_GuardState();

	// ��������״̬����
	void SetGuardState(EAIState NewState);

	// ������״̬�ı����
	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnStateChanged(EAIState NewState);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
