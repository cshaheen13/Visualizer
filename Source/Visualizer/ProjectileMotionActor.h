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

	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* Overlap;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION(BlueprintCallable)
	void SetProjectileText(float initialX, float initialZ);

	void DeleteProjectileText();

	UFUNCTION(BlueprintCallable)
	void SetHitGroundTime(float initialZ, float initialV);

	float GetHitGroundTime();

	bool IsTextRendered = false;

	UPROPERTY(BlueprintReadWrite)
	FString SelectedLocation;

	UFUNCTION(BlueprintCallable)
	void SetupShot(FString Location);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	

	class ATextRenderActor* WinDistanceText;

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

	float FreeThrowDist = -100;

	float ThreePointDist = -375;

	float HalfCourtDist = -915;

	FVector OriginalLocation;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void MoveProjectileMotionActor(float DeltaTime);

	float QuadraticEquation(float Gravity, float InitialAngle, float InitialV, float RadiusBall, float InitialZ);

};
