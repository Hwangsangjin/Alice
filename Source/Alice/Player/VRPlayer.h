// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "VRPlayer.generated.h"

UCLASS()
class ALICE_API AVRPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVRPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Components")
	TObjectPtr<class UCameraComponent> FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Components")
	TObjectPtr<class UStaticMeshComponent> HMDMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Components")
	TObjectPtr<class UMotionControllerComponent> LeftController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Components")
	TObjectPtr<class USkeletalMeshComponent> LeftHand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Components")
	TObjectPtr<class UTextRenderComponent> LeftLog;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Components")
	TObjectPtr<class UMotionControllerComponent> RightController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Components")
	TObjectPtr<class USkeletalMeshComponent> RightHand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Components")
	TObjectPtr<class UTextRenderComponent> RightLog;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Components")
	TObjectPtr<class UMoveComponent> MoveComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Components")
	TObjectPtr<class UGrabComponent> GrabComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Components")
	TObjectPtr<class UNiagaraComponent> TeleportVFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Inputs")
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Inputs")
	TObjectPtr<class UInputAction> RightTriggerBool;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Inputs")
	TObjectPtr<class UInputAction> RightTriggerFloat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Inputs")
	TObjectPtr<class UInputAction> RightTriggerTouch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Inputs")
	TObjectPtr<class UInputAction> RightThumbStick;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Inputs")
	TObjectPtr<class UInputAction> LeftTriggerBool;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Inputs")
	TObjectPtr<class UInputAction> MoveAxis2D;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Inputs")
	TObjectPtr<class UInputAction> TurnAxis2D;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Inputs")
	TArray<class UInputAction*> InputActions;

private:
	void RightTriggerInput_Bool(const FInputActionValue& Value);
	void RightTriggerInput_Float(const FInputActionValue& Value);
	void RightTriggerInput_Touch(const FInputActionValue& Value);
	void RightThumbStickInput(const FInputActionValue& Value);
	void Move(const FInputActionValue& Value);
	void Turn(const FInputActionValue& Value);
	void Teleport(float SightRange, FVector Direction, FVector Pivot);
};
