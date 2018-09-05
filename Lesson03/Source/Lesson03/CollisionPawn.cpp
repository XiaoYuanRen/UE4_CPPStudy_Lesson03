// Fill out your copyright notice in the Description page of Project Settings.

#include "CollisionPawn.h"
#include "RunTime/Engine/Classes/Engine/Engine.h"
#include "RunTime/Engine/Classes/Components/SphereComponent.h"
#include "RunTime/Engine/Classes/Components/StaticMeshComponent.h"
#include "RunTime/Engine/Classes/Camera/CameraComponent.h"
#include "RunTime/Engine/Classes/GameFramework/SpringArmComponent.h"
#include "RunTime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "RunTime/Engine/Classes/Particles/ParticleSystem.h"

// Sets default values
ACollisionPawn::ACollisionPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionPawn"));
	SphereComponent->SetSphereRadius(40.0f);
	RootComponent = SphereComponent;
	SphereComponent->SetCollisionProfileName(TEXT("Pawn"));

	UStaticMeshComponent* VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	VisualMesh->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere"));
	if (SphereVisualAsset.Succeeded())
	{
		VisualMesh->SetStaticMesh(SphereVisualAsset.Object);
		VisualMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f));
		VisualMesh->SetWorldScale3D(FVector(0.8f));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Load static mesh failure ...."))
	}

	OurParitcleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	OurParitcleSystem->SetupAttachment(SphereComponent);
	OurParitcleSystem->bAutoActivate = false;
	OurParitcleSystem->SetRelativeLocation(FVector(-20.0f, 0.0f, 20.0f));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Game/StarterContent/Particles/P_Fire"));
	if (ParticleAsset.Succeeded())
	{
		OurParitcleSystem->SetTemplate(ParticleAsset.Object);
	}

	USpringArmComponent* SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraAttachmentArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 3.0f;

	UCameraComponent* CurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ActualCamera"));
	CurCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	OurMovementComponent = CreateDefaultSubobject<UCollisionPawnMovementComponent>(TEXT("MovementComponent"));
	OurMovementComponent->SetUpdatedComponent(RootComponent);


}

// Called when the game starts or when spawned
void ACollisionPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACollisionPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

// Called to bind functionality to input
void ACollisionPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("ParticleToggle", IE_Pressed, this, &ACollisionPawn::ToggelFire);
	PlayerInputComponent->BindAxis("MoveForward", this, &ACollisionPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACollisionPawn::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ACollisionPawn::Turn);
}

UPawnMovementComponent * ACollisionPawn::GetMovementComponent() const
{
	return OurMovementComponent;
}

void ACollisionPawn::MoveForward(float AxisValue)
{
	if (OurMovementComponent && (OurMovementComponent->UpdatedComponent == RootComponent))
	{
		OurMovementComponent->AddInputVector(GetActorForwardVector() * AxisValue);
	}
}

void ACollisionPawn::MoveRight(float AxisValue)
{
	if (OurMovementComponent && (OurMovementComponent->UpdatedComponent == RootComponent))
	{
		OurMovementComponent->AddInputVector(GetActorRightVector()*AxisValue);
	}
}

void ACollisionPawn::Turn(float AxisValue)
{
	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw += AxisValue;
	SetActorRotation(NewRotation);
}

bool IsActive = false;
void ACollisionPawn::ToggelFire()
{
	if (OurParitcleSystem && OurParitcleSystem->Template)
	{
		if (IsActive)
		{
			OurParitcleSystem->Deactivate();
		}
		else
		{
			OurParitcleSystem->Activate();
		}
		IsActive = !IsActive;
	}
}

