// Fill out your copyright notice in the Description page of Project Settings.


#include "SongConductor.h"

#include "ChartPlayer.h"
#include "Components/AudioComponent.h"

// Sets default values
ASongConductor::ASongConductor()
{
	PrimaryActorTick.bCanEverTick = false;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	RootComponent = AudioComponent;
	
	AudioComponent->bAutoActivate = false;
	
	AudioComponent->OnAudioFinished.AddDynamic(this, &ASongConductor::OnSongFinished);
}

// Called when the game starts or when spawned
void ASongConductor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASongConductor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASongConductor::StartSong()
{
	if (!Song || !AudioComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("SongConductor: Song or AudioComponent is null"));
		return;
	}
	
	AudioComponent->SetSound(Song);
	AudioComponent->Play();
	SongStartWorldTime = GetWorld()->GetAudioTimeSeconds();
	bStarted = true;
	
	UE_LOG(LogTemp, Display, TEXT("SongConductor: StartSong at world time %f"), SongStartWorldTime);
}

float ASongConductor::GetSongTime() const
{
	if (!bStarted) return 0.f;
	
	
	return static_cast<float>(GetWorld()->GetAudioTimeSeconds() - SongStartWorldTime);
}

bool ASongConductor::IsPlaying() const
{
	return AudioComponent && AudioComponent->IsPlaying();
}

// TODO Add recording mode
void ASongConductor::RecordPress(int32 Lane)
{
	RecordedChart.Add({Lane, GetSongTime(), ENoteState::Upcoming});
}

void ASongConductor::OnSongFinished()
{
	for (const FNoteData& Note : RecordedChart)
	{
		UE_LOG(LogTemp, Display, TEXT("Chart.Add({%d, %.3ff, ENoteState::Upcoming});"), Note.Lane, Note.HitTime);
	}
}

