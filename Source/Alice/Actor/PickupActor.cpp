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
	// ����� �� �ڵ� �޽ÿ� �����ȴ�.
	BoxComponent->SetSimulatePhysics(false);

	if (AttachmentRule == EAttachmentRule::KeepWorld)
	{
		FAttachmentTransformRules Rule = FAttachmentTransformRules::KeepWorldTransform;
		// ��� ������ ������(���� ��ǥ ����) �����ϸ鼭 ���δ�.
		AttachToComponent(HandMesh, Rule);
	}
	else if (AttachmentRule == EAttachmentRule::SnapToTarget)
	{
		FAttachmentTransformRules Rule = FAttachmentTransformRules::SnapToTargetIncludingScale;
		// ������ ��ġ�� �̵��ؼ� ���δ�.
		AttachToComponent(HandMesh, Rule, FName("GrabSocket"));
		SetActorRelativeLocation(GetActorLocation() + LocationOffset);
	}
}

void APickupActor::Released(FVector DeltaPosition, FQuat DeltaRotation)
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	BoxComponent->SetSimulatePhysics(true);

	// �ӵ��� ���ӵ��� �߰����ش�.
	if (DeltaPosition.Size() > 0.05f)
	{
		BoxComponent->AddImpulse(DeltaPosition * ThrowPower);
		BoxComponent->AddTorqueInRadians(DeltaRotation.GetRotationAxis() * RotPower);
	}
}

