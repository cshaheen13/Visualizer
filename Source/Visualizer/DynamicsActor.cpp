// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicsActor.h"
#include "Engine/World.h"
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"
#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"
#include "Runtime/Engine/Classes/Engine/TextRenderActor.h"
#include "Components/TextRenderComponent.h"
#include "Runtime/Engine/Classes/Engine/RectLight.h"
#include "Runtime/Engine/Classes/Components/RectLightComponent.h"
#include "Runtime/Engine/Classes/Components/SceneComponent.h"
#include "Runtime/Engine/Classes/Engine/EngineTypes.h"


// Sets default values
ADynamicsActor::ADynamicsActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADynamicsActor::BeginPlay()
{
	Super::BeginPlay();
	ADynamicsActor::Setup();
	UE_LOG(LogClass, Log, TEXT("Rand = %f"), RandDistance);
	ADynamicsActor::SetWinText();
	ADynamicsActor::SetWinLight();
}

float ADynamicsActor::GetAccelX()
{
	return AccelX;
}

void ADynamicsActor::SetAccelX(float KineticFriction, float Gravity)
{
	AccelX = KineticFriction * Gravity;
}

FVector ADynamicsActor::GetWinDistance()
{
	return WinDistance;
}

void ADynamicsActor::SetWinDistance(float x, float y, float z)
{
	WinDistance = FVector(x, y, z);
}

// Called every frame
void ADynamicsActor::Tick(float DeltaTime)
{
	MoveActor(DeltaTime);
}

void ADynamicsActor::Setup()
{
	SetActorTickEnabled(false);
	SetAccelX(KineticFriction, Gravity);
	
	//Needed to set precision for floats so win distance is two decimal places or less
	int RandInt = FMath::RandRange(300, 1500);
	RandDistance = RandInt;

	ADynamicsActor::SetWinDistance(RandDistance, 0.0f, 0.0f);
}

void ADynamicsActor::SetWinText()
{
	FString WinDistanceString = "X = " + (FString::SanitizeFloat(RandDistance / 100)) + " meters";
	WinDistanceText = GetWorld()->SpawnActor<ATextRenderActor>(ATextRenderActor::StaticClass(), FVector(RandDistance, 930.0f, -10.0f), FRotator(0.0f, 90.0f, 0.0f));
	WinDistanceText->GetTextRender()->SetText(FString(WinDistanceString));
	WinDistanceText->GetTextRender()->SetTextRenderColor(FColor::Red);
	WinDistanceText->SetActorScale3D(FVector(0.0f, 1.5f, 1.5f));
}

void ADynamicsActor::SetWinLight()
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

void ADynamicsActor::MoveActor(float DeltaTime)
{
	FVector NewLocation = GetActorLocation();
	FVector OldLocation = GetActorLocation();
	float deltaX = 100 * ((InitialVelocity * RunningTime) + ((ADynamicsActor::GetAccelX())*(RunningTime)*(RunningTime)*.5));

	if (((deltaX - NewLocation.X) >= 0))
	{
		NewLocation.X = deltaX;
		RunningTime += DeltaTime;
		SetActorLocation(NewLocation);
	}

	if ((RunningTime >= 0.1) && ((NewLocation.X - OldLocation.X) <= 0.02))
	{
		ADynamicsActor::CheckWinLose();
	}
}

void ADynamicsActor::CheckWinLose()
{
	FVector NewLocation = GetActorLocation();
	FVector WinLoseDist = GetWinDistance();
	//TODO Figure out rounding for floats
	UE_LOG(LogClass, Warning, TEXT("X = %f"), WinLoseDist.X - NewLocation.X);
	if (((WinLoseDist.X - NewLocation.X) < 5.0f) && ((WinLoseDist.X - NewLocation.X) > -5.0f))
	{
		UE_LOG(LogClass, Warning, TEXT("You Win! The block travelled %f meters."), NewLocation.X/100);
	}
	else
	{
		UE_LOG(LogClass, Warning, TEXT("You Lose! The block travelled %f meters."), NewLocation.X/100);
	}
	GameOver = true;
	SetActorTickEnabled(false);
}