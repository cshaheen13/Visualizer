// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpawnTextActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VISUALIZER_API USpawnTextActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpawnTextActorComponent();

	class ATextRenderActor* SpawnText(class ATextRenderActor* TextActo, float InitialX, float InitialY, float InitialZ, float Pitch, float Yaw, float Roll, FString TextString, FString TextName, FColor Color, enum EHorizTextAligment HorizAlign, enum EVerticalTextAligment VertAlign, float WorldSize, float XScal, float YSca, bool IsTextHid);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
