// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "DynamicsActor.generated.h"



UCLASS()
class VISUALIZER_API ADynamicsActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADynamicsActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AActor* ActorThatWins;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InitialVelocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector WinDistance;

	UPROPERTY(EditAnywhere)
	float KineticFriction;

	UPROPERTY(EditAnywhere)
	float Gravity;

	UPROPERTY(BlueprintReadOnly)
	bool GameOver = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	

	class ARectLight* WinLight;

	class ATextRenderActor* WinDistanceText;
		
	float RunningTime;

	float RandDistance;

	float AccelX;

	float GetAccelX();

	void SetAccelX(float KineticFriction, float Gravity);

	FVector WinLightLocation;

	FRotator WinLightRotation;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector GetWinDistance();

	void SetWinDistance(float x, float y, float z);

	void Setup();

	void SetWinText();

	void SetWinLight();

	void MoveActor(float RunningTime);

	void CheckWinLose();
};
