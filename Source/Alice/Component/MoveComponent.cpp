// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveComponent.h"
#include "Player/VRPlayer.h"
#include "EnhancedInputComponent.h"
#include "MotionControllerComponent.h"
#include "Components/TextRenderComponent.h"
#include "components/CapsuleComponent.h"
#include "FunctionLibrary/VRDrawFunctionLibrary.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"
#include "Actor/TeleportRingActor.h"
#include "NiagaraComponent.h"

// Sets default values for this component's properties
UMoveComponent::UMoveComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMoveComponent::BeginPlay()
{
	Super::BeginPlay();

	Player = GetOwner<AVRPlayer>();

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	TeleportRingInst = GetWorld()->SpawnActor<ATeleportRingActor>(TeleportRingBP, FVector::ZeroVector, FRotator::ZeroRotator, Params);
	if (TeleportRingInst)
	{
		TeleportRingInst->RingVFX->SetVisibility(false);
	}
}


// Called every frame
void UMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMoveComponent::SetupPlayerInputComponent(UEnhancedInputComponent* EnhancedInputComponent, TArray<class UInputAction*> Inputs)
{
	EnhancedInputComponent->BindAction(Inputs[0], ETriggerEvent::Triggered, this, &UMoveComponent::ShowLine);
	EnhancedInputComponent->BindAction(Inputs[0], ETriggerEvent::Completed, this, &UMoveComponent::Teleport);
}

void UMoveComponent::ShowLine(const FInputActionValue& Value)
{
	bool bIsPressed = Value.Get<bool>();
	Player->LeftLog->SetText(FText::FromString(FString::Printf(TEXT("%s"), bIsPressed ? *FString("Pressed!") : *FString("Released..."))));

	if (bIsPressed && Player)
	{
#pragma region 중력 가속도를 이용한 방식
		DrawTrajectory(Player->LeftController->GetComponentLocation(), Player->LeftHand->GetForwardVector() * (-1) + Player->LeftHand->GetUpVector(), LineSpeed, 50.0f, 0.1f);
#pragma endregion

#pragma region 베지어 곡선식을 이용한 방식
		//DrawTrajectoryBezier(Player->LeftHand->GetComponentLocation(), Player->LeftHand->GetForwardVector() + Player->LeftHand->GetRightVector(), 50);
#pragma endregion
	}
}

void UMoveComponent::DrawTrajectory(FVector Start, FVector Direction, int32 Segment, float Speed, float Interval)
{
	TArray<FVector> LinePositions;
	UE_LOG(LogTemp, Warning, TEXT("X: %f, Y: %f, Z: %f"), Direction.X, Direction.Y, Direction.Z);

	for (int32 i = 0; i < Segment; i++)
	{
		// Direction 방향으로 Segment 회만큼 Interval 간격(초)으로 반복해서 이동했을 위치를 계산한다.
		float ElapsedTime = Interval * i;
		FVector GravityZ = FVector(0, 0, GetWorld()->GetDefaultGravityZ());
		FVector NewLocation = Start + Direction * Speed * ElapsedTime - (0.5f * GravityZ * ElapsedTime * ElapsedTime);

		FHitResult HitResult;
		if (i > 0 && GetWorld()->LineTraceSingleByChannel(HitResult, LinePositions[i - 1], NewLocation, ECC_Visibility))
		{
			Player->LeftLog->SetText(FText::FromString(HitResult.GetActor()->GetActorNameOrLabel()));
			LinePositions.Add(HitResult.ImpactPoint);
			break;
		}

		// 계산 결과 값들은 배열 변수에 담아놓는다.
		LinePositions.Add(NewLocation);
	}

	// 계산된 위치들을 선으로 연결해서 그린다.
	/*for (int32 i = 0; i < LinePositions.Num() - 1; i++)
	{
		DrawDebugLine(GetWorld(), LinePositions[i], LinePositions[i + 1], FColor::Green, false, 0, 0, 2.0f);
	}*/

	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(Player->TeleportVFX, FName("PointArray"), LinePositions);

	// 마지막 위치에 빨간 상자를 표시한다.
	TargetLocation = LinePositions[LinePositions.Num() - 1];
	//DrawDebugSolidBox(GetWorld(), LinePositions[LinePositions.Num()] - 1, FVector(5), FColor::Red);
	if (TeleportRingInst)
	{
		TeleportRingInst->SetActorLocation(TargetLocation);
		TeleportRingInst->RingVFX->SetVisibility(true);
	}
}

void UMoveComponent::DrawTrajectoryBezier(FVector Start, FVector Direction, int32 Segment)
{
	FHitResult HitResult;
	FVector End = Start + Direction * 1000.0;
	FVector ImpactPoint;

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility))
	{
		ImpactPoint = HitResult.ImpactPoint;
		FVector Centor = FVector((ImpactPoint.X + Start.X) * 0.5f, (ImpactPoint.Y + Start.Y) * 0.5f, Start.Z);
		TArray<FVector> Results = UVRDrawFunctionLibrary::CalculateBezierCurve(Start, Centor, ImpactPoint, Segment);
		if (Results.Num() > 0)
		{
			for (int32 i = 0; i < Results.Num() - 1; i++)
			{
				DrawDebugLine(GetWorld(), Results[i], Results[i + 1], FColor::Green, 0, 0, 2.0f);
			}
		}
	}
}

void UMoveComponent::Teleport()
{
	// Black Fade In 효과를 준다.
	Player->GetController<APlayerController>()->PlayerCameraManager->StartCameraFade(0, 1.0f, TeleportDelay, FLinearColor::Black);

	if (!TargetLocation.IsNearlyZero())
	{
		FTimerHandle TeleportTime;

		GetWorld()->GetTimerManager().SetTimer(TeleportTime, FTimerDelegate::CreateLambda([&]()
			{
				// 1. 플레이어 이동
				Player->SetActorLocation(TargetLocation + FVector(0, 0, Player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
				// 2. 링 이펙트 표시 끄기
				TeleportRingInst->RingVFX->SetVisibility(false);
				// 3. 라인 이펙트의 배열 값 초기화
				TArray<FVector> ResetVectors = { FVector::ZeroVector, FVector::ZeroVector };
				UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(Player->TeleportVFX, FName("PointArray"), ResetVectors);
			}), TeleportDelay, false);
	}
}

