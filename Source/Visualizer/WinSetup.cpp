// Fill out your copyright notice in the Description page of Project Settings.


#include "WinSetup.h"
#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"
#include "Runtime/Engine/Classes/Engine/TextRenderActor.h"
#include "Components/TextRenderComponent.h"
#include "Runtime/Engine/Classes/Engine/RectLight.h"
#include "Runtime/Engine/Classes/Components/RectLightComponent.h"
#include "Runtime/Engine/Classes/Components/SceneComponent.h"

// Sets default values for this component's properties
UWinSetup::UWinSetup()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWinSetup::BeginPlay()
{
	Super::BeginPlay();
	int RandInt = FMath::RandRange(300, 1500);
	RandDistance = RandInt;
	SetWinText();
	SetWinLight();
	// ...
}


// Called every frame
void UWinSetup::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWinSetup::SetWinText()
{
	FString WinDistanceString = "X = " + (FString::SanitizeFloat(RandDistance / 100)) + " meters";
	WinDistanceText = GetWorld()->SpawnActor<ATextRenderActor>(ATextRenderActor::StaticClass(), FVector(RandDistance, 930.0f, -10.0f), FRotator(0.0f, 90.0f, 0.0f));
	WinDistanceText->GetTextRender()->SetText(FString(WinDistanceString));
	WinDistanceText->GetTextRender()->SetTextRenderColor(FColor::Red);
	WinDistanceText->SetActorScale3D(FVector(0.0f, 1.5f, 1.5f));
}

void UWinSetup::SetWinLight()
{
	WinLightLocation = FVector(RandDistance, 865, 40);
	WinLightRotation = FRotator(-90, 270, -180);
	WinLight = GetWorld()->SpawnActor<ARectLight>(WinLightLocation, WinLightRotation);
	WinLight->SetLightColor(FLinearColor::Blue);
	//***Cast Component onto Actor***
	URectLightComponent * GetWinLight = Cast<URectLightComponent>(WinLight->GetComponentByClass(URectLightComponent::StaticClass()));
	GetWinLight->SetMobility(EComponentMobility::Movable);
	GetWinLight->SetSourceWidth(115);
	GetWinLight->SetSourceHeight(115);
}

