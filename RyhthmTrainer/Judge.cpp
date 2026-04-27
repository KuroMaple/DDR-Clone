// Fill out your copyright notice in the Description page of Project Settings.


#include "Judge.h"


void UJudge::Init(AChartPlayer* InChartPlayer)
{
	if (!InChartPlayer)
	{
		UE_LOG(LogTemp, Error, TEXT("Judge::Init - ChartPlyaer is null"));
		return;
	}
	ChartPlayer = InChartPlayer;
}

EHitType UJudge::JudgePress(int32 LanePressed, float PressTime)
{
	// Find the next note in lane:
	FNoteData* NextNote = nullptr;
	for (FNoteData& CurrNote : ChartPlayer->Chart)
	{
		if (CurrNote.NoteState == ENoteState::Active)
		{
			if (CurrNote.HitTime > PressTime + GoodHitWindow)
			{
				ResetCombo();
				break; // Press was way too early
			}
			if (CurrNote.Lane == LanePressed)
			{
				NextNote = &CurrNote;
				break;
			}
		}
	}
	
	if (!NextNote) return EHitType::Miss;
	
	// Measure hit accuracy
	float Threshold = FMath::Abs(NextNote->HitTime - PressTime);
	if (Threshold < PerfectHitWindow)
	{
		NextNote->NoteState = ENoteState::Hit;
		Score += 2;
		++Combo;
		return EHitType::Perfect;
	}
	if (Threshold < GoodHitWindow)
	{
		NextNote->NoteState = ENoteState::Hit;
		Score += 1;
		++Combo;
		return EHitType::Good;
	}
	
	ResetCombo();
	NextNote->NoteState = ENoteState::Missed;
	return EHitType::Miss;
}

void UJudge::ResetCombo()
{
	Combo = 0;
}
