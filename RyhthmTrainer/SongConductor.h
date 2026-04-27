// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChartPlayer.h"
#include "GameFramework/Actor.h"
#include "SongConductor.generated.h"


UCLASS()
class RYHTHMTRAINER_API ASongConductor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASongConductor();
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Song)
	TObjectPtr<USoundBase> Song;
	
	UPROPERTY(VisibleAnywhere, Category=Song)
	TObjectPtr<UAudioComponent> AudioComponent;
	
	UFUNCTION(BlueprintCallable, Category=Song)
	void StartSong();
	
	UFUNCTION(BlueprintCallable, Category=Song)
	float GetSongTime() const;
	
	UFUNCTION(BlueprintCallable, Category=Song)
	bool IsPlaying() const;
	
	TArray<FNoteData> RecordedChart;
	
	void RecordPress(int32 Lane);
	
	UFUNCTION()
	void OnSongFinished();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	double SongStartWorldTime = -1.0;
	bool bStarted = false;
	
};
