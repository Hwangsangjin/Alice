// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PickupActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Actor/CenterActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APickupActor::APickupActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxComponent->SetCollisionProfileName(FName("Pickup"));
	BoxComponent->SetSimulatePhysics(true);
	BoxComponent->SetEnableGravity(true);
	SetRootComponent(BoxComponent);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APickupActor::BeginPlay()
{
	Super::BeginPlay();
	
	CenterActor = Cast<ACenterActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ACenterActor::StaticClass()));
}

// Called every frame
void APickupActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickupActor::Grabbed(USkeletalMeshComponent* HandMesh, EAttachmentRule AttachmentRule)
{
	BoxComponent->SetSimulatePhysics(false);

	if (AttachmentRule == EAttachmentRule::KeepWorld)
	{
		FAttachmentTransformRules Rule = FAttachmentTransformRules::KeepWorldTransform;
		AttachToComponent(HandMesh, Rule);
	}
	else if (AttachmentRule == EAttachmentRule::SnapToTarget)
	{
		FAttachmentTransformRules Rule = FAttachmentTransformRules::SnapToTargetIncludingScale;
		AttachToComponent(HandMesh, Rule, FName("GrabSocket"));
	}
}

void APickupActor::Released(FVector DeltaPosition, FQuat DeltaRotation)
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]() { AttachToActor(CenterActor, FAttachmentTransformRules::KeepWorldTransform); }), 1.0f, false);

	BoxComponent->SetSimulatePhysics(true);
}

