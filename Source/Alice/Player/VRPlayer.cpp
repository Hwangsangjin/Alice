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
#include "NiagaraComponent.h"
#include "Component/GrabComponent.h"

// Sets default values
AVRPlayer::AVRPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Follow Camera"));
	FollowCamera->SetupAttachment(RootComponent);

	HMDMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HMD Mesh"));
	HMDMesh->SetupAttachment(FollowCamera);

	LeftController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Left Controller"));
	LeftController->SetupAttachment(RootComponent);
	LeftController->SetRelativeLocation(FVector(50, -30, -10));
	LeftController->SetTrackingMotionSource(FName("Left"));

	LeftHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Left Hand Mesh"));
	LeftHand->SetupAttachment(LeftController);
	LeftHand->SetRelativeRotation(FRotator(-90, 0, -180));

	LeftLog = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Left Log"));
	LeftLog->SetupAttachment(LeftController);
	LeftLog->SetRelativeLocation(FVector(-20, 10, 0));
	LeftLog->SetRelativeRotation(FRotator(0, -90, -90));
	LeftLog->SetHorizontalAlignment(EHTA_Center);
	LeftLog->SetVerticalAlignment(EVRTA_TextCenter);
	LeftLog->SetWorldSize(20.0f);
	LeftLog->SetTextRenderColor(FColor(255, 255, 0));

	RightController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Right Controller"));
	RightController->SetupAttachment(RootComponent);
	RightController->SetRelativeLocation(FVector(50, 30, -10));
	RightController->SetTrackingMotionSource(FName("Right"));

	RightHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Right Hand Mesh"));
	RightHand->SetupAttachment(RightController);
	RightHand->SetRelativeRotation(FRotator(90, 45, 0));

	RightLog = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Right Log"));
	RightLog->SetupAttachment(RightController);
	RightLog->SetRelativeLocation(FVector(20, 10, 0));
	RightLog->SetRelativeRotation(FRotator(0, -90, 90));
	RightLog->SetHorizontalAlignment(EHTA_Center);
	RightLog->SetVerticalAlignment(EVRTA_TextCenter);
	RightLog->SetWorldSize(20.0f);
	RightLog->SetTextRenderColor(FColor(255, 255, 0));

	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	MoveComponent = CreateDefaultSubobject<UMoveComponent>(TEXT("Move Component"));
	GrabComponent = CreateDefaultSubobject<UGrabComponent>(TEXT("Grab Component"));

	TeleportVFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Teleport VFX"));
	TeleportVFX->SetupAttachment(LeftHand);

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> TempInputMappingContext(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Inputs/IMC_VRPlayer.IMC_VRPlayer'"));
	if (TempInputMappingContext.Succeeded())
	{
		InputMappingContext = TempInputMappingContext.Object;
	}
}

// Called when the game starts or when spawned
void AVRPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	// 헤드 마운트 디스플레이 장비의 트레킹(추적) 기준 위치를 스테이지로 설정
	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Stage);

	// 입력 매핑 설정
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
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
		/*EnhancedInputComponent->BindAction(RightTriggerTouch, ETriggerEvent::Triggered, this, &AVRPlayer::RightTriggerInput_Touch);
		EnhancedInputComponent->BindAction(RightTriggerTouch, ETriggerEvent::Completed, this, &AVRPlayer::RightTriggerInput_Touch);
		EnhancedInputComponent->BindAction(RightTriggerBool, ETriggerEvent::Triggered, this, &AVRPlayer::RightTriggerInput_Bool);
		EnhancedInputComponent->BindAction(RightTriggerBool, ETriggerEvent::Completed, this, &AVRPlayer::RightTriggerInput_Bool);
		EnhancedInputComponent->BindAction(RightTriggerFloat, ETriggerEvent::Triggered, this, &AVRPlayer::RightTriggerInput_Float);
		EnhancedInputComponent->BindAction(RightTriggerFloat, ETriggerEvent::Completed, this, &AVRPlayer::RightTriggerInput_Float);
		EnhancedInputComponent->BindAction(RightThumbStick, ETriggerEvent::Triggered, this, &AVRPlayer::RightThumbStickInput);
		EnhancedInputComponent->BindAction(RightThumbStick, ETriggerEvent::Completed, this, &AVRPlayer::RightThumbStickInput);*/

		EnhancedInputComponent->BindAction(MoveAxis2D, ETriggerEvent::Triggered, this, &AVRPlayer::Move);
		EnhancedInputComponent->BindAction(TurnAxis2D, ETriggerEvent::Triggered, this, &AVRPlayer::Turn);

		MoveComponent->SetupPlayerInputComponent(EnhancedInputComponent, InputActions);
		GrabComponent->SetupPlayerInputComponent(EnhancedInputComponent, InputActions);
	}
}

void AVRPlayer::RightTriggerInput_Bool(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		RightLog->SetText(FText::FromString(FString("RightTrigger Pressed!")));
	}
	else
	{
		RightLog->SetText(FText::FromString(FString("RightTrigger Pressed!")));
	}
}

void AVRPlayer::RightTriggerInput_Float(const FInputActionValue& Value)
{
	RightLog->SetText(FText::FromString(FString::Printf(TEXT("RightTrigger: %.2f"), Value.Get<float>())));

	Teleport(500.0f, RightController->GetForwardVector(), RightController->GetComponentLocation());
}

void AVRPlayer::RightTriggerInput_Touch(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		RightLog->SetText(FText::FromString(FString("RightTrigger Touched!")));
	}
	else
	{
		RightLog->SetText(FText::FromString(FString("RightTrigger Untouched!")));
	}
}

void AVRPlayer::RightThumbStickInput(const FInputActionValue& Value)
{
	FVector2D InputValue = Value.Get<FVector2D>();
	RightLog->SetText(FText::FromString(FString::Printf(TEXT("X: %.2f\r\nY: %.2f"), InputValue.X, InputValue.Y)));
}

void AVRPlayer::Move(const FInputActionValue& Value)
{
	FVector2D InputValue = Value.Get<FVector2D>();

	if (IsValid(Controller))
	{
		const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, InputValue.X);
		AddMovementInput(RightDirection, InputValue.Y);
	}
}

void AVRPlayer::Turn(const FInputActionValue& Value)
{
	FVector2D InputValue = Value.Get<FVector2D>();

	if (IsValid(Controller))
	{
		AddControllerYawInput(InputValue.X);
	}
}

void AVRPlayer::Teleport(float SightRange, FVector Direction, FVector Pivot)
{
	FHitResult HitResult;
	FVector Start = Pivot;
	FVector End = Pivot + Direction * SightRange;
	bool bIsHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);
	if (bIsHit)
	{
		//SetActorLocation(HitResult.ImpactPoint + FVector(0, 0, GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));

		DrawDebugLine(GetWorld(), Start, HitResult.ImpactPoint, FColor::Red);
	}
	else
	{
		DrawDebugLine(GetWorld(), Start, End, FColor::Red);
	}
}

