// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "VRPlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ALICE_API UVRPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UVRPlayerAnimInstance();

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
