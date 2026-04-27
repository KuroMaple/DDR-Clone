// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidgetClass.generated.h"

enum class EHitType : uint8;
class UTextBlock;
/**
 * 
 */
UCLASS()
class RYHTHMTRAINER_API UHUDWidgetClass : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentScore;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> JudgementText;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> ComboText;
	
	UPROPERTY()
	TObjectPtr<UTextBlock> ComboWords;
	
	void UpdateScore(int32 NewScore) const;
	void UpdateJudgementText(EHitType HitType);
	void ClearJudgementText() const;
	void UpdateComboText(int32 Combo);
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override; 
private:
	FTimerHandle JudgementTextTimerHandle;
	void PopTextAnimation();
	
	float PopTimer = 0.f;
	float PopDuration = 0.15f;
	
};
