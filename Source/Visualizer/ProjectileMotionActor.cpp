// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileMotionActor.h"
#include "SpawnTextActorComponent.h"
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Components/PrimitiveComponent.h"
#include "Engine/TextRenderActor.h"
#include "Components/TextRenderComponent.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "EngineUtils.h"
#include "MyBlueprintFunctionLibrary.h"
#include "Misc/Char.h"

// Sets default values
AProjectileMotionActor::AProjectileMotionActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Overlap = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CAPSULE"));
	Overlap->InitCapsuleSize(16, 16.f);
	Overlap->SetCollisionProfileName(TEXT("Trigger"));
	Overlap->SetupAttachment(RootComponent);
	Overlap->OnComponentBeginOverlap.AddDynamic(this, &AProjectileMotionActor::BeginOverlap);
	Overlap->OnComponentBeginOverlap.AddDynamic(this, &AProjectileMotionActor::EndOverlap);
}

// Called when the game starts or when spawned
void AProjectileMotionActor::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
	OriginalLocation = GetActorLocation();
	SetActorLocation(OriginalLocation);
	InitialZLoc = OriginalLocation.Z;
	InitialXLoc = OriginalLocation.X;
	UE_LOG(LogClass, Warning, TEXT("V = %f"), InitialVelocity);
	UE_LOG(LogClass, Warning, TEXT("angle in radians = %f"), FMath::Sin(FMath::DegreesToRadians(InitialAngle))); 
}

// Called every frame
void AProjectileMotionActor::Tick(float DeltaTime)
{
	MoveProjectileMotionActor(DeltaTime);
}

void AProjectileMotionActor::SetupShot(FString Location)
{
	if (Location == "Free Throw")
	{
		OriginalLocation = FVector(FreeThrowDist, 925.0f, 212.f);
		CameraLoc = FreeThrowCameraLoc;
	}
	else if (Location == "Three Point")
	{
		OriginalLocation = FVector(ThreePointDist, 925.0f, 212.f);
		CameraLoc = ThreePointCameraLoc;
	}
	else
	{
		OriginalLocation = FVector(HalfCourtDist, 925.0f, 212.f);
		CameraLoc = HalfCourtCameraLoc;
	}
	
	UE_LOG(LogClass, Warning, TEXT("Loc = %s"), *Location);
	SetActorLocation(OriginalLocation);
	MoveCamera(Camera, CameraLoc);

	InitialZLoc = OriginalLocation.Z;
	InitialXLoc = OriginalLocation.X;

	HoopLocation = HoopLocationCylinder->GetActorLocation();
	ShotDistance = HoopLocation.X - InitialXLoc;
	UE_LOG(LogClass, Warning, TEXT("ShotDistance = %f"), ShotDistance);

	CenterPoint = InitialXLoc + ShotDistance / 2;
	SetDistanceText(InitialXLoc, InitialZLoc);

}

void AProjectileMotionActor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor && (OtherActor != this))
	{
		UE_LOG(LogClass, Warning, TEXT("Overlap Begins"));
	}
	isImpulse = true;
}

void AProjectileMotionActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	UE_LOG(LogClass, Warning, TEXT("Overlap Ends"));
}

void AProjectileMotionActor::SetHitGroundTime(float initialZ, float initialV)
{
	HitGroundTime = QuadraticEquation(Gravity, InitialAngle, initialV, RadiusBall, initialZ);
}

float AProjectileMotionActor::GetHitGroundTime()
{
	return HitGroundTime;
}

float AProjectileMotionActor::QuadraticEquation(float Gravity, float InitialAngle, float InitialV, float RadiusBall, float InitialZ)
{
	float a = -100 * .5 * Gravity;
	float b = 100 * InitialV * FMath::Sin(FMath::DegreesToRadians(InitialAngle));
	float c = InitialZ - RadiusBall;

	float root1 = (-b + FMath::Sqrt(FMath::Square(b) - (4 * a * c))) / (2 * a);
	UE_LOG(LogClass, Warning, TEXT("root1 = %f"), root1);
	float root2 = (-b - FMath::Sqrt(FMath::Square(b) - (4 * a * c))) / (2 * a);
	UE_LOG(LogClass, Warning, TEXT("root2 = %f"), root2);

	return root2;
}

void AProjectileMotionActor::MoveCamera(ACameraActor* Camera, FVector CameraLocation)
{
	UCameraComponent * Comp = Camera->GetCameraComponent();
	Comp->SetWorldLocation(CameraLocation);
}

void AProjectileMotionActor::SetDistanceText(float initialX, float initialZ)
{
	FString ShotDistanceString = "X = " + (FString::SanitizeFloat(ShotDistance / 100)) + " meters";

	if (IsDistanceTextRendered == false)
	{
		USpawnTextActorComponent* GetSpawnText = Cast<USpawnTextActorComponent>(ActorThatWins->GetComponentByClass(USpawnTextActorComponent::StaticClass()));
		if (GetSpawnText)
		{		
			DistanceXText = GetSpawnText->SpawnText(DistanceXText, InitialXLoc + ShotDistance / 2, -260, 150, 0, 90, 0, ShotDistanceString, "DistanceTextX", FColor::Black, EHTA_Center, EVRTA_TextCenter, 1, 100, 100, false);
			HeightTextGoal = GetSpawnText->SpawnText(HeightTextGoal, 745, 910, 150, 0, 180, 0, "Y(g) = 3.05 meters", "HeightTextGoal", FColor::Black, EHTA_Center, EVRTA_TextCenter, 1, 50, 50, false);
			HeightTextBall = GetSpawnText->SpawnText(HeightTextBall, InitialXLoc, 925, 180, 0, 90, 0, "Y(b) = 2.12 meters", "HeightTextBall", FColor::White, EHTA_Center, EVRTA_TextCenter, 1, 25, 25, false);
			IsDistanceTextRendered = true;
		}
		else {
			UE_LOG(LogClass, Warning, TEXT("No Spawn Text Component"));
		}
	}
	else
	{
		if (DistanceXText != NULL && HeightTextBall != NULL) {
			DistanceXText->GetTextRender()->SetText(FString(ShotDistanceString));
			DistanceXText->SetActorLocation(FVector(InitialXLoc + ShotDistance / 2, -260.f, 150.f));
			HeightTextBall->SetActorLocation(FVector(InitialXLoc, 925.f, 185.f));
		}
		else {
			UE_LOG(LogClass, Warning, TEXT("NULL DistanceXText, HeightTextBall"));
		}

	}
}

void AProjectileMotionActor::SetProjectileText(float initialX, float initialZ, bool PathHidden)
{	
	if (IsTextRendered)
	{
		DeleteProjectileText();
		IsTextRendered = false;
	}

	InitV = InitialVelocity;
	OldX = initialX;
	OldZ = initialZ;
	FString WinDistanceString = "|";

	TimeMaxZ = InitV * FMath::Sin(FMath::DegreesToRadians(InitialAngle)) / Gravity;
	ZMax = initialZ + (InitV * FMath::Sin(FMath::DegreesToRadians(InitialAngle)) * TimeMaxZ) - (.5 * Gravity * TimeMaxZ * TimeMaxZ);
	UE_LOG(LogClass, Warning, TEXT("tmax = %f"), TimeMaxZ);
	UE_LOG(LogClass, Warning, TEXT("zmax = %f"), ZMax);

	for (float t = .05; t < 5; t += .075)
	{
		float deltaX = initialX + (100 * (InitV * FMath::Cos(FMath::DegreesToRadians(InitialAngle)) * (t)));
		float deltaZ = initialZ + (100 * ((InitV * FMath::Sin(FMath::DegreesToRadians(InitialAngle)) * (t)) - (.5 * Gravity * (t) * (t))));
		float VelSlope = (deltaZ - OldZ) / (deltaX - OldX);
		float angle = FMath::RadiansToDegrees(FMath::Atan(VelSlope)) - 90;


		USpawnTextActorComponent* GetSpawnText = Cast<USpawnTextActorComponent>(ActorThatWins->GetComponentByClass(USpawnTextActorComponent::StaticClass()));
		if (GetSpawnText)
		{
			ProjectileMotionMapText = GetSpawnText->SpawnText(ProjectileMotionMapText, deltaX, 925, deltaZ, 0, 90, angle, WinDistanceString, "NULL", FColor::Red, EHTA_Center, EVRTA_TextCenter, 1, 50, 25, PathHidden);
		}
		else
		{
			UE_LOG(LogClass, Warning, TEXT("No Spawn Text Component"));
		}

		VelX = InitV * FMath::Cos(FMath::DegreesToRadians(InitialAngle));
		VelZ = (InitV * FMath::Sin(FMath::DegreesToRadians(InitialAngle))) - (Gravity * t);

		if (GetHitGroundTime() - t <= .05)
		{
			deltaX = initialX + (100 * (InitV * FMath::Cos(FMath::DegreesToRadians(InitialAngle)) * (GetHitGroundTime())));
			deltaZ = initialZ + (100 * ((InitV * FMath::Sin(FMath::DegreesToRadians(InitialAngle)) * (GetHitGroundTime())) - (.5 * Gravity * (GetHitGroundTime()) * (GetHitGroundTime()))));
			VelocityZHit = (InitV * FMath::Sin(FMath::DegreesToRadians(InitialAngle))) - (Gravity * GetHitGroundTime());
			VelZ = VelocityZHit * CoeffRestitution;
			InitV = (FMath::Sqrt((VelX * VelX) + (VelZ * VelZ)));
			SetHitGroundTime(deltaZ, InitV);
			t = .05;
			initialX = deltaX;
			initialZ = deltaZ;
			UE_LOG(LogClass, Warning, TEXT("t = %f"), t);
			UE_LOG(LogClass, Warning, TEXT("x = %f"), deltaX);
			UE_LOG(LogClass, Warning, TEXT("z = %f"), deltaZ);
		}

		OldX = deltaX;
		OldZ = deltaZ;

		if ((InitV < 1) || (deltaX > 525))
		{
			IsTextRendered = true;
			break;
		}
	}
}

