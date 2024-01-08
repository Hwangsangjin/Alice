// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/WheelComponent.h"
#include "MotionControllerComponent.h"
#include "EnhancedInputComponent.h"
#include "Player/VRPlayer.h"
#include "Actor/CenterActor.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UWheelComponent::UWheelComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UWheelComponent::BeginPlay()
{
	Super::BeginPlay();

	Player = GetOwner<AVRPlayer>();
	CenterActor = Cast<ACenterActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ACenterActor::StaticClass()));
}


// Called every frame
void UWheelComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const FVector PrevLocation = (Player->LeftController->GetComponentLocation() + Player->RightController->GetComponentLocation()) * 0.5;

	if (bPressdLeftGrip && bPressdRightGrip)
	{
		const FVector CurrentLocation = (Player->LeftController->GetComponentLocation() + Player->RightController->GetComponentLocation()) * 0.5;
		const FVector DeltaLocation = PrevLocation - CurrentLocation;
		Player->AddLocation(DeltaLocation);

		const float Angle = GetControllerAngle(Player->LeftController->GetComponentLocation(), Player->RightController->GetComponentLocation(), Player->GetActorRightVector());
		if (CenterActor)
		{
			CenterActor->AddRotation(FRotator(0, Angle, 0));
		}
	}
}

void UWheelComponent::SetupPlayerInputComponent(UEnhancedInputComponent* EnhancedInputComponent, TArray<class UInputAction*> InputActions)
{
	EnhancedInputComponent->BindAction(InputActions[0], ETriggerEvent::Started, this, &UWheelComponent::LeftGripOn);
	EnhancedInputComponent->BindAction(InputActions[0], ETriggerEvent::Completed, this, &UWheelComponent::LeftGripOff);
	EnhancedInputComponent->BindAction(InputActions[3], ETriggerEvent::Started, this, &UWheelComponent::RightGripOn);
	EnhancedInputComponent->BindAction(InputActions[3], ETriggerEvent::Completed, this, &UWheelComponent::RightGripOff);
}

float UWheelComponent::GetControllerAngle(FVector LeftControllerLocation, FVector RightControllerLocation, FVector BaseDirection) const
{
	const FVector TargetVector = (RightControllerLocation - LeftControllerLocation).GetSafeNormal();
	const FVector BaseVector = BaseDirection.GetSafeNormal();

	const float InnerProduct = FVector::DotProduct(TargetVector, BaseVector);
	const float Degree = FMath::RadiansToDegrees(FMath::Acos(InnerProduct));

	const FVector OuterProduct = FVector::CrossProduct(BaseVector, TargetVector);
	const float Sign = OuterProduct.GetSignVector().Z;

	return Degree * Sign * GetWorld()->GetDeltaSeconds();
}

void UWheelComponent::LeftGripOn()
{
	bPressdLeftGrip = true;
}

void UWheelComponent::LeftGripOff()
{
	bPressdLeftGrip = false;
}

void UWheelComponent::RightGripOn()
{
	bPressdRightGrip = true;
}

void UWheelComponent::RightGripOff()
{
	bPressdRightGrip = false;
}

