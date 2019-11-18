// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicsActor.h"
#include "WinSetup.h"
#include "Engine/World.h"
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"
#include "Runtime/Engine/Classes/Engine/EngineTypes.h"
#include "Runtime/Engine/Classes/Engine/TextRenderActor.h"
#include "Components/TextRenderComponent.h"


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
	MoveKineticFrictionActor(DeltaTime);
}

void ADynamicsActor::Setup()
{
	SetActorTickEnabled(false);
	SetAccelX(KineticFriction, Gravity);
	
	///TODO set precision for floats so win distance is two decimal places or less
	//***Cast Component onto Actor***
	UWinSetup* GetWinSetup = Cast<UWinSetup>(ActorThatWins->GetComponentByClass(UWinSetup::StaticClass()));
	if (GetWinSetup)
	{
		SetupDistance = GetWinSetup->RandDistance;
		SetWinDistance(SetupDistance, 0.f, 0.f);
		UE_LOG(LogClass, Warning, TEXT("Win Distance %f meters."), SetupDistance);
	}
	else
	{
		SpawnCoordinateSystemText();
		UE_LOG(LogClass, Warning, TEXT("No Win Setup"));
	}

}

void ADynamicsActor::SpawnCoordinateSystemText()
{
	FString CoordinateSystemDash = "-";
	CoordinateSystemXDash = GetWorld()->SpawnActor<ATextRenderActor>(ATextRenderActor::StaticClass(), FVector(1150.0f, 1830.0f, 130.0f), FRotator(0.0f, 90.0f, 0.0f));
	CoordinateSystemXDash->GetTextRender()->SetText(FString(CoordinateSystemDash));
	CoordinateSystemXDash->GetTextRender()->SetTextRenderColor(FColor::Red);
	CoordinateSystemXDash->GetTextRender()->SetHorizontalAlignment(EHTA_Center);
	CoordinateSystemXDash->GetTextRender()->SetVerticalAlignment(EVRTA_TextCenter);
	CoordinateSystemXDash->SetActorScale3D(FVector(1, 5, 1));

	CoordinateSystemZDash = GetWorld()->SpawnActor<ATextRenderActor>(ATextRenderActor::StaticClass(), FVector(1135.0f, 1830.0f, 145.0f), FRotator(0.0f, 90.0f, 90.0f));
	CoordinateSystemZDash->GetTextRender()->SetText(FString(CoordinateSystemDash));
	CoordinateSystemZDash->GetTextRender()->SetTextRenderColor(FColor::Blue);
	CoordinateSystemZDash->GetTextRender()->SetHorizontalAlignment(EHTA_Center);
	CoordinateSystemZDash->GetTextRender()->SetVerticalAlignment(EVRTA_TextCenter);
	CoordinateSystemZDash->SetActorScale3D(FVector(1, 5, 1));

	FString CoordinateSystemX = "X";
	CoordinateSystemXText = GetWorld()->SpawnActor<ATextRenderActor>(ATextRenderActor::StaticClass(), FVector(1172.0f, 1830.0f, 130.0f), FRotator(0.0f, 90.0f, 0.0f));
	CoordinateSystemXText->GetTextRender()->SetText(FString(CoordinateSystemX));
	CoordinateSystemXText->GetTextRender()->SetTextRenderColor(FColor::Red); 
	CoordinateSystemXText->GetTextRender()->SetHorizontalAlignment(EHTA_Center);
	CoordinateSystemXText->GetTextRender()->SetVerticalAlignment(EVRTA_TextCenter);
	CoordinateSystemXText->SetActorScale3D(FVector(1, .75, .75));

	FString CoordinateSystemZ = "Y";
	CoordinateSystemZText = GetWorld()->SpawnActor<ATextRenderActor>(ATextRenderActor::StaticClass(), FVector(1135.0f, 1830.0f, 170.0f), FRotator(0.0f, 90.0f, 0.0f));
	CoordinateSystemZText->GetTextRender()->SetText(FString(CoordinateSystemZ));
	CoordinateSystemZText->GetTextRender()->SetTextRenderColor(FColor::Blue);
	CoordinateSystemZText->GetTextRender()->SetHorizontalAlignment(EHTA_Center);
	CoordinateSystemZText->GetTextRender()->SetVerticalAlignment(EVRTA_TextCenter);
	CoordinateSystemZText->SetActorScale3D(FVector(1, .75, .75));
}

void ADynamicsActor::MoveKineticFrictionActor(float DeltaTime)
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
	FVector WinLoseDist = ADynamicsActor::GetWinDistance();
	//TODO Figure out rounding for floats
	UE_LOG(LogClass, Warning, TEXT("X = %f"), WinLoseDist.X - NewLocation.X);
	if (((WinLoseDist.X - NewLocation.X) < 5.0f) && ((WinLoseDist.X - NewLocation.X) > -5.0f))
	{
		UE_LOG(LogClass, Warning, TEXT("You Win! The block travelled %f meters."), NewLocation.X/100);
		DoYouWin = true;
	}
	else
	{
		UE_LOG(LogClass, Warning, TEXT("You Lose! The block travelled %f meters."), NewLocation.X/100);
	}
	IsGameOver = true;
	SetActorTickEnabled(false);
}