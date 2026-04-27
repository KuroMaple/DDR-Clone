// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChartPlayer.h"
#include "Judge.generated.h"

UENUM()
enum class EHitType : uint8
{
	Perfect,
	Good,
	Miss
};


class AChartPlayer;
/**
 * 
 */
UCLASS()
class RYHTHMTRAINER_API UJudge : public UObject
{
	GENERATED_BODY()
public:
	void Init(AChartPlayer* InChartPlayer);
	EHitType JudgePress(int32 LanePressed, float PressTime);
	void ResetCombo();
	int32 Score = 0;
	int32 Combo = 0;
private:
	UPROPERTY()
	TObjectPtr<AChartPlayer> ChartPlayer;
	
	float GoodHitWindow = 0.25f;
	float PerfectHitWindow = 0.1f;
};
