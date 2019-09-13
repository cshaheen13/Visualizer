// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileMotionActor.generated.h"

UCLASS()
class VISUALIZER_API AProjectileMotionActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileMotionActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AActor* ActorThatWins;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InitialVelocity;

	UPROPERTY(EditAnywhere)
	float Gravity;

	UPROPERTY(EditAnywhere)
	float InitialAngle;

	UPROPERTY(BlueprintReadOnly)
	bool IsGameOver = false;

	UPROPERTY(BlueprintReadWrite)
	bool isOverlapGoal = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	

	float RunningTime;

	float AccelX;

	FVector OriginalLocation;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void MoveProjectileMotionActor(float DeltaTime);

	void Setup();

};
