// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AliceCharacter.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NiagaraComponent.h"

// Sets default values
AAliceCharacter::AAliceCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Dust = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Dust"));
	Dust->SetupAttachment(RootComponent);
	Dust->SetVisibility(false);
}

// Called when the game starts or when spawned
void AAliceCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AAliceCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAliceCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAliceCharacter::SetMovementMode(EMovementMode NewMovementMode)
{
	GetCharacterMovement()->SetMovementMode(NewMovementMode);
}

void AAliceCharacter::Move(const FVector& ForwardDirection, const FVector& RightDirection, const FVector2D& InputValue)
{
	if (GetCharacterMovement()->IsFalling())
	{
		return;
	}

	if (bLadder)
	{
		Dust->SetVisibility(false);
		AddMovementInput(FVector(0, 0, 0));
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		AddMovementInput(GetActorUpVector(), InputValue.X);
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	else
	{
		Dust->SetVisibility(true);
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		AddMovementInput(ForwardDirection, InputValue.X);
		AddMovementInput(RightDirection, InputValue.Y);
		GetCharacterMovement()->bOrientRotationToMovement = true;

		/*FTimerHandle TimerHandle;
		const float DustTime = 1.0f;
		if (Dust->GetVisibleFlag())
		{
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]()
				{
					Dust->SetVisibility(false);

				}), DustTime, false);
		}*/
	}
}

