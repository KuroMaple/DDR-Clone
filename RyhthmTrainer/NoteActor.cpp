// Fill out your copyright notice in the Description page of Project Settings.


#include "NoteActor.h"

#include "SongConductor.h"

// Sets default values
ANoteActor::ANoteActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowMesh"));
	RootComponent = ArrowMesh;
	
}

// Called when the game starts or when spawned
void ANoteActor::BeginPlay()
{
	Super::BeginPlay();
	
}

FLinearColor ANoteActor::GetColorForBeat(ENoteBeatType Beat)
{
	switch (Beat)
	{
	case ENoteBeatType::Burst:  
		return FLinearColor::FromSRGBColor(FColor::FromHex("06ff06"));  // green
	case ENoteBeatType::Fast:   
		return FLinearColor::FromSRGBColor(FColor::FromHex("fef617"));  // yellow
	case ENoteBeatType::Medium: 
		return FLinearColor::FromSRGBColor(FColor::FromHex("0c06ff"));  // blue
	case ENoteBeatType::Slow:   
		return FLinearColor::FromSRGBColor(FColor::FromHex("fe1919"));  // red
	default:                    
		return FLinearColor::White;
	}

}

// Called every frame
void ANoteActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (this->NoteData->NoteState == ENoteState::Hit || this->NoteData->NoteState == ENoteState::Missed)
	{
		Destroy();
		return;
	}
	
	const float CurrTime = SongConductor->GetSongTime();
	const float TimeTillHit = NoteData->HitTime - CurrTime;
	
	FVector NewPosition = TargetReceptaclePosition;
	NewPosition.Z -= TimeTillHit * ScrollSpeed;
	NewPosition.X -= 10.f; // Making notes spawn in front of receptacle
	SetActorLocation(NewPosition);
	
}

void ANoteActor::Init(
	ASongConductor* InSongConductor,
	FNoteData* InNoteData, 
	const FVector& InTargetReceptaclePos,
	const float InScrollSpeed)
{
	SongConductor = InSongConductor;
	NoteData = InNoteData;
	TargetReceptaclePosition = InTargetReceptaclePos;
	ScrollSpeed = InScrollSpeed;
	
	DynamicMat = ArrowMesh->CreateDynamicMaterialInstance(0);
	FLinearColor NoteColor = GetColorForBeat(NoteData->NoteBeat);
	FLinearColor OutlineColor = NoteColor * 0.3f;
	DynamicMat->SetVectorParameterValue(TEXT("FillColor"), NoteColor);
	DynamicMat->SetVectorParameterValue(TEXT("OutlineColor"), OutlineColor);
	
	//Adjusting Note Rotation
	switch (NoteData->Lane)
	{
	case 0:
		{
			SetActorRotation(FRotator(90, 90, 0)); 
			break;
		}
	case 1:
		{
			SetActorRotation(FRotator(0, -90, 180)); 
			break;
		}
	case 2:
		{
			SetActorRotation(FRotator(0, 90, 0)); 
			break;
		}
	case 3:
		{
			SetActorRotation(FRotator(-90, 90, 0)); 
			break;
		}
	default:
		UE_LOG(LogTemp, Error, TEXT("NoteActor.cpp: Unable to find Note lane"));
		break;
	}
}

