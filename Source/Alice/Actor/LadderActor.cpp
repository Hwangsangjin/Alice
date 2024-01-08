// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/LadderActor.h"

#include "Character/AliceCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ALadderActor::ALadderActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(BoxComponent);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	MeshComponent->SetupAttachment(BoxComponent);
}

// Called when the game starts or when spawned
void ALadderActor::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ALadderActor::OnLadderOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ALadderActor::OnLadderEndOverlap);
}

// Called every frame
void ALadderActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALadderActor::OnLadderOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AAliceCharacter* Alice = Cast<AAliceCharacter>(OtherActor);
	if (Alice != nullptr)
	{
		Alice->bLadder = true;
	}
}

void ALadderActor::OnLadderEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AAliceCharacter* Alice = Cast<AAliceCharacter>(OtherActor);
	if (Alice != nullptr)
	{
		Alice->bLadder = false;
		Alice->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
}

