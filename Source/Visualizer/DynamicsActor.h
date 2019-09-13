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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float KineticFriction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Gravity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector WinDistance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float AccelX;

	UPROPERTY(BlueprintReadOnly)
	bool DoYouWin = false;

	UPROPERTY(BlueprintReadOnly)
	bool IsGameOver = false;

	float SetupDistance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
		
	float RunningTime;

	float GetAccelX();

	void SetAccelX(float Gravity, float KineticFriction);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector GetWinDistance();

	void SetWinDistance(float x, float y, float z);

	void Setup();

	void MoveKineticFrictionActor(float RunningTime);

	void CheckWinLose();
};
