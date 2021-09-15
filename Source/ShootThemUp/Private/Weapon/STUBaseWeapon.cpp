// Shoot Them Up Game. All Rights Reserved.

#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All)

ASTUBaseWeapon::ASTUBaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);
}

void ASTUBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	check(WeaponMesh);
}

void ASTUBaseWeapon::Fire()
{
	UE_LOG(LogBaseWeapon, Display, TEXT("Fire"));

	MakeShot();
}

void ASTUBaseWeapon::MakeShot()
{
	if (!GetWorld()) return;

	const auto Player = Cast<ACharacter>(GetOwner());
	if (!Player) return;

	const auto Controller = Player->GetController<APlayerController>();
	if (!Controller) return;

	FVector ViewLocation;
	FRotator ViewRotation;
	Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);

	const FTransform SocketTransform = WeaponMesh->GetSocketTransform(MuzzleSocketName);
	const FVector TraceStart = ViewLocation;
	const FVector ShootDirection = ViewRotation.Vector();
	const FVector TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(Player);

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionQueryParams);
	bool bShotDown(false);
	if (HitResult.bBlockingHit)
	{
		const FVector MuzzleDirection = SocketTransform.GetRotation().GetForwardVector();
		const FVector ImpactPointDirection = HitResult.ImpactPoint - SocketTransform.GetLocation();
		const float AngleRad = FMath::Acos(FVector::DotProduct(MuzzleDirection,  ImpactPointDirection) / (MuzzleDirection.Size() * ImpactPointDirection.Size()));
		const float Angle = FMath::RadiansToDegrees(AngleRad);
		UE_LOG(LogBaseWeapon, Display, TEXT("Angle: %f"), Angle);

		if (FMath::Abs(Angle) <= 90.0)
		{
			bShotDown = true;
			DrawDebugLine(GetWorld(), SocketTransform.GetLocation(), HitResult.ImpactPoint, FColor::Red, false, 3.0f);
			DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);
			UE_LOG(LogBaseWeapon, Display, TEXT("Bone: %s"), *HitResult.BoneName.ToString());
		}
	}
	if (!bShotDown)
	{
		DrawDebugLine(GetWorld(), SocketTransform.GetLocation(), TraceEnd, FColor::Blue, false, 3.0f);
	}
}