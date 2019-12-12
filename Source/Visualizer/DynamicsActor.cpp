// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicsActor.h"
#include "WinSetup.h"
#include "Engine/World.h"
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"
#include "Runtime/Engine/Classes/Engine/EngineTypes.h"
#include "Runtime/Engine/Classes/Engine/TextRenderActor.h"
#include "Components/TextRenderComponent.h"
#include "SpawnTextActorComponent.h"


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
	USpawnTextActorComponent* GetSpawnText = Cast<USpawnTextActorComponent>(ActorThatWins->GetComponentByClass(USpawnTextActorComponent::StaticClass()));
	if (GetSpawnText)
	{
		CoordinateSystemXText = GetSpawnText->SpawnText(CoordinateSystemXText, 1172, 1830, 130, 0, 90, 0, CoordinateSystemX, "CoordinateSystemX", FColor::Red, EHTA_Center, EVRTA_TextCenter, 1, 25, 25, false);
		CoordinateSystemXDash = GetSpawnText->SpawnText(CoordinateSystemXDash, 1150, 1830, 130, 0, 90, 0, CoordinateSystemDash, "CoordinateSystemXDash", FColor::Red, EHTA_Center, EVRTA_TextCenter, 1, 100, 50, false);
		CoordinateSystemZText = GetSpawnText->SpawnText(CoordinateSystemZText, 1135, 1830, 170, 0, 90, 0, CoordinateSystemZ, "CoordinateSystemZ", FColor::Blue, EHTA_Center, EVRTA_TextCenter, 1, 25, 25, false);
		CoordinateSystemZDash = GetSpawnText->SpawnText(CoordinateSystemZDash, 1135, 1830, 145, 0, 90, 90, CoordinateSystemDash, "CoordinateSystemZDash", FColor::Blue, EHTA_Center, EVRTA_TextCenter, 1, 100, 50, false);
	}
	else {
		UE_LOG(LogClass, Warning, TEXT("No Spawn Text Component"));
	}
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
	else if (IsGameOver != true) {
		IsGameOver = true;
		SetActorTickEnabled(false);
	}

	UE_LOG(LogClass, Warning, TEXT("Time = %f"), RunningTime);

	if ((RunningTime >= 0.1) && ((NewLocation.X - OldLocation.X) <= 0.02))
	{
		//TODO: Make this an actor component since sandbox doesn't need this
		ADynamicsActor::CheckWinLose();
	}
}

void ADynamicsActor::CheckWinLose()
{
	FVector NewLocation = GetActorLocation();
	FVector WinLoseDist = ADynamicsActor::GetWinDistance();
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