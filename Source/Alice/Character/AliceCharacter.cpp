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
	// 1.�Է°��� �޾ƿ´�
	FVector2D InputValue = Value.Get<FVector2D>();
	// 2. ��Ʈ�ѷ��� �ִٸ�
	if (IsValid(Controller))
	{
		// 3. ��Ʈ�ѷ��� ȸ������ �����´�
		const FRotator YawRotation(0, Controller->GetControlRotation().
		Yaw, 0);
		// 4. ȸ������ �̿��ؼ� �չ���, �����ʹ��� ���͸� ���Ѵ�
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// 5. ���⺤�͸� �̿��ؼ� �̵���Ų��
		AddMovementInput(ForwardDirection, InputValue.X);
		AddMovementInput(RightDirection, InputValue.Y);
	}

	// �����ð��� �ߴ� ���
	/*FVector inputDir = FVector(Value.Get<FVector2D>().X, Value.Get<FVector2D>().Y, 0);
	FVector modifiedDir = GetTransform().TransformVector(inputDir);
	AddMovementInput(modifiedDir);*/
}

void AAliceCharacter::Turn(const FInputActionValue& Value)
{	
	// �Է°��� �����ͼ�
	FVector2D InputValue = Value.Get<FVector2D>();

	if (IsValid(Controller))
	{
		// ȸ���Ѵ�
		AddControllerYawInput(InputValue.X);
		AddControllerPitchInput(InputValue.Y);
	}
}

