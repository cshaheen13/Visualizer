// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnTextActorComponent.h"
#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"
#include "Engine/TextRenderActor.h"
#include "Components/TextRenderComponent.h"

// Sets default values for this component's properties
USpawnTextActorComponent::USpawnTextActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

ATextRenderActor* USpawnTextActorComponent::SpawnText(class ATextRenderActor* TextActo, float InitialX, float InitialY, float InitialZ, float Pitch, float Yaw, float Roll, FString TextString, FString TextName, FColor Color, enum EHorizTextAligment HorizAlign, enum EVerticalTextAligment VertAlign, float WorldSize, float XScal, float YSca, bool IsTextHid) {

	TextActo = GetWorld()->SpawnActor<ATextRenderActor>(ATextRenderActor::StaticClass(), FVector(InitialX, InitialY, InitialZ), FRotator(Pitch, Yaw, Roll));
	if (TextName != "NULL") {
		FText MyText = FText::FromString(TextString);
		const TCHAR* MyTextName = *TextName;
		TextActo->Rename(MyTextName);
		TextActo->GetTextRender()->SetText(MyText);
	}
	TextActo->GetTextRender()->SetTextRenderColor(Color);
	TextActo->GetTextRender()->SetHorizontalAlignment(HorizAlign);
	TextActo->GetTextRender()->SetVerticalAlignment(VertAlign);
	TextActo->GetTextRender()->SetWorldSize(WorldSize);
	TextActo->GetTextRender()->SetXScale(XScal);
	TextActo->GetTextRender()->SetYScale(YSca);
	TextActo->GetTextRender()->SetHiddenInGame(IsTextHid);

	return TextActo;

}


// Called when the game starts
void USpawnTextActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USpawnTextActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

