// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/AliceAnimInstance.h"
#include "../Character/AliceCharacter.h"

void UAliceAnimInstance::NativeInitializeAnimation()
{
	APawn* Pawn = TryGetPawnOwner();
	Alice = Cast<AAliceCharacter>(Pawn);
}
                                                                                                                                                    
void UAliceAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Alice)
	{
		FVector Velocity = Alice->GetVelocity();
		FVector ForwardVector = Alice->GetActorForwardVector();
		Speed = FVector::DotProduct(ForwardVector, Velocity);

		FVector RightVector = Alice->GetActorRightVector();
		Direction = FVector::DotProduct(RightVector, Velocity); 
	}
}
