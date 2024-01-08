// Fill out your copyright notice in the Description page of Project Settings.


#include "VRPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "MotionControllerComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Components/TextRenderComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "Component/MoveComponent.h"
#include "Component/GrabComponent.h"
#include "Component/WheelComponent.h"

// Sets default values
AVRPlayer::AVRPlayer()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Default Scene Root"));
	DefaultSceneRoot->SetupAttachment(RootComponent);

	VROrigin = CreateDefaultSubobject<USceneComponent>(TEXT("VR Origin"));
	VROrigin->SetupAttachment(DefaultSceneRoot);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Follow Camera"));
	FollowCamera->SetupAttachment(VROrigin);

	HMDMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HMD Mesh"));
	HMDMesh->SetupAttachment(FollowCamera);

	LeftController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Left Controller"));
	LeftController->SetupAttachment(VROrigin);
	LeftController->SetTrackingMotionSource(FName("Left"));

	LeftHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Left Hand Mesh"));
	LeftHand->SetupAttachment(LeftController);

	RightController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Right Controller"));
	RightController->SetupAttachment(VROrigin);
	RightController->SetTrackingMotionSource(FName("Right"));

	RightHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Right Hand Mesh"));
	RightHand->SetupAttachment(RightController);

	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	MoveComponent = CreateDefaultSubobject<UMoveComponent>(TEXT("Move Component"));
	GrabComponent = CreateDefaultSubobject<UGrabComponent>(TEXT("Grab Component"));
	WheelComponent = CreateDefaultSubobject<UWheelComponent>(TEXT("Wheel Component"));

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Inputs/IMC_VRPlayer.IMC_VRPlayer'"));
	if (IMC.Succeeded())
	{
		InputMappingContext = IMC.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> LeftGrip(TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_LeftGrip.IA_LeftGrip'"));
	if (LeftGrip.Succeeded())
	{
		InputActions.Add(LeftGrip.Object);
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> LeftTrigger(TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_LeftTrigger.IA_LeftTrigger'"));
	if (LeftTrigger.Succeeded())
	{
		InputActions.Add(LeftTrigger.Object);
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> LeftThumbStick(TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_LeftThumbStick.IA_LeftThumbStick'"));
	if (LeftThumbStick.Succeeded())
	{
		InputActions.Add(LeftThumbStick.Object);
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> RightGrip(TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_RightGrip.IA_RightGrip'"));
	if (RightGrip.Succeeded())
	{
		InputActions.Add(RightGrip.Object);
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> RightTrigger(TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_RightTrigger.IA_RightTrigger'"));
	if (RightTrigger.Succeeded())
	{
		InputActions.Add(RightTrigger.Object);
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> RightThumbStick(TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_RightThumbStick.IA_RightThumbStick'"));
	if (RightThumbStick.Succeeded())
	{
		InputActions.Add(RightThumbStick.Object);
	}
}

// Called when the game starts or when spawned
void AVRPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	// 헤드 마운트 디스플레이 장비의 트레킹(추적) 기준 위치 설정
	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);

	// 입력 매핑 설정
	PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

// Called every frame
void AVRPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVRPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// 기존 입력 데이터를 향상된 입력 데이터 형태로 변환
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		// 바인딩
		MoveComponent->SetupPlayerInputComponent(EnhancedInputComponent, InputActions);
		GrabComponent->SetupPlayerInputComponent(EnhancedInputComponent, InputActions);
		WheelComponent->SetupPlayerInputComponent(EnhancedInputComponent, InputActions);
	}
}

void AVRPlayer::AddLocation(FVector InDeltaLocation)
{
	AddActorWorldOffset(InDeltaLocation);
}

