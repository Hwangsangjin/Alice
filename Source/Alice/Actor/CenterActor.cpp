// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/CenterActor.h"
#include "Kismet/GameplayStatics.h"
#include "Character/AliceCharacter.h"
#include "Character/RabbitCharacter.h"

// Sets default values
ACenterActor::ACenterActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACenterActor::BeginPlay()
{
	Super::BeginPlay();
	
	AliceCharacter = Cast<AAliceCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), AAliceCharacter::StaticClass()));

	/*TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARabbitCharacter::StaticClass(), OutActors);
	for (int32 i = 0; i < OutActors.Num(); i++)
	{
		RabbitCharacters[i] = Cast<ARabbitCharacter>(OutActors[i]);
	}*/
}

// Called every frame
void ACenterActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACenterActor::AddRotation(FRotator DeltaRotation)
{
	if (AliceCharacter)
	{
		AliceCharacter->SetMovementMode(MOVE_None);
	}

	/*for (int32 i = 0; i < RabbitCharacters.Num(); i++)
	{
		RabbitCharacters[i]->SetMovementMode(MOVE_None);
	}*/

	AddActorWorldRotation(DeltaRotation);
}

