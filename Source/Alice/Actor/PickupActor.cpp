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

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	LocationOffset = FVector(0, 0, 10);
	ThrowPower = 50.0f;
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

void APickupActor::Grabbed(USkeletalMeshComponent* HandMesh, EAttachmentRule AttachmentRule)
{
	// 잡았을 때 핸드 메시에 부착된다.
	BoxComponent->SetSimulatePhysics(false);

	if (AttachmentRule == EAttachmentRule::KeepWorld)
	{
		FAttachmentTransformRules Rule = FAttachmentTransformRules::KeepWorldTransform;
		// 잡는 순간의 간격을(월드 좌표 기준) 유지하면서 붙인다.
		AttachToComponent(HandMesh, Rule);
	}
	else if (AttachmentRule == EAttachmentRule::SnapToTarget)
	{
		FAttachmentTransformRules Rule = FAttachmentTransformRules::SnapToTargetIncludingScale;
		// 지정된 위치로 이동해서 붙인다.
		AttachToComponent(HandMesh, Rule, FName("GrabSocket"));
		SetActorRelativeLocation(GetActorLocation() + LocationOffset);
	}
}

void APickupActor::Released(FVector DeltaPosition, FQuat DeltaRotation)
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	BoxComponent->SetSimulatePhysics(true);

	// 속도와 각속도를 추가해준다.
	if (DeltaPosition.Size() > 0.05f)
	{
		BoxComponent->AddImpulse(DeltaPosition * ThrowPower);
		BoxComponent->AddTorqueInRadians(DeltaRotation.GetRotationAxis() * RotPower);
	}
}

