// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileMotionActor.h"
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Components/PrimitiveComponent.h"
#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"
#include "Runtime/Engine/Classes/Engine/TextRenderActor.h"
#include "Components/TextRenderComponent.h"
#include "EngineUtils.h"

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
	//SetHitGroundTime(InitialZLoc, InitialVelocity);
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
		OriginalLocation = FVector(FreeThrowDist, 925.0f, 212.312f);
	}
	else if (Location == "Three Point")
	{
		OriginalLocation = FVector(ThreePointDist, 925.0f, 212.312f);
	}
	else
	{
		OriginalLocation = FVector(HalfCourtDist, 925.0f, 212.312f);
	}
	
	UE_LOG(LogClass, Warning, TEXT("Loc = %s"), *Location);
	SetActorLocation(OriginalLocation);
	InitialZLoc = OriginalLocation.Z;
	InitialXLoc = OriginalLocation.X;
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

void AProjectileMotionActor::SetProjectileText(float initialX, float initialZ)
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

		WinDistanceText = GetWorld()->SpawnActor<ATextRenderActor>(ATextRenderActor::StaticClass(), FVector(deltaX, 925.f, deltaZ), FRotator(0.f, 90.f, angle));
		WinDistanceText->GetTextRender()->SetText(FString(WinDistanceString));
		WinDistanceText->GetTextRender()->SetTextRenderColor(FColor::Red);
		WinDistanceText->GetTextRender()->SetHorizontalAlignment(EHTA_Center);
		WinDistanceText->GetTextRender()->SetVerticalAlignment(EVRTA_TextCenter);
		WinDistanceText->GetTextRender()->SetWorldSize(1);
		WinDistanceText->GetTextRender()->SetXScale(50);
		WinDistanceText->GetTextRender()->SetYScale(25);

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

void AProjectileMotionActor::DeleteProjectileText()
{
	for (TActorIterator<ATextRenderActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
		ATextRenderActor *Text = *ActorItr;
		Text->Destroy();
	}
}

void AProjectileMotionActor::MoveProjectileMotionActor(float DeltaTime)
{
	FVector NewLocation = GetActorLocation();
	FVector OldLocation = GetActorLocation();
	float OldTime = RunningTime;

	float deltaX = InitialXLoc + (100 * (InitialVelocity * FMath::Cos(FMath::DegreesToRadians(InitialAngle)) * (RunningTime)));
	float deltaZ = InitialZLoc + (100 * ((InitialVelocity * FMath::Sin(FMath::DegreesToRadians(InitialAngle)) * (RunningTime)) - (.5 * Gravity * (RunningTime) * (RunningTime))));
	
	//VelX = (NewLocation.X - OldLocation.X) / (RunningTime - OldTime);
	//VelZ = (NewLocation.Z - OldLocation.Z) / (RunningTime - OldTime);

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

		if (ImpulseVelocityZ < .5)
		{
			SetActorTickEnabled(false);
			UPrimitiveComponent * Primitive = Cast<UPrimitiveComponent>(GetComponentByClass(UPrimitiveComponent::StaticClass()));
			Primitive->SetSimulatePhysics(true);
			UE_LOG(LogClass, Warning, TEXT("Sim Physics"));
		}

		//need to turn isimpulse off and end overlap
	}
}