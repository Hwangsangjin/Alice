// Fill out your copyright notice in the Description page of Project Settings.


#include "VRDrawFunctionLibrary.h"

TArray<FVector> UVRDrawFunctionLibrary::CalculateBezierCurve(FVector p0, FVector p1, FVector p2, int32 Segment)
{
	TArray<FVector> LinePositions;

	float Interval = 1.0f / static_cast<float>(Segment);

	for (float i = 0; i < 1.0f; i += Interval)
	{
		FVector m0 = FMath::Lerp(p0, p1, i);
		FVector m1 = FMath::Lerp(p1, p2, i);
		FVector b = FMath::Lerp(m0, m1, i);

		LinePositions.Add(b);
	}

	return LinePositions;
}
