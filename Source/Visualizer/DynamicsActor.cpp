// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicsActor.h"

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

}

void ADynamicsActor::MoveActor(float DeltaTime)
{
	FVector NewLocation = GetActorLocation();
	float accelX = KineticFriction * Gravity;
	//float finalTime = -(InitialVelocity / accelX);
	float deltaX = 100 * ((InitialVelocity * RunningTime) + ((accelX)*(RunningTime)*(RunningTime)*.5));
	UE_LOG(LogClass, Warning, TEXT("t = %f"), RunningTime);
	UE_LOG(LogClass, Warning, TEXT("X = %f"), deltaX);

	if ((deltaX - NewLocation.X) >= 0)
	{
		NewLocation.X = deltaX;
		RunningTime += DeltaTime;
		SetActorLocation(NewLocation);
	}
}

// Called every frame
void ADynamicsActor::Tick(float DeltaTime)
{
	MoveActor(DeltaTime);
}

