// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WheelComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALICE_API UWheelComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWheelComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetupPlayerInputComponent(class UEnhancedInputComponent* EnhancedInputComponent, TArray<class UInputAction*> InputActions);

	float GetControllerAngle(FVector LeftControllerLocation, FVector RightControllerLocation, FVector BaseDirection) const;

	void LeftGripOn();
	void LeftGripOff();
	void RightGripOn();
	void RightGripOff();

private:
	TObjectPtr<class AVRPlayer> Player;
	TObjectPtr<class ACenterActor> CenterActor;

	bool bPressdLeftGrip;
	bool bPressdRightGrip;
};
