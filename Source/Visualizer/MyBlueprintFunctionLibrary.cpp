// Fill out your copyright notice in the Description page of Project Settings.

#include "MyBlueprintFunctionLibrary.h"
#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"
#include "Engine/TextRenderActor.h"
#include "Components/TextRenderComponent.h"


FString UMyBlueprintFunctionLibrary::GetFloatAsStringWithPrecision(float TheFloat, int32 Precision, bool IncludeLeadingZero)
{
	//Round to integral if have something like 1.9999 within precision
	float Rounded = roundf(TheFloat);
	if (FMath::Abs(TheFloat - Rounded) < FMath::Pow(10, -1 * Precision))
	{
		TheFloat = Rounded;
	}
	FNumberFormattingOptions NumberFormat;					//Text.h
	NumberFormat.MinimumIntegralDigits = (IncludeLeadingZero) ? 1 : 0;
	NumberFormat.MaximumIntegralDigits = 10000;
	NumberFormat.MinimumFractionalDigits = Precision;
	NumberFormat.MaximumFractionalDigits = Precision;
	return FText::AsNumber(TheFloat, &NumberFormat).ToString();
}

//Float as FText With Precision!
FText UMyBlueprintFunctionLibrary::GetFloatAsTextWithPrecision(float TheFloat, int32 Precision, bool IncludeLeadingZero)
{
	//Round to integral if have something like 1.9999 within precision
	float Rounded = roundf(TheFloat);
	if (FMath::Abs(TheFloat - Rounded) < FMath::Pow(10, -1 * Precision))
	{
		TheFloat = Rounded;
	}
	FNumberFormattingOptions NumberFormat;					//Text.h
	NumberFormat.MinimumIntegralDigits = (IncludeLeadingZero) ? 1 : 0;
	NumberFormat.MaximumIntegralDigits = 10000;
	NumberFormat.MinimumFractionalDigits = Precision;
	NumberFormat.MaximumFractionalDigits = Precision;
	return FText::AsNumber(TheFloat, &NumberFormat);
}
