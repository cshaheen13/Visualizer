// Fill out your copyright notice in the Description page of Project Settings.


#include "WinLose.h"
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"
#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"
#include "Runtime/Engine/Classes/Engine/TextRenderActor.h"
#include "Components/TextRenderComponent.h"
#include "Runtime/Engine/Classes/Engine/RectLight.h"
#include "Runtime/Engine/Classes/Components/RectLightComponent.h"


// Sets default values for this component's properties
UWinLose::UWinLose()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UWinLose::SetWinDistance(float x, float y, float z)
{
	WinDistance = FVector(x, y, z);
}

FVector UWinLose::GetWinDistance()
{
	return WinDistance;
}

// Called when the game starts
void UWinLose::BeginPlay()
{
	Super::BeginPlay();

	float RandDistance = FMath::RandRange(300, 1500);
	UWinLose::SetWinDistance(RandDistance, 0.0f, 0.0f);
	UE_LOG(LogClass, Warning, TEXT("Rand = %f"), RandDistance);
	FString WinDistanceString = "X = " + (FString::SanitizeFloat(RandDistance)) + " meters";
	WinLightDistance = FVector(RandDistance, 870, 40);
	WinLightRotation = FRotator(-90, 270, -180);

	//WinDistanceText = NewObject<UTextRenderComponent>(this);
	WinDistanceText = GetWorld()->SpawnActor<ATextRenderActor>(ATextRenderActor::StaticClass(), FVector(RandDistance, 930.0f, -10.0f), FRotator(0.0f, 90.0f, 0.0f));
	WinDistanceText->GetTextRender()->SetText(FString(WinDistanceString));
	WinDistanceText->GetTextRender()->SetTextRenderColor(FColor::Red);
	WinDistanceText->SetActorScale3D(FVector(0.0f, 1.5f, 1.5f));

	WinLight = GetWorld()->SpawnActor<ARectLight>(WinLightDistance, WinLightRotation);
	WinLight->SetLightColor(FLinearColor::Blue);
	//***Cast Component onto Actor***
	URectLightComponent * GetWinLight = Cast<URectLightComponent>(WinLight->GetComponentByClass(URectLightComponent::StaticClass()));
	GetWinLight->SetSourceWidth(115);
	GetWinLight->SetSourceHeight(115);

	AActor* Owner = GetOwner();
}


// Called every frame
void UWinLose::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	//Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//FVector NewLocation = ActorThatWins->GetActorLocation();

	//if ((UWinLose::GetWinDistance().X - NewLocation.X) < 0.1f)
	//{
	//	UE_LOG(LogClass, Warning, TEXT("You Win"));
	//}
}

