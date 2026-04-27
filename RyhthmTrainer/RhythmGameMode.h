// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RhythmGameMode.generated.h"

class UHUDWidgetClass;
class UJudge;
class AChartPlayer;
class ASongConductor;
/**
 * 
 */
UCLASS()
class RYHTHMTRAINER_API ARhythmGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ARhythmGameMode();
	
	UPROPERTY(EditDefaultsOnly, Category=Song)
	TSubclassOf<ASongConductor> SongConductorClass;
	
	UPROPERTY()
	TObjectPtr<ASongConductor> SongConductor;
	
	UPROPERTY()
	TObjectPtr<AChartPlayer> ChartPlayer;
	
	UPROPERTY(EditAnywhere, Category=Chart)
	TSubclassOf<AChartPlayer> ChartPlayerBP;
	
	UPROPERTY()
	TObjectPtr<UJudge> Judge;
	
	UPROPERTY(EditAnywhere, Category=UI)
	TSubclassOf<UHUDWidgetClass> HUDWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UHUDWidgetClass> HUDWidget;
	
	UPROPERTY(EditDefaultsOnly, Category=Debug)
	bool bLogSongTime = false;
	
	UPROPERTY(EditAnywhere, Category=Debug)
	bool bRecordingMode;
	
	virtual void BeginPlay() override;
};
