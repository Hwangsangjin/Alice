// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/GrabComponent.h"
#include "Player/VRPlayer.h"
#include "EnhancedInputComponent.h"
#include "Components/TextRenderComponent.h"
#include "Actor/PickupActor.h"

//#define LINE_TRACE
//#define SPHERE_TRACE
#define OVERLAP_SPHERE

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

		DeltaQuat = Player->RightHand->GetComponentQuat() * PrevQuat.Inverse();
		DeltaQuat = Player->RightHand->GetComponentQuat() - PrevQuat;
		PrevQuat = Player->RightHand->GetComponentQuat();
	}
}

void UGrabComponent::SetupPlayerInputComponent(UEnhancedInputComponent* EnhancedInputComponent, TArray<class UInputAction*> InputActions)
{
	EnhancedInputComponent->BindAction(InputActions[1], ETriggerEvent::Started, this, &UGrabComponent::GrabObject);
	EnhancedInputComponent->BindAction(InputActions[1], ETriggerEvent::Completed, this, &UGrabComponent::ReleaseObject);
}

void UGrabComponent::GrabObject()
{
	//Player->RightLog->SetText(FText::FromString(FString("Try Grabbed!")));

	if (CurrentObject)
	{
		return;
	}

	UWorld* World = GetWorld();

#pragma region 라인 트레이스 활용
#ifdef LINE_TRACE
	if (World)
	{
		FHitResult HitResult;
		FVector Start = Player->RightHand->GetComponentLocation();
		FVector End = Start + Player->RightHand->GetRightVector() * 30.0f;

		if (World->LineTraceSingleByProfile(HitResult, Start, End, FName("Pickup")))
		{
			CurrentObject = Cast<APickupActor>(HitResult.GetActor());
			if (CurrentObject)
			{
				CurrentObject->Grabbed(Player->RightHand, EAttachmentRule::SnapToTarget);
			}
		}
	}
#endif
#pragma endregion

#pragma region 스피어 트레이스 활용
#ifdef SPHERE_TRACE

#endif
#pragma endregion

#pragma region 오버랩 스피어 활용
#ifdef OVERLAP_SPHERE
	TArray<FOverlapResult> OverlapResults;
	if (World->OverlapMultiByProfile(OverlapResults, Player->RightHand->GetComponentLocation(), Player->RightHand->GetComponentQuat(), FName("Pickup"), FCollisionShape::MakeSphere(50.0f)))
	{
		FString String;
		float MinDistance = 25.0f;
		int32 Index = 0;

		for (int32 i = 0; i < OverlapResults.Num(); i++)
		{
			if (OverlapResults[i].GetActor()->IsA<APickupActor>())
			{
				String.Append(OverlapResults[i].GetActor()->GetActorNameOrLabel() + "\r\n");
				float CurrentDistance = FVector::Distance(Player->RightHand->GetComponentLocation(), OverlapResults[i].GetActor()->GetActorLocation());
				if (CurrentDistance < MinDistance)
				{
					MinDistance = CurrentDistance;
					Index = 1;
				}
			}
		}

		CurrentObject = Cast<APickupActor>(OverlapResults[Index].GetActor());
		if (CurrentObject)
		{
			CurrentObject->Grabbed(Player->RightHand, EAttachmentRule::KeepWorld);
			PrevLocation = Player->RightHand->GetComponentLocation();
			Player->RightLog->SetText(FText::FromString(String));
		}
	}
	else
	{
		Player->RightLog->SetText(FText::FromString(TEXT("Not sensing...")));
	}

	//DrawDebugSphere(World, Player->RightHand->GetComponentLocation(), 15.0f, 30, FColor::Magenta, false, 3.0f, 0, 1.0f);
#endif
#pragma endregion
}

void UGrabComponent::ReleaseObject()
{
	if (CurrentObject)
	{
		CurrentObject->Released(DeltaLocation, DeltaQuat);
		CurrentObject = nullptr;
	} 
}

