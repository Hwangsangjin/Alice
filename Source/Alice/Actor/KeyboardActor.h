// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KeyboardActor.generated.h"

UCLASS()
class ALICE_API AKeyboardActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKeyboardActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnHitKey(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void RestKeyColor();

	UPROPERTY(EditAnywhere, Category = "Settings|Components")
	TObjectPtr<class UBoxComponent> BoxComponent;

	UPROPERTY(EditAnywhere, Category = "Settings|Components")
	TObjectPtr<class UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(EditAnywhere, Category = "Settings|Resources")
	TSubclassOf<class USoundBase> KeySound;

	TObjectPtr<class UMaterialInstanceDynamic> KeyMaterial;
};
