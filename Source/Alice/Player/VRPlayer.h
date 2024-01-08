// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "VRPlayer.generated.h"

UCLASS()
class ALICE_API AVRPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AVRPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void AddLocation(FVector InDeltaLocation);

	TObjectPtr<class APlayerController> PlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Values")
	float DesiredHMDHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Inputs")
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Inputs")
	TArray<class UInputAction*> InputActions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Components")
	TObjectPtr<class USceneComponent> DefaultSceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Components")
	TObjectPtr<class USceneComponent> VROrigin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Components")
	TObjectPtr<class UCameraComponent> FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Components")
	TObjectPtr<class UStaticMeshComponent> HMDMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Components")
	TObjectPtr<class UMotionControllerComponent> LeftController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Components")
	TObjectPtr<class USkeletalMeshComponent> LeftHand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Components")
	TObjectPtr<class UMotionControllerComponent> RightController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Components")
	TObjectPtr<class USkeletalMeshComponent> RightHand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Components")
	TObjectPtr<class UMoveComponent> MoveComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Components")
	TObjectPtr<class UGrabComponent> GrabComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Components")
	TObjectPtr<class UWheelComponent> WheelComponent;
};
