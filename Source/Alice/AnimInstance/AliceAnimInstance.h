// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AliceAnimInstance.generated.h"

/**
 *
 */
UCLASS()
class ALICE_API UAliceAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	class AAliceCharacter* Alice;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerAnim")
	float Speed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerAnim")
	float Direction;
};
