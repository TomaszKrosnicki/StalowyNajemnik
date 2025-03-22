// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "MainCharacter.generated.h"

class AWeapon;

UCLASS()
class STALOWYNAJEMNIK_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraSpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Aim", meta = (AllowPrivateAccess = "true"))
	USceneComponent* AimLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bIsAiming = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bIsJumping = false;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float TransitionSpeed = 1000.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Camera|Aim Camera")
	FVector AimCameraArmLocation = FVector(0.0f, 80.0f, 40.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Camera|Aim Camera")
	float AimCameraArmLenght = 200.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Camera|Locomotion Camera")
	FVector LocomotionCameraArmLocation = FVector(0.0f, 0.0f, 40.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Camera|Locomotion Camera")
	float LocomotionCameraArmLenght = 400.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement|Normal Max Speed")
	float NormalMaxMS = 500.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement|Aim Max Speed")
	float AimMaxMS = 200.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AWeapon> WeaponClass;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FVector LegSocketTransformOffset;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FRotator LegSocketRotationOffset;
	UPROPERTY()
	AWeapon* Weapon;

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void Jump();
	void Aim();
	void Shoot();
	void StopAim();
	void SwitchAim(bool Aim, bool RotationToMovement, float WalkSpeed);
	FRotator GetControllerYawRotation();
	FVector CurrentCameraArmLocation;
	float CurrentCameraArmLenght;

};
