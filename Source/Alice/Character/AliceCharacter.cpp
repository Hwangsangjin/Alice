// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AliceCharacter.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Character.h>

// Sets default values
AAliceCharacter::AAliceCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> TempInputMappingContext(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Inputs/IMC_Alice.IMC_Alice'"));
	if (TempInputMappingContext.Succeeded())
	{
		InputMappingContext = TempInputMappingContext.Object;
	}
}

// Called when the game starts or when spawned
void AAliceCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
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
void AAliceCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAliceCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(MoveAxis2D, ETriggerEvent::Triggered, this, &AAliceCharacter::Move);
		EnhancedInputComponent->BindAction(TurnAxis2D, ETriggerEvent::Triggered, this, &AAliceCharacter::Turn);
	}
}

void AAliceCharacter::Move(const FInputActionValue& Value)
{
	// 1.입력값을 받아온다
	FVector2D InputValue = Value.Get<FVector2D>();
	// 2. 컨트롤러가 있다면
	if (IsValid(Controller))
	{
		// 3. 컨트롤러의 회전값을 가져온다
		const FRotator YawRotation(0, Controller->GetControlRotation().
		Yaw, 0);
		// 4. 회전값을 이용해서 앞방향, 오른쪽방향 벡터를 구한다
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// 5. 방향벡터를 이용해서 이동시킨다
		AddMovementInput(ForwardDirection, InputValue.X);
		AddMovementInput(RightDirection, InputValue.Y);
	}

	// 수업시간에 했던 방법
	/*FVector inputDir = FVector(Value.Get<FVector2D>().X, Value.Get<FVector2D>().Y, 0);
	FVector modifiedDir = GetTransform().TransformVector(inputDir);
	AddMovementInput(modifiedDir);*/
}

void AAliceCharacter::Turn(const FInputActionValue& Value)
{	
	// 입력값을 가져와서
	FVector2D InputValue = Value.Get<FVector2D>();

	if (IsValid(Controller))
	{
		// 회전한다
		AddControllerYawInput(InputValue.X);
		AddControllerPitchInput(InputValue.Y);
	}
}

