// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ReceptacleActor.generated.h"

UCLASS()
class RYHTHMTRAINER_API AReceptacleActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AReceptacleActor();
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComponent;
	
	UPROPERTY(EditAnywhere, Category=Effects)
	TObjectPtr<UMaterialInterface> OriginalMaterial;
	
	UPROPERTY(EditAnywhere, Category=Effects)
	TObjectPtr<UMaterialInterface> ActiveMaterial;
	
	void HandleOnClick();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	FTimerHandle ClickEffectTimerHandle;
	
	FVector OriginalScale;
	FVector GetNewScale(const float ScaleFactor) const;
};
