// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/MoveComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Player/VRPlayer.h"
#include "Character/AliceCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UMoveComponent::UMoveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMoveComponent::BeginPlay()
{
	Super::BeginPlay();

	Player = GetOwner<AVRPlayer>();
	AliceCharacter = Cast<AAliceCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), AAliceCharacter::StaticClass()));
}


// Called every frame
void UMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMoveComponent::SetupPlayerInputComponent(UEnhancedInputComponent* EnhancedInputComponent, TArray<class UInputAction*> InputActions)
{
	EnhancedInputComponent->BindAction(InputActions[2], ETriggerEvent::Triggered, this, &UMoveComponent::Move);
}

void UMoveComponent::Move(const FInputActionValue& Value)
{
	FVector2D InputValue = Value.Get<FVector2D>();

	if (IsValid(Player->Controller))
	{
		const FRotator YawRotation(0, Player->Controller->GetControlRotation().Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AliceCharacter->Move(ForwardDirection, RightDirection, InputValue);
	}
}

