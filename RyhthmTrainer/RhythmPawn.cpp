// Fill out your copyright notice in the Description page of Project Settings.


#include "RhythmPawn.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Judge.h"
#include "ReceptacleActor.h"
#include "RhythmGameMode.h"
#include "HUDWidgetClass.h"
#include "SongConductor.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
ARhythmPawn::ARhythmPawn()
{
 
	PrimaryActorTick.bCanEverTick = true;
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(RootComponent);
	
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void ARhythmPawn::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = 
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(ArrowKeyMappingContext, 0);
		}
	}
	
	GameMode = Cast<ARhythmGameMode>(GetWorld()->GetAuthGameMode());
}

void ARhythmPawn::OnLeftPress(const FInputActionValue& Value)
{
	HandleLanePress(0);
}

void ARhythmPawn::OnRightPress(const FInputActionValue& Value)
{
	HandleLanePress(3);
}

void ARhythmPawn::OnUpPress(const FInputActionValue& Value)
{
	HandleLanePress(2);
}

void ARhythmPawn::OnDownPress(const FInputActionValue& Value)
{
	HandleLanePress(1);
}

void ARhythmPawn::HandleLanePress(const int32 Lane)
{
	if (GameMode->bRecordingMode)
	{
		GameMode->SongConductor->RecordPress(Lane);
		return;
	}
	
	float PressTime = GameMode->SongConductor->GetSongTime();
	EHitType Result = GameMode->Judge->JudgePress(Lane, PressTime);
	
	if (Result == EHitType::Perfect && PerfectBurstEffect)
	{
		FRotator Rotation = FRotator(0, 180, 0);
		FVector Scale = FVector(10.f);
		FVector SpawnLocation = GameMode->ChartPlayer->Receptacles[Lane]->GetActorLocation();
		SpawnLocation.X -= 25;
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(), 
			PerfectBurstEffect, 
			SpawnLocation,
			Rotation,
			Scale,
			true,
			true);
	}
	
	GameMode->HUDWidget->UpdateScore(GameMode->Judge->Score);
	GameMode->HUDWidget->UpdateJudgementText(Result);
	GameMode->HUDWidget->UpdateComboText(GameMode->Judge->Combo);
	if (ComboShakeClass)
	{
		if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
		{
			if (const int32 Combo = GameMode->Judge->Combo; Combo >= 5 && Combo % 5 == 0)
			{
				PC->ClientStartCameraShake(ComboShakeClass, 1.0f);
			}
		}
	}
	
	GameMode->ChartPlayer->Receptacles[Lane]->HandleOnClick();
	
	
}


// Called every frame
void ARhythmPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Debug Display time
	// GEngine->AddOnScreenDebugMessage(
	// 0, 
	// 0.f, 
	// FColor::Yellow, 
	// FString::Printf(TEXT("SongTime: %.2f"), GameMode->SongConductor->GetSongTime()));

}

// Called to bind functionality to input
void ARhythmPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(LeftAction, ETriggerEvent::Triggered, this
			, &ARhythmPawn::OnLeftPress);
		EnhancedInputComponent->BindAction(RightAction, ETriggerEvent::Triggered, this, &ARhythmPawn::OnRightPress);
		EnhancedInputComponent->BindAction(UpAction, ETriggerEvent::Triggered, this, &ARhythmPawn::OnUpPress);
		EnhancedInputComponent->BindAction(DownAction, ETriggerEvent::Triggered, this, &ARhythmPawn::OnDownPress);
		
	}

}

