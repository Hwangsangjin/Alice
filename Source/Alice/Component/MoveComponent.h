// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputActionValue.h"
#include "MoveComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALICE_API UMoveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMoveComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetupPlayerInputComponent(class UEnhancedInputComponent* EnhancedInputComponent, TArray<class UInputAction*> Inputs);

	UPROPERTY(EditAnywhere, Category = "Settings")
	float LineSpeed = 500.0f;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float TeleportDelay = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Settings")
	TSubclassOf<class ATeleportRingActor> TeleportRingBP;

private:
	void ShowLine(const FInputActionValue& Value);
	void DrawTrajectory(FVector Start, FVector Direction, int32 Segment, float Speed, float Interval);
	void DrawTrajectoryBezier(FVector Start, FVector Direction, int32 Segment);
	void Teleport();

	TObjectPtr<class AVRPlayer> Player;
	FVector TargetLocation;

	TObjectPtr<class ATeleportRingActor> TeleportRingInst;
};
