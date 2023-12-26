// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/GrabComponent.h"
#include "Player/VRPlayer.h"
#include "EnhancedInputComponent.h"
#include "Components/TextRenderComponent.h"
#include "Actor/PickupActor.h"

// Sets default values for this component's properties
UGrabComponent::UGrabComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabComponent::BeginPlay()
{
	Super::BeginPlay();

	Player = GetOwner<AVRPlayer>();
	
}


// Called every frame
void UGrabComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UGrabComponent::SetupPlayerInputComponent(UEnhancedInputComponent* EnhancedInputComponent, TArray<class UInputAction*> InputActions)
{
	EnhancedInputComponent->BindAction(InputActions[1], ETriggerEvent::Started, this, &UGrabComponent::GrabObject);
}

void UGrabComponent::GrabObject()
{
	Player->RightLog->SetText(FText::FromString(FString("Try Grabbed!")));

	UWorld* World = GetWorld();
	if (World)
	{
		FHitResult HitResult;
		FVector Start = Player->RightHand->GetComponentLocation();
		FVector End = Start + Player->RightHand->GetRightVector() * 30.0f;

		if (World->LineTraceSingleByProfile(HitResult, Start, End, FName("Pickup")))
		{
			APickupActor* PickedObject = Cast<APickupActor>(HitResult.GetActor());
			if (PickedObject)
			{
				PickedObject->Grabbed(Player->RightHand);
			}
		}
	}
}

