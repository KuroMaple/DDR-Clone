// Fill out your copyright notice in the Description page of Project Settings.


#include "RhythmGameMode.h"

#include "ChartPlayer.h"
#include "Judge.h"
#include "RhythmPawn.h"
#include "HUDWidgetClass.h"
#include "SongConductor.h"

ARhythmGameMode::ARhythmGameMode()
{
	DefaultPawnClass = ARhythmPawn::StaticClass();
	SongConductorClass = ASongConductor::StaticClass();
}

void ARhythmGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	if (!SongConductorClass) return;
	
	SongConductor = GetWorld()->SpawnActor<ASongConductor>(SongConductorClass);
	if (SongConductor)
	{
		if (!ChartPlayerBP)
		{
			UE_LOG(LogTemp, Error, TEXT("GameMode: ChartPlayerBP is not set!"));
			return;
		}
		ChartPlayer = GetWorld()->SpawnActor<AChartPlayer>(ChartPlayerBP);
		ChartPlayer->Conductor = SongConductor;
		Judge = NewObject<UJudge>(this);
		Judge->Init(ChartPlayer);
		
		HUDWidget = CreateWidget<UHUDWidgetClass>(GetWorld()->GetFirstPlayerController(), HUDWidgetClass);
		
		HUDWidget->AddToViewport();
		
		SongConductor->StartSong();
	}
	
	if (bLogSongTime)
	{
		FTimerHandle DebugTimerHandle;
		GetWorldTimerManager().SetTimer(
			DebugTimerHandle,
			[this]
			{
				if (SongConductor)
				{
					UE_LOG(LogTemp, Display, TEXT("SongTime = %.3f"), SongConductor->GetSongTime());
				}
			},0.25f, true);
	}
}
