// Fill out your copyright notice in the Description page of Project Settings.


#include "ChartPlayer.h"

#include "HUDWidgetClass.h"
#include "Judge.h"
#include "NoteActor.h"
#include "ReceptacleActor.h"
#include "RhythmGameMode.h"
#include "SongConductor.h"
#include "Kismet/GameplayStatics.h"

AChartPlayer::AChartPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AChartPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	GameMode = Cast<ARhythmGameMode>(GetWorld()->GetAuthGameMode());
	// === Full Recorded Song ===
	
	// Demo portion
	Chart.Add({0, 3.464f, ENoteState::Upcoming});
	Chart.Add({3, 5.348f, ENoteState::Upcoming});
	Chart.Add({0, 7.849f, ENoteState::Upcoming});
	Chart.Add({2, 8.258f, ENoteState::Upcoming});
	Chart.Add({0, 9.233f, ENoteState::Upcoming});
	Chart.Add({1, 9.683f, ENoteState::Upcoming});
	Chart.Add({0, 10.925f, ENoteState::Upcoming});
	Chart.Add({1, 11.383f, ENoteState::Upcoming});
	Chart.Add({3, 11.800f, ENoteState::Upcoming});
	Chart.Add({0, 15.122f, ENoteState::Upcoming});
	Chart.Add({3, 15.122f, ENoteState::Upcoming});
	Chart.Add({2, 15.526f, ENoteState::Upcoming});
	Chart.Add({0, 16.018f, ENoteState::Upcoming});
	Chart.Add({3, 16.018f, ENoteState::Upcoming});
	Chart.Add({1, 16.460f, ENoteState::Upcoming});
	Chart.Add({0, 16.943f, ENoteState::Upcoming});
	Chart.Add({3, 17.318f, ENoteState::Upcoming});
	//Chart.Add({3, 17.785f, ENoteState::Upcoming});
	Chart.Add({2, 17.977f, ENoteState::Upcoming});
	Chart.Add({1, 18.302f, ENoteState::Upcoming});
	Chart.Add({3, 18.744f, ENoteState::Upcoming});
	Chart.Add({0, 19.161f, ENoteState::Upcoming});
	Chart.Add({0, 20.537f, ENoteState::Upcoming});
	Chart.Add({1, 20.537f, ENoteState::Upcoming});
	Chart.Add({2, 21.262f, ENoteState::Upcoming});
	Chart.Add({3, 21.262f, ENoteState::Upcoming});
	//Chart.Add({3, 21.763f, ENoteState::Upcoming});
	Chart.Add({0, 22.288f, ENoteState::Upcoming});
	Chart.Add({2, 22.797f, ENoteState::Upcoming});
	Chart.Add({1, 23.314f, ENoteState::Upcoming});
	Chart.Add({0, 23.815f, ENoteState::Upcoming});
	Chart.Add({2, 24.290f, ENoteState::Upcoming});
	Chart.Add({1, 24.832f, ENoteState::Upcoming});
	Chart.Add({0, 25.132f, ENoteState::Upcoming});
	Chart.Add({3, 25.299f, ENoteState::Upcoming});
	Chart.Add({2, 25.493f, ENoteState::Upcoming});
	Chart.Add({0, 25.726f, ENoteState::Upcoming});
	//Chart.Add({0, 26.126f, ENoteState::Upcoming});
	Chart.Add({2, 26.593f, ENoteState::Upcoming});
	Chart.Add({3, 27.076f, ENoteState::Upcoming});
	Chart.Add({0, 27.585f, ENoteState::Upcoming});
	Chart.Add({1, 28.035f, ENoteState::Upcoming});
	Chart.Add({2, 28.501f, ENoteState::Upcoming});
	Chart.Add({3, 28.935f, ENoteState::Upcoming});
	Chart.Add({0, 29.376f, ENoteState::Upcoming});
	
	
	// ClassifyingNoteColors
	for (int32 i = 1; i < Chart.Num(); ++i)
	{
		float Gap = Chart[i].HitTime - Chart[i - 1].HitTime;
    
		if (FMath::IsNearlyEqual(Chart[i].HitTime, Chart[i - 1].HitTime))
		{
			Chart[i].NoteBeat = Chart[i - 1].NoteBeat;
			continue;
		}
    
		// existing classification
		if (Gap < 0.2f)      Chart[i].NoteBeat = ENoteBeatType::Burst;
		else if (Gap < 0.4f) Chart[i].NoteBeat = ENoteBeatType::Fast;
		else if (Gap < 0.7f) Chart[i].NoteBeat = ENoteBeatType::Medium;
		else    Chart[i].NoteBeat = ENoteBeatType::Slow;
		
	}
	
	// Receptacle positions
	for (int i = 0; i < 4; ++i)
	{
		FName Tag = FName(*FString::Printf(TEXT("Receptor_%d"), i));
		TArray<AActor*> Found;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), Tag, Found);
		if (Found.Num() > 0)
		{
			if (AReceptacleActor* FoundReceptacle = Cast<AReceptacleActor>(Found[0]))
			{
				Receptacles.Add(FoundReceptacle);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("ChartPlayer.cpp: Unable to Cast Receptacle %d"), i);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ChartPlayer.cpp: Unable to find Receptacle %d"), i);
		}
		
	}
	
	
}

// Called every frame
void AChartPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!NoteActorClass)
	{
		UE_LOG(LogTemp, Error, TEXT("ChartPlayer: NoteActorClass is not set"));
		return;
	}
	while (NextNoteIndex < Chart.Num() && 
		Chart[NextNoteIndex].HitTime <= Conductor->GetSongTime() + SpawnLeadTime)
	{
		
		// Note Spawning
		ANoteActor* NewNote = GetWorld()->SpawnActor<ANoteActor>(NoteActorClass); // Spawning the Blueprint version
		FNoteData* NewNoteData = &Chart[NextNoteIndex];
		NewNote->Init(
			Conductor, 
			NewNoteData, 
			Receptacles[NewNoteData->Lane]->GetActorLocation(), 
			ScrollSpeed);
		NewNoteData->NoteState = ENoteState::Active;
		
		++NextNoteIndex;
	}
	
	while (OldestActiveIndex < NextNoteIndex)
	{
		FNoteData& OldNote = Chart[OldestActiveIndex];
		if (OldNote.NoteState == ENoteState::Hit)
		{
			++OldestActiveIndex;
			continue;
		}
		if (Conductor->GetSongTime() > OldNote.HitTime + HitWindow)
		{
			OldNote.NoteState = ENoteState::Missed;
			GameMode->HUDWidget->UpdateJudgementText(EHitType::Miss);
			GameMode->Judge->ResetCombo();
			GameMode->HUDWidget->UpdateComboText(GameMode->Judge->Combo);
			++OldestActiveIndex;
			continue;
		}
		break;
	}
}

