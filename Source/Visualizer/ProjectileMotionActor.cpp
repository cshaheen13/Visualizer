// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileMotionActor.h"
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"

// Sets default values
AProjectileMotionActor::AProjectileMotionActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AProjectileMotionActor::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
	FVector OriginalLocation = GetActorLocation();
	UE_LOG(LogClass, Warning, TEXT("z = %f"), OriginalLocation.Z);
	UE_LOG(LogClass, Warning, TEXT("angle in radians = %f"), FMath::Sin(FMath::DegreesToRadians(InitialAngle))); 
//	for (int i = 0; i <= 30; i++)
//	{
//		float deltaX = 100 * (InitialVelocity * FMath::Cos(FMath::DegreesToRadians(InitialAngle)) * i);
//		float deltaZ = 100 * ((InitialVelocity * FMath::Sin(FMath::DegreesToRadians(InitialAngle))) - (.5 * Gravity * i * i));
//		UE_LOG(LogClass, Warning, TEXT("i = %f"), i);
//		UE_LOG(LogClass, Warning, TEXT("x = %f"), deltaX);
//		UE_LOG(LogClass, Warning, TEXT("z = %f"), deltaZ);
//	}
}

// Called every frame
void AProjectileMotionActor::Tick(float DeltaTime)
{
	MoveProjectileMotionActor(DeltaTime);
}

void AProjectileMotionActor::Setup()
{
	//SetActorTickEnabled(false);
	//SetAccelX(KineticFriction, Gravity);

}

void AProjectileMotionActor::MoveProjectileMotionActor(float DeltaTime)
{
	FVector NewLocation = GetActorLocation();
	float InitialZ = OriginalLocation.Z;
	float deltaX = 100 * (InitialVelocity * FMath::Cos(FMath::DegreesToRadians(InitialAngle)) * RunningTime);
	float deltaZ = 213.36 + (100 * ((InitialVelocity * FMath::Sin(FMath::DegreesToRadians(InitialAngle)) * RunningTime) - (.5 * Gravity * RunningTime * RunningTime)));

	UE_LOG(LogClass, Warning, TEXT("initialZ = %f"), InitialZ);
	UE_LOG(LogClass, Warning, TEXT("t = %f"), RunningTime);

	NewLocation.X = deltaX;
	UE_LOG(LogClass, Warning, TEXT("X = %f"), deltaX);
	NewLocation.Z = deltaZ;	
	UE_LOG(LogClass, Warning, TEXT("Z = %f"), deltaZ);
	RunningTime += DeltaTime;
	SetActorLocation(NewLocation);

	if (isOverlapGoal)
	{
		SetActorTickEnabled(false);
	}
	/*if ((RunningTime >= 0.1) && ((NewLocation.X - OldLocation.X) <= 0.02))
	{
		ADynamicsActor::CheckWinLose();
	}*/
}