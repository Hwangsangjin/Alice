// Fill out your copyright notice in the Description page of Project Settings.


#include "BeziercurveActor.h"

// Sets default values
ABeziercurveActor::ABeziercurveActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	p0 = CreateDefaultSubobject<USceneComponent>(TEXT("P0"));
	p0->SetupAttachment(RootComponent);
	p1 = CreateDefaultSubobject<USceneComponent>(TEXT("P1"));
	p1->SetupAttachment(RootComponent);
	p2 = CreateDefaultSubobject<USceneComponent>(TEXT("P2"));
	p2->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABeziercurveActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABeziercurveActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawBezierCurve(p0->GetComponentLocation(), p1->GetComponentLocation(), p2->GetComponentLocation());
}

void ABeziercurveActor::DrawBezierCurve(FVector Location0, FVector Location1, FVector Location2)
{
	TArray<FVector> LinePositions;
	FVector m0;
	FVector m1;
	FVector b;

	for (float i = 0; i < 1.0f; i += 0.05f)
	{
		m0 = FMath::Lerp(Location0, Location1, i);
		m1 = FMath::Lerp(Location1, Location2, i);
		b = FMath::Lerp(m0, m1, i);

		LinePositions.Add(b);
	}

	UWorld* World = GetWorld();
	DrawDebugLine(World, Location0, Location1, FColor::Black, false, 0, 0, 2.0f);
	DrawDebugLine(World, Location1, Location2, FColor::Black, false, 0, 0, 2.0f);
	DrawDebugPoint(World, m0, 10, FColor::White, false, 0, 0);
	DrawDebugPoint(World, m1, 10, FColor::White, false, 0, 0);
	DrawDebugPoint(World, b, 10, FColor::Purple, false, 0, 0);

	if (LinePositions.Num() > 0)
	{
		for (int32 i = 0; i < LinePositions.Num() - 1; i++)
		{
			DrawDebugLine(World, LinePositions[i], LinePositions[i + 1], FColor::Magenta, false, 0, 0, 1);
		}
	}
}

