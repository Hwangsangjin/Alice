// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RabbitCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ARabbitCharacter::ARabbitCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARabbitCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARabbitCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARabbitCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ARabbitCharacter::SetMovementMode(EMovementMode NewMovementMode)
{
	GetCharacterMovement()->SetMovementMode(NewMovementMode);
}

