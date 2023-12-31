// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CenterActor.generated.h"

UCLASS()
class ALICE_API ACenterActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACenterActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AddRotation(FRotator DeltaRotation);

	TObjectPtr<class AAliceCharacter> AliceCharacter;
	TArray<class ARabbitCharacter*> RabbitCharacters;
};
