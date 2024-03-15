// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerTank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"


APlayerTank::APlayerTank()
{
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm->SetupAttachment(RootComponent);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
}

void APlayerTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APlayerTank::Move);
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APlayerTank::Turn);

    //PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &APlayerTank::Fire);
}

void APlayerTank::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (PlayerTankPlayerController)
    {
        FHitResult HitResult;
        PlayerTankPlayerController->GetHitResultUnderCursor(
            ECollisionChannel::ECC_Visibility,
            false,
            HitResult);

        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint + FVector(0.f,0.f,30.f), 30.f, 12, FColor::Red);

        RotateTurret(HitResult.ImpactPoint);
    }
}

void APlayerTank::HandleDestruction()
{
    Super::HandleDestruction();
    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);
    bAlive = false;
}


void APlayerTank::BeginPlay()
{
    Super::BeginPlay();

    PlayerTankPlayerController = Cast<APlayerController>(GetController());
}


void APlayerTank::Move(float Value)
{
    FVector DeltaLocation = FVector::ZeroVector;
    // X = Value * DeltaTime * Speed
    DeltaLocation.X = Value * Speed * UGameplayStatics::GetWorldDeltaSeconds(this);
    AddActorLocalOffset(DeltaLocation, true);

}

void APlayerTank::Turn(float Value)
{
    FRotator DeltaRotation = FRotator::ZeroRotator;
    // Yaw = Value * DeltaTime * TurnRate
    DeltaRotation.Yaw = Value * TurnRate * UGameplayStatics::GetWorldDeltaSeconds(this);
    AddActorLocalRotation(DeltaRotation, true);
    GetController();
}

