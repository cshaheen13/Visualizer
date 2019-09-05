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

}

// Called every frame
void ADynamicsActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector NewLocation = GetActorLocation();
	float accelX = KineticFriction * Gravity;
	float finalTime = InitialVelocity / accelX;
	float deltaX = 100 * ((InitialVelocity * RunningTime) + ((accelX)*(RunningTime)*(RunningTime)*.5));
	
	if ((deltaX - NewLocation.X) >= 0)
	{
		NewLocation.X = deltaX;
		RunningTime += DeltaTime;
		SetActorLocation(NewLocation);
	}
}