//void AProjectileMotionActor::ResetShot(FVector location)
//{
//	SetActorLocation(OriginalLocation);
//}

void AProjectileMotionActor::SetTextActorVisible(bool PathHidden)
{
	for (TActorIterator<ATextRenderActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
		ATextRenderActor *Text = *ActorItr;
		if ((Text->GetName() != "DistanceTextX") && (Text->GetName() != "HeightTextGoal") && (Text->GetName() != "HeightTextBall")) 
		{
			Text->GetTextRender()->SetHiddenInGame(PathHidden);;
		}
		
	}
}

void AProjectileMotionActor::DeleteProjectileText()
{
	for (TActorIterator<ATextRenderActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
		ATextRenderActor *Text = *ActorItr;
		if ((Text->GetName() != "DistanceTextX") && (Text->GetName() != "HeightTextGoal") && (Text->GetName() != "HeightTextBall"))
		{
			Text->Destroy();
		}

	}
}

void AProjectileMotionActor::MoveProjectileMotionActor(float DeltaTime)
{
	FVector NewLocation = GetActorLocation();
	FVector OldLocation = GetActorLocation();
	float OldTime = RunningTime;

	float deltaX = InitialXLoc + (100 * (InitialVelocity * FMath::Cos(FMath::DegreesToRadians(InitialAngle)) * (RunningTime)));
	float deltaZ = InitialZLoc + (100 * ((InitialVelocity * FMath::Sin(FMath::DegreesToRadians(InitialAngle)) * (RunningTime)) - (.5 * Gravity * (RunningTime) * (RunningTime))));

	VelX = InitialVelocity * FMath::Cos(FMath::DegreesToRadians(InitialAngle));
	VelZ = (InitialVelocity * FMath::Sin(FMath::DegreesToRadians(InitialAngle))) - (Gravity * RunningTime);

	NewLocation.X = deltaX;
	NewLocation.Z = deltaZ;	
	RunningTime += DeltaTime;
	SetActorLocation(NewLocation);

	if (isOverlapGoal)
	{
		SetActorTickEnabled(false);
	}

	if(isImpulse)
	{
		VelZ = VelZ * CoeffRestitution;
		InitialVelocity = (FMath::Sqrt((VelX * VelX) + (VelZ * VelZ)));
		ImpulseVelocityZ = InitialVelocity * FMath::Sin(FMath::DegreesToRadians(InitialAngle));
		UE_LOG(LogClass, Warning, TEXT("ImpVZ = %f"), ImpulseVelocityZ);

		FVector InitialImpulseLocation = GetActorLocation();
		RunningTime = 0;
		InitialXLoc = InitialImpulseLocation.X;
		InitialZLoc = InitialImpulseLocation.Z;
		isImpulse = false;

		if ((ImpulseVelocityZ < .5) || (deltaX >= 525))
		{
			SetActorTickEnabled(false);
			UPrimitiveComponent * Primitive = Cast<UPrimitiveComponent>(GetComponentByClass(UPrimitiveComponent::StaticClass()));
			Primitive->SetSimulatePhysics(true);
			UE_LOG(LogClass, Warning, TEXT("Sim Physics"));
		}

		//need to turn isimpulse off and end overlap
	}
}

