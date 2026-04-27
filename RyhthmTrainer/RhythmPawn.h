// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/CameraShakeBase.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RhythmPawn.generated.h"

class ARhythmGameMode;
struct FInputActionValue;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class UNiagaraSystem;

UCLASS()
class RYHTHMTRAINER_API ARhythmPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ARhythmPawn();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Root;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	TObjectPtr<UInputMappingContext> ArrowKeyMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	TObjectPtr<UInputAction> LeftAction; 
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	TObjectPtr<UInputAction> RightAction; 
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	TObjectPtr<UInputAction> UpAction; 
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
	TObjectPtr<UInputAction> DownAction; 
	
	UPROPERTY()
	TObjectPtr<ARhythmGameMode> GameMode;
	
	UPROPERTY(EditAnywhere, Category=Effects)
	TObjectPtr<UNiagaraSystem> PerfectBurstEffect;
	
	UPROPERTY(EditAnywhere, Category=Effects)
	TSubclassOf<UCameraShakeBase> ComboShakeClass;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void OnLeftPress(const FInputActionValue& Value);
	void OnRightPress(const FInputActionValue& Value);
	void OnUpPress(const FInputActionValue& Value);
	void OnDownPress(const FInputActionValue& Value);
	
	void HandleLanePress(const int32 Lane);
	
};
