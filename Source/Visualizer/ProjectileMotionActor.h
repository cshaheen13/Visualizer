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
	AActor* ActorThatWins;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* HoopLocationCylinder;

	UPROPERTY(EditAnywhere)
	class ACameraActor* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InitialVelocity;

	UPROPERTY(EditAnywhere)
	float Gravity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InitialAngle;

	UPROPERTY(BlueprintReadOnly)
	float InitialZLoc;

	UPROPERTY(BlueprintReadOnly)
	float InitialXLoc;

	UPROPERTY(BlueprintReadOnly)
	bool IsGameOver = false;

	UPROPERTY(BlueprintReadOnly)
	bool isImpulse = false;

	UPROPERTY(BlueprintReadWrite)
	bool isOverlapGoal = false;

	UPROPERTY(BlueprintReadWrite)
	bool IsPathHidden = true;

	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* Overlap;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION(BlueprintCallable)
	void SetDistanceText(float initialX, float initialZ);

	UFUNCTION(BlueprintCallable)
	void SetProjectileText(float initialX, float initialZ, bool PathHidden);

	UFUNCTION(BlueprintCallable)
	void SetTextActorVisible(bool PathHidden);

	//UFUNCTION(BlueprintCallable)
	//void ResetShot(FVector location);

	void DeleteProjectileText();

	UFUNCTION(BlueprintCallable)
	void SetHitGroundTime(float initialZ, float initialV);

	float GetHitGroundTime();

	float ShotDistance;

	bool IsTextRendered = false;

	bool IsDistanceTextRendered = false;

	UPROPERTY(BlueprintReadWrite)
	FString SelectedLocation;

	UFUNCTION(BlueprintCallable)
	void SetupShot(FString Location);

	UPROPERTY(BlueprintReadOnly)
	FVector OriginalLocation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	

	class ATextRenderActor* ProjectileMotionMapText;

	class ATextRenderActor* DistanceXText;

	class ATextRenderActor* HeightTextGoal;

	class ATextRenderActor* HeightTextBall;

	float RunningTime;

	float AccelX;
	
	float OldZ;

	float OldX;

	float ImpulseVelocityZ;

	float VelX;

	float VelZ;

	float RadiusBall = 12;

	float CoeffRestitution = .83;

	float ImpulseTime;

	float InitV;

	float t;

	float HitGroundTime;
	
	float VelocityZHit;

	float TimeMaxZ;

	float ZMax;

	float FreeThrowDist = -40;

	float ThreePointDist = -285;

	float HalfCourtDist = -800;

	FVector CameraLoc;

	FVector FreeThrowCameraLoc = FVector(225, 1635, 125);

	FVector ThreePointCameraLoc = FVector(115, 1810, 180);

	FVector HalfCourtCameraLoc = FVector(-100, 2000, 180);

	float CenterPoint;

	FVector HoopLocation;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void MoveProjectileMotionActor(float DeltaTime);

	void MoveCamera(ACameraActor* Camera, FVector CameraLocation);

	float QuadraticEquation(float Gravity, float InitialAngle, float InitialV, float RadiusBall, float InitialZ);

	int count = 0;

};