//void AProjectileMotionActor::SpawnText(class ATextRenderActor* TextActo, float InitialX, float InitialY, float InitialZ, float Pitch, float Yaw, float Roll, FString TextString, FString TextName, FColor Color, enum EHorizTextAligment HorizAlign, enum EVerticalTextAligment VertAlign, float WorldSize, float XScal, float YSca, bool IsTextHid) {
//
//	TextActo = GetWorld()->SpawnActor<ATextRenderActor>(ATextRenderActor::StaticClass(), FVector(InitialX, InitialY, InitialZ), FRotator(Pitch, Yaw, Roll));
//	if (TextName != "NULL") {
//		FString MyTextVariable = TextName;
//		const TCHAR* TextName1 = *MyTextVariable;
//		TextActo->Rename(TextName1);
//	}
//	TextActo->GetTextRender()->SetText(FString(TextString));
//	TextActo->GetTextRender()->SetTextRenderColor(Color);
//	TextActo->GetTextRender()->SetHorizontalAlignment(HorizAlign);
//	TextActo->GetTextRender()->SetVerticalAlignment(VertAlign);
//	TextActo->GetTextRender()->SetWorldSize(WorldSize);
//	TextActo->GetTextRender()->SetXScale(XScal);
//	TextActo->GetTextRender()->SetYScale(YSca);
//	TextActo->GetTextRender()->SetHiddenInGame(IsTextHid);
//
//}

//{
//	InitV = InitialVelocity;
//	FVector NewLocation;
//	FVector OldLocation = GetActorLocation();
//	float initialX = OldLocation.X;
//	float initialZ = OldLocation.Z;
//	if (count == 0)
//	{
//		SetHitGroundTime(initialZ, InitV);
//		UE_LOG(LogClass, Warning, TEXT("hit time = %f"), HitGroundTime);
//		count++;
//	}
//	UE_LOG(LogClass, Warning, TEXT("count= %i"), count);
//
//	/*TimeMaxZ = InitV * FMath::Sin(FMath::DegreesToRadians(InitialAngle)) / Gravity;
//	ZMax = initialZ + (InitV * FMath::Sin(FMath::DegreesToRadians(InitialAngle)) * TimeMaxZ) - (.5 * Gravity * TimeMaxZ * TimeMaxZ);
//	UE_LOG(LogClass, Warning, TEXT("tmax = %f"), TimeMaxZ);
//	UE_LOG(LogClass, Warning, TEXT("zmax = %f"), ZMax);*/
//
//	float deltaX = initialX + ((InitV) * FMath::Cos(FMath::DegreesToRadians(InitialAngle)) * (RunningTime));
//	float deltaZ = initialZ + (((InitV) * FMath::Sin(FMath::DegreesToRadians(InitialAngle)) * (RunningTime)) - (.5 * Gravity * (RunningTime) * (RunningTime)));
//	UE_LOG(LogClass, Warning, TEXT("x = %f"), deltaX);
//	UE_LOG(LogClass, Warning, TEXT("z = %f"), deltaZ);
//	/*float VelSlope = (deltaZ - OldZ) / (deltaX - OldX);
//	float angle = FMath::RadiansToDegrees(FMath::Atan(VelSlope)) - 90;*/
//
//	NewLocation.X = deltaX;
//	NewLocation.Z = deltaZ;	
//	RunningTime += DeltaTime;
//	SetActorLocation(NewLocation);
//
//	VelX = InitV * FMath::Cos(FMath::DegreesToRadians(InitialAngle));
//	VelZ = (InitV * FMath::Sin(FMath::DegreesToRadians(InitialAngle))) - (Gravity * RunningTime);
//
//	if (GetHitGroundTime() - RunningTime <= .05)
//	{
//		deltaX = initialX + (100 * (InitV * FMath::Cos(FMath::DegreesToRadians(InitialAngle)) * (GetHitGroundTime())));
//		deltaZ = initialZ + (100 * ((InitV * FMath::Sin(FMath::DegreesToRadians(InitialAngle)) * (GetHitGroundTime())) - (.5 * Gravity * (GetHitGroundTime()) * (GetHitGroundTime()))));
//		VelocityZHit = (InitV * FMath::Sin(FMath::DegreesToRadians(InitialAngle))) - (Gravity * GetHitGroundTime());
//		VelZ = VelocityZHit * CoeffRestitution;
//		InitV = (FMath::Sqrt((VelX * VelX) + (VelZ * VelZ)));
//		SetHitGroundTime(deltaZ, InitV);
//		RunningTime = 0;
//		initialX = deltaX;
//		initialZ = deltaZ;
//		UE_LOG(LogClass, Warning, TEXT("t = %f"), t);
//		UE_LOG(LogClass, Warning, TEXT("x = %f"), deltaX);
//		UE_LOG(LogClass, Warning, TEXT("z = %f"), deltaZ);
//	}
//
//	//OldX = deltaX;
//	//OldZ = deltaZ;
//}