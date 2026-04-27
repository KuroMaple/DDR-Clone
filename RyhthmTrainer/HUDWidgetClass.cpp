// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDWidgetClass.h"
#include "Fonts/SlateFontInfo.h"
#include "Judge.h"
#include "Components/TextBlock.h"

void UHUDWidgetClass::UpdateScore(const int32 NewScore) const
{
	CurrentScore->SetText(FText::AsNumber(NewScore));
}

void UHUDWidgetClass::UpdateJudgementText(EHitType HitType) 
{
	FSlateFontInfo FontInfo = JudgementText->GetFont();
	switch (HitType)
	{
	case EHitType::Perfect:
		JudgementText->SetText(FText::FromString("Perfect"));
		JudgementText->SetColorAndOpacity(FSlateColor(FLinearColor::FromSRGBColor(FColor::FromHex("fdf07e"))));
		FontInfo.OutlineSettings.OutlineColor = FLinearColor::FromSRGBColor(FColor::FromHex("665500"));
		break;
	case EHitType::Good:
		JudgementText->SetText(FText::FromString("Good"));
		JudgementText->SetColorAndOpacity(FSlateColor(FLinearColor::FromSRGBColor(FColor::FromHex("87e1fc"))));
		FontInfo.OutlineSettings.OutlineColor = FLinearColor::FromSRGBColor(FColor::FromHex("0a2945"));
		break;
	case EHitType::Miss:
		JudgementText->SetText(FText::FromString("Miss"));
		JudgementText->SetColorAndOpacity(FSlateColor(FLinearColor::FromSRGBColor(FColor::FromHex("f30000"))));
		FontInfo.OutlineSettings.OutlineColor = FLinearColor::FromSRGBColor(FColor::FromHex("560605"));
		break;
	}
	FontInfo.OutlineSettings.OutlineSize = 7;
	JudgementText->SetFont(FontInfo);
	
	
	GetWorld()->GetTimerManager().SetTimer(
		JudgementTextTimerHandle,
		[this]
		{
			ClearJudgementText();
		},
		0.5f,
		false);
	
	PopTextAnimation();
}

void UHUDWidgetClass::ClearJudgementText() const
{
	JudgementText->SetText(FText::GetEmpty());
}

void UHUDWidgetClass::UpdateComboText(int32 Combo)
{
	if (Combo >= 2)
	{
		FString ComboString = FString::Printf(TEXT("%d"), Combo);
		ComboText->SetText(FText::FromString(ComboString));
		ComboWords->SetText(FText::FromString("COMBO"));
	}
	else
	{
		ComboText->SetText(FText::GetEmpty());
		ComboWords->SetText(FText::GetEmpty());
	}
	PopTextAnimation();
}

void UHUDWidgetClass::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (PopTimer <= 0.f) return;
	if (PopTimer > 0.f)
	{
		PopTimer -= InDeltaTime;
		float Alpha = FMath::Clamp(PopTimer / PopDuration, 0.f, 1.f);
		float Scale = FMath::Lerp(1.0f, 1.2f, Alpha);
		JudgementText->SetRenderScale(FVector2D(Scale));
		ComboText->SetRenderScale(FVector2D(Scale));
		ComboWords->SetRenderScale(FVector2D(Scale));
	}
}

void UHUDWidgetClass::PopTextAnimation()
{
	PopTimer = PopDuration;
}


