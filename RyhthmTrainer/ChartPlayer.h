// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChartPlayer.generated.h"

class ARhythmGameMode;
class AReceptacleActor;
class ANoteActor;
class ASongConductor;

UENUM()
enum class ENoteBeatType : uint8
{
	Slow,
	Medium,
	Fast,
	Burst,
};

UENUM()
enum class ENoteState : uint8
{
	Upcoming,
	Active,
	Hit,
	Missed
};

// May move to whole class later
USTRUCT(BlueprintType)
struct FNoteData
{
	GENERATED_BODY()
	
	UPROPERTY()
	int32 Lane;
	
	UPROPERTY()
	float HitTime;
	
	UPROPERTY()
	ENoteState NoteState = ENoteState::Upcoming;
	
	UPROPERTY()
	ENoteBeatType NoteBeat = ENoteBeatType::Slow;
};

UCLASS()
class RYHTHMTRAINER_API AChartPlayer : public AActor
{
	GENERATED_BODY()
	
public:	
	AChartPlayer();
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY()
	TObjectPtr<ASongConductor> Conductor;
	
	UPROPERTY()
	TArray<FNoteData> Chart;
	
	UPROPERTY()
	TArray<AReceptacleActor*> Receptacles;
	
	UPROPERTY()
	TObjectPtr<ARhythmGameMode> GameMode; // For passive misses
	
	// Also set in judge, lower here to make visual difference
	UPROPERTY(EditAnywhere)
	float HitWindow = 0.2;
protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY()
	int32 NextNoteIndex;
	
	int32 OldestActiveIndex;
	
	
	UPROPERTY(EditAnywhere)
	float SpawnLeadTime = 2.0f;
	
	UPROPERTY(EditAnywhere, Category=Notes)
	TSubclassOf<ANoteActor> NoteActorClass;
	
	float ScrollSpeed = 500.f;
};
