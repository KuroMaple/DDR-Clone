// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChartPlayer.h"
#include "GameFramework/Actor.h"
#include "NoteActor.generated.h"

class ASongConductor;

UCLASS()
class RYHTHMTRAINER_API ANoteActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANoteActor();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void Init(
		ASongConductor* InSongConductor, 
		FNoteData* InNoteData, 
		const FVector& InTargetReceptaclePos, 
		const float InScrollSpeed);
	
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicMat;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private:
	
	FNoteData* NoteData;
	
	FVector TargetReceptaclePosition;
	float ScrollSpeed;
	
	UPROPERTY()
	FVector ReceptorPosition;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> ArrowMesh;
	
	UPROPERTY()
	TObjectPtr<ASongConductor> SongConductor;

	static FLinearColor GetColorForBeat(ENoteBeatType Beat);
};
