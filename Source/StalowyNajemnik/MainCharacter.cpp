// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "Projectile.h"
#include "Weapon.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Wylaczenie rotacji postaci w strone kamery
	bUseControllerRotationRoll = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = NormalMaxMS;
	GetCharacterMovement()->JumpZVelocity = 500.0f;
	GetCharacterMovement()->AirControl = 0.5f;

	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	CameraSpringArm->SetupAttachment(RootComponent);
	CameraSpringArm->TargetArmLength = 300.0f;
	CameraSpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraSpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));

}

void AMainCharacter::OnProjectileHit_Implementation(AProjectile* Projectile)
{
	HealthComponent->TakeDamage(Projectile->GetParticleDamage());

	if (HealthComponent->IsDead())
	{
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentCameraArmLocation = LocomotionCameraArmLocation;
	CurrentCameraArmLenght = LocomotionCameraArmLenght;

	Weapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);
	Weapon->SetActorRelativeScale3D(FVector(0.01f, 0.01f, 0.01f));
	Weapon->SetOwner(this);
	AttachWeaponToSocket(LegSocketName, LegSocketTransformOffset, LegSocketRotationOffset);
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(HealthComponent->IsDead())
	{
		return;
	}

	if (bIsAiming && GetCharacterMovement()->IsFalling())
	{
		StopAim();
	}

	if (bIsAiming && !GetCharacterMovement()->IsFalling())
	{
		FRotator CurrentRotation = GetActorRotation();
		FRotator ControllerRotation = Controller->GetControlRotation();
		FRotator TargetRotation = FRotator(0.0f, ControllerRotation.Yaw, 0.0f);
		SetActorRotation(FMath::RInterpConstantTo(CurrentRotation, TargetRotation, DeltaTime, 1000.0f));

		CurrentCameraArmLocation = FMath::VInterpConstantTo(CurrentCameraArmLocation, AimCameraArmLocation, DeltaTime, TransitionSpeed);
		CurrentCameraArmLenght = FMath::FInterpConstantTo(CurrentCameraArmLenght, AimCameraArmLenght, DeltaTime, TransitionSpeed);
	}
	else
	{
		CurrentCameraArmLocation = FMath::VInterpConstantTo(CurrentCameraArmLocation, LocomotionCameraArmLocation, DeltaTime, TransitionSpeed);
		CurrentCameraArmLenght = FMath::FInterpConstantTo(CurrentCameraArmLenght, LocomotionCameraArmLenght, DeltaTime, TransitionSpeed);
	}

	CameraSpringArm->SetRelativeLocation(CurrentCameraArmLocation);
	CameraSpringArm->TargetArmLength = CurrentCameraArmLenght;

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMainCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMainCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction(TEXT("Aim"), EInputEvent::IE_Pressed, this, &AMainCharacter::Aim);
	PlayerInputComponent->BindAction(TEXT("Aim"), EInputEvent::IE_Released, this, &AMainCharacter::StopAim);
	PlayerInputComponent->BindAction(TEXT("SwitchEnergyUp"), EInputEvent::IE_Pressed, this, &AMainCharacter::SwitchEnergyUp);
	PlayerInputComponent->BindAction(TEXT("SwitchEnergyDown"), EInputEvent::IE_Pressed, this, &AMainCharacter::SwitchEnergyDown);

	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AMainCharacter::Shoot);

}

void AMainCharacter::AIShoot()
{
	Weapon->TriggerWeapon();
}

void AMainCharacter::MoveForward(float AxisValue)
{
	if(HealthComponent->IsDead())
	{
		return;
	}
	FVector Direction = FRotationMatrix(GetControllerYawRotation()).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, AxisValue);
}

void AMainCharacter::MoveRight(float AxisValue)
{
	if(HealthComponent->IsDead())
	{
		return;
	}
	FVector Direction = FRotationMatrix(GetControllerYawRotation()).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, AxisValue);
}

void AMainCharacter::Jump()
{
	if (bIsAiming || GetCharacterMovement()->IsFalling()) { return; }
	bIsJumping = true;
	ACharacter::Jump();
}

void AMainCharacter::Aim()
{
	SwitchAim(true, false, AimMaxMS);
	AttachWeaponToSocket(AimSocketName, AimSocketTransformOffset, AimSocketRotationOffset);
}

void AMainCharacter::Shoot()
{
	if (!bIsAiming) { return; }
	Weapon->TriggerWeapon();
}

void AMainCharacter::StopAim()
{
	SwitchAim(false, true, NormalMaxMS);
	AttachWeaponToSocket(LegSocketName, LegSocketTransformOffset, LegSocketRotationOffset);
}

void AMainCharacter::SwitchEnergyUp()
{
	Weapon->SwitchCurrentEnergyType(1);
}

void AMainCharacter::SwitchEnergyDown()
{
	Weapon->SwitchCurrentEnergyType(-1);
}

void AMainCharacter::SwitchAim(bool Aim, bool RotationToMovement, float WalkSpeed)
{
	bIsAiming = Aim;
	GetCharacterMovement()->bOrientRotationToMovement = RotationToMovement;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AMainCharacter::AttachWeaponToSocket(FString SocketName, FVector TransformOffset, FRotator RotationOffset)
{
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, *SocketName);
	Weapon->SetActorRelativeLocation(TransformOffset);
	Weapon->SetActorRelativeRotation(RotationOffset);
}

FRotator AMainCharacter::GetControllerYawRotation()
{
	FRotator Rotation = Controller->GetControlRotation();
	return FRotator(0.0f, Rotation.Yaw, 0.0f);
}



