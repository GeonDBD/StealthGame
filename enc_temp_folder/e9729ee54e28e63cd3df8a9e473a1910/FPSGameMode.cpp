// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();
}

void AFPSGameMode::CompleteMission(APawn* InstigatorPawn)
{
	if (InstigatorPawn)
	{
		// 禁用pawn输入
		// 专门用来禁用玩家控制器控制Pawn
		InstigatorPawn->DisableInput(nullptr);

		if (SpectatingViewpointClass)
		{
			// 获取类的所有Actor，定义一个规定的子类存放在数组中
			TArray<AActor*> ReturnedActors;
			UGameplayStatics::GetAllActorsOfClass(this, SpectatingViewpointClass, ReturnedActors);

			// 找到任何有效的Actor，更改目标视角
			if (ReturnedActors.Num() > 0)
			{
				AActor* NewViewTarget = ReturnedActors[0];

				// 获取Pawn的控制器并转换为玩家控制器
				APlayerController* PC = Cast<APlayerController>(InstigatorPawn->GetController());
				if (PC)
				{
					// 使用Blend设置视图目标
					PC->SetViewTargetWithBlend(NewViewTarget, 0.5f, EViewTargetBlendFunction::VTBlend_Cubic);
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("SpectatingViewpointClass is nullptr. Please update GameMode class with valid subclass. Cannot change spectating view target."));
		}
	}

	OnMissionCompleted(InstigatorPawn);
}
