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
	SetActorTickEnabled(false);
	SetAccelX(KineticFriction, Gravity);

	float RandDistance = FMath::RandRange(300, 1500);
	ADynamicsActor::SetWinDistance(RandDistance, 0.0f, 0.0f);
	UE_LOG(LogClass, Warning, TEXT("Rand = %f"), RandDistance);
	FString WinDistanceString = "X = " + (FString::SanitizeFloat(RandDistance/100)) + " meters";
	WinLightLocation = FVector(RandDistance, 865, 40);
	WinLightRotation = FRotator(-90, 270, -180);

	WinDistanceText = GetWorld()->SpawnActor<ATextRenderActor>(ATextRenderActor::StaticClass(), FVector(RandDistance, 930.0f, -10.0f), FRotator(0.0f, 90.0f, 0.0f));
	WinDistanceText->GetTextRender()->SetText(FString(WinDistanceString));
	WinDistanceText->GetTextRender()->SetTextRenderColor(FColor::Red);
	WinDistanceText->SetActorScale3D(FVector(0.0f, 1.5f, 1.5f));

	WinLight = GetWorld()->SpawnActor<ARectLight>(WinLightLocation, WinLightRotation);
	WinLight->SetLightColor(FLinearColor::Blue);
	//***Cast Component onto Actor***
	URectLightComponent * GetWinLight = Cast<URectLightComponent>(WinLight->GetComponentByClass(URectLightComponent::StaticClass()));
	GetWinLight->SetMobility(EComponentMobility::Movable);
	GetWinLight->SetSourceWidth(115);
	GetWinLight->SetSourceHeight(115);
}

void ADynamicsActor::SetWinDistance(float x, float y, float z)
{
	WinDistance = FVector(x, y, z);
}

FVector ADynamicsActor::GetWinDistance()
{
	return WinDistance;
}

float ADynamicsActor::GetAccelX()
{
	return AccelX;
}

void ADynamicsActor::SetAccelX(float KineticFriction, float Gravity)
{
	AccelX = KineticFriction * Gravity;
}

void ADynamicsActor::MoveActor(float DeltaTime)
{
	FVector NewLocation = GetActorLocation();
	FVector OldLocation = GetActorLocation();
	float deltaX = 100 * ((InitialVelocity * RunningTime) + ((ADynamicsActor::GetAccelX())*(RunningTime)*(RunningTime)*.5));
	//UE_LOG(LogClass, Warning, TEXT("t = %f"), RunningTime);
	//UE_LOG(LogClass, Warning, TEXT("X = %f"), deltaX);

	if (((deltaX - NewLocation.X) >= 0))
	{
		NewLocation.X = deltaX;
		RunningTime += DeltaTime;
		SetActorLocation(NewLocation);
		UE_LOG(LogClass, Warning, TEXT("%f"), NewLocation.X - OldLocation.X);
	}
	if ((RunningTime != 0) && ((NewLocation.X - OldLocation.X) <= 0.02))
	{
		FVector WinLoseDist = GetWinDistance();
		if (((WinLoseDist.X - NewLocation.X) < 0.1f) && ((WinLoseDist.X - NewLocation.X) > -0.1f))
		{
			YouWinBool = true;
			UE_LOG(LogClass, Warning, TEXT("You Win! %f"), WinLoseDist.X - NewLocation.X);
		}
	}
}

// Called every frame
void ADynamicsActor::Tick(float DeltaTime)
{
	MoveActor(DeltaTime);
}

