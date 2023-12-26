// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BeziercurveActor.generated.h"

UCLASS()
class ALICE_API ABeziercurveActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABeziercurveActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Settings|Components")
	TObjectPtr<class USceneComponent> p0;

	UPROPERTY(EditAnywhere, Category = "Settings|Components")
	TObjectPtr<class USceneComponent> p1;

	UPROPERTY(EditAnywhere, Category = "Settings|Components")
	TObjectPtr<class USceneComponent> p2;

	UPROPERTY(EditAnywhere, Category = "Settings|Components", Meta = (ClampMin = 0.0, ClampMax = 1.0, UIMin = 0.0, UIMax = 1.0))
	float t;

private:
	void DrawBezierCurve(FVector Location0, FVector Location1, FVector Location2);
};
