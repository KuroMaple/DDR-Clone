// Fill out your copyright notice in the Description page of Project Settings.


#include "ReceptacleActor.h"

// Sets default values
AReceptacleActor::AReceptacleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
}

void AReceptacleActor::HandleOnClick()
{
	MeshComponent->SetMaterial(0, ActiveMaterial);
	this->SetActorScale3D(GetNewScale(1.1));
	
	GetWorldTimerManager().SetTimer(ClickEffectTimerHandle,
		[this]
		{
			MeshComponent->SetMaterial(0, OriginalMaterial);
			this->SetActorScale3D(OriginalScale);
		}, 0.15f, false);
}

// Called when the game starts or when spawned
void AReceptacleActor::BeginPlay()
{
	Super::BeginPlay();
	if (OriginalMaterial)
	{
		MeshComponent->SetMaterial(0, OriginalMaterial);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ReceptacleActor.cpp: Original material not set"));
	}
	
	// Set Size of Mesh
	OriginalScale = GetActorScale3D();
	
}

FVector AReceptacleActor::GetNewScale(const float ScaleFactor) const
{
	return FVector
	(
		OriginalScale.X * ScaleFactor,
		OriginalScale.Y * ScaleFactor,
		OriginalScale.Z * ScaleFactor
	);
}

// Called every frame
void AReceptacleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

