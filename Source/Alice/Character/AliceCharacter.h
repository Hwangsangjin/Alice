// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "AliceCharacter.generated.h"

UCLASS()
class ALICE_API AAliceCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAliceCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Components")
	TObjectPtr<class UNiagaraComponent> Dust;

	void SetMovementMode(EMovementMode NewMovementMode);
	void Move(const FVector& ForwardDirection, const FVector& RightDirection, const FVector2D& InputValue);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bLadder;

	class UAliceAnimInstance* AnimInstance;
};
