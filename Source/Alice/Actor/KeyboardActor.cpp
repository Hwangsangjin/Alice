// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/KeyboardActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

// Sets default values
AKeyboardActor::AKeyboardActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxComponent->SetCollisionObjectType(ECC_GameTraceChannel3);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap);
	BoxComponent->SetBoxExtent(FVector(50));
	BoxComponent->SetWorldScale3D(FVector(0.2f, 0.05f, 0.05f));
	SetRootComponent(BoxComponent);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMeshComponent->SetRelativeLocation(FVector(0, 0, -50));
}

// Called when the game starts or when spawned
void AKeyboardActor::BeginPlay()
{
	Super::BeginPlay();
	
	// Box Component에 충돌 이벤트 함수 바인딩하기
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AKeyboardActor::OnHitKey);

	KeyMaterial = UMaterialInstanceDynamic::Create(StaticMeshComponent->GetMaterial(0), this);
	StaticMeshComponent->SetMaterial(0, KeyMaterial);
}

// Called every frame
void AKeyboardActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKeyboardActor::OnHitKey(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), KeySound.GetDefaultObject(), GetActorLocation());
	KeyMaterial->SetVectorParameterValue(FName("EmissiveColor"), FVector4(0.214748, 1.000000, 0.151042, 1.0));
	KeyMaterial->SetScalarParameterValue(FName("Intensity"), 5.0f);

	FTimerHandle ResetHandle;
	GetWorldTimerManager().ClearTimer(ResetHandle);
	GetWorldTimerManager().SetTimer(ResetHandle, this, &AKeyboardActor::RestKeyColor, 0.5f, false);
}

void AKeyboardActor::RestKeyColor()
{
	KeyMaterial->SetVectorParameterValue(FName("EmissiveColor"), FVector4(.000000, 1.000000, 1.000000, 1.0));
	KeyMaterial->SetScalarParameterValue(FName("Intensity"), 1.0f);
}

