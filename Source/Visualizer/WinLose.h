// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "Components/ActorComponent.h"
#include "WinLose.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VISUALIZER_API UWinLose : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWinLose();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* WinConditionTrigger;

	UPROPERTY(EditAnywhere)
	AActor* ActorThatWins;

	UPROPERTY(VisibleAnywhere)
	FVector WinDistance;

	class ARectLight* WinDistanceLight2;

	FVector WinLightDistance;
	
	FRotator WinLightRotation;

	class ATextRenderActor* WinDistanceText;

	FVector GetWinDistance();

	void SetWinDistance(float x, float y, float z);
};
