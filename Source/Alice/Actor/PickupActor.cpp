// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PickupActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
APickupActor::APickupActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(BoxComponent);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxComponent->SetCollisionProfileName(FName("Pickup"));
	BoxComponent->SetSimulatePhysics(true);
	BoxComponent->SetEnableGravity(true);
	BoxComponent->SetWorldScale3D(FVector(0.1f, 0.1f, 0.3f));
	BoxComponent->SetBoxExtent(FVector(50));

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetRelativeLocation(FVector(0, 0, -50.0f));
}

// Called when the game starts or when spawned
void APickupActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickupActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickupActor::Grabbed(USkeletalMeshComponent* HandMesh)
{
	BoxComponent->SetSimulatePhysics(false);

	FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules::KeepWorldTransform;
	AttachToComponent(HandMesh, AttachmentRules);
}

