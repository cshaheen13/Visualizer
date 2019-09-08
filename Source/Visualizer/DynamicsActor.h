// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DynamicsActor.generated.h"

UCLASS()
class VISUALIZER_API ADynamicsActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADynamicsActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void MoveActor(float RunningTime);

	UPROPERTY(BlueprintReadWrite)
	bool testPush = false;

	float RunningTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InitialVelocity;

	UPROPERTY(EditAnywhere)
	float KineticFriction;

	UPROPERTY(EditAnywhere)
	float Gravity;

	float accelX;

};
