// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/EngineTypes.h"
#include "PickupActor.generated.h"

UCLASS()
class ALICE_API APickupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Grabbed(class USkeletalMeshComponent* HandMesh, EAttachmentRule AttachmentRule);
	void Released(FVector DeltaPosition, FQuat DeltaRotation);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Components")
	TObjectPtr<class UBoxComponent> BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Components")
	TObjectPtr<class UStaticMeshComponent> MeshComponent;

	UPROPERTY(EditAnywhere, Category = "Settings|Values")
	TObjectPtr<class ACenterActor> CenterActor;
};
