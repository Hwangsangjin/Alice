// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/GrabComponent.h"
#include "EnhancedInputComponent.h"
#include "Player/VRPlayer.h"
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

	if (CurrentObject)
	{
		DeltaLocation = Player->RightHand->GetComponentLocation() - PrevLocation;
		PrevLocation = Player->RightHand->GetComponentLocation();

		//DeltaQuat = Player->RightHand->GetComponentQuat() * PrevQuat.Inverse();
		DeltaQuat = Player->RightHand->GetComponentQuat() - PrevQuat;
		PrevQuat = Player->RightHand->GetComponentQuat();
	}
}

void UGrabComponent::SetupPlayerInputComponent(UEnhancedInputComponent* EnhancedInputComponent, TArray<class UInputAction*> InputActions)
{
	EnhancedInputComponent->BindAction(InputActions[1], ETriggerEvent::Started, this, &UGrabComponent::GrabObject);
	EnhancedInputComponent->BindAction(InputActions[1], ETriggerEvent::Completed, this, &UGrabComponent::ReleaseObject);
	EnhancedInputComponent->BindAction(InputActions[4], ETriggerEvent::Started, this, &UGrabComponent::GrabObject);
	EnhancedInputComponent->BindAction(InputActions[4], ETriggerEvent::Completed, this, &UGrabComponent::ReleaseObject);
}

void UGrabComponent::GrabObject()
{
	if (CurrentObject)
	{
		return;
	}

	UWorld* World = GetWorld();

	FHitResult HitResult;

	const FVector Start = Player->RightHand->GetSocketLocation(TEXT("GrabSocket"));

	const float TraceRange = 200.0f;
	const FVector End = Start + Player->RightHand->GetRightVector() * TraceRange;

	const float SphereRadius = 100.0f;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(SphereRadius);

	const bool bHasHit = World->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel2, Sphere);
	if (bHasHit)
	{
		CurrentObject = Cast<APickupActor>(HitResult.GetActor());
		if (CurrentObject)
		{
			CurrentObject->Grabbed(Player->RightHand, EAttachmentRule::KeepWorld);
		}
	}

	FVector TraceVec = Player->RightHand->GetRightVector() * TraceRange;
	FVector Center = Player->RightHand->GetComponentLocation() + TraceVec * 0.5f;
	const float HalfHeight = TraceRange * 0.5f + SphereRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bHasHit ? FColor::Green : FColor::Red;
	const float DebugLifeTime = 3.0f;
	//DrawDebugCapsule(World, Center, HalfHeight, SphereRadius, CapsuleRot, DrawColor, false, DebugLifeTime);
}

void UGrabComponent::ReleaseObject()
{
	if (CurrentObject)
	{
		CurrentObject->Released(DeltaLocation, DeltaQuat);
		CurrentObject = nullptr;
	} 
}

