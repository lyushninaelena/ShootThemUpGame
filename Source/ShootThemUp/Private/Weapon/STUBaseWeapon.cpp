// Shoot Them Up Game. All Rights Reserved.

#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "STUBaseCharacter.h"
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

void ASTUBaseWeapon::StartFire()
{
	MakeShot();
	GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASTUBaseWeapon::MakeShot, TimeBetweenShots, true);
}

void ASTUBaseWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(ShotTimerHandle);
}

void ASTUBaseWeapon::MakeShot()
{
	if (!GetWorld()) return;
	
	FVector TraceStart, TraceEnd;
	if (!GetTraceData(TraceStart, TraceEnd)) return;

	FVector MuzzleLocation, MuzzleDirection;
	GetMuzzleData(MuzzleLocation, MuzzleDirection);

	FHitResult HitResult;
	if (!MakeHit(HitResult, TraceStart, TraceEnd)) return;

	if (HitResult.bBlockingHit && IsForwardShot(HitResult.ImpactPoint, MuzzleLocation, MuzzleDirection))
	{
		MakeDamage(HitResult);
		
		DrawDebugLine(GetWorld(), MuzzleLocation, HitResult.ImpactPoint, FColor::Red, false, 3.0f);
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);
	}
	else
	{
		DrawDebugLine(GetWorld(), MuzzleLocation, TraceEnd, FColor::Blue, false, 3.0f);
	}
}

APlayerController* ASTUBaseWeapon::GetPlayerController() const
{
	const auto Player = Cast<ACharacter>(GetOwner());
	if (!Player) return nullptr;

	return Player->GetController<APlayerController>();
}

bool ASTUBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
	APlayerController* Controller = GetPlayerController();
	if (!Controller) return false;

	Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
	return true;
}

void ASTUBaseWeapon::GetMuzzleData(FVector& MuzzleLocation, FVector& MuzzleDirection) const
{
	const FTransform SocketTransform = WeaponMesh->GetSocketTransform(MuzzleSocketName);
	MuzzleLocation =  SocketTransform.GetLocation();
	MuzzleDirection = SocketTransform.GetRotation().GetForwardVector();
}

bool ASTUBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;
	
	TraceStart = ViewLocation;
	const float HalfRad = FMath::DegreesToRadians(BulletSpread); 
	const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	return true;
}

bool ASTUBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const
{
	if (!GetWorld()) return false;
	
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(GetOwner());

	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionQueryParams);
	return true;
}

bool ASTUBaseWeapon::IsForwardShot(const FVector& ImpactPoint, const FVector&MuzzleLocation, const FVector& MuzzleDirection) const
{
	const FVector MuzzleShootDirection = ImpactPoint - MuzzleLocation;
	const float DotProduct = FVector::DotProduct(MuzzleDirection,  MuzzleShootDirection);
	const float AngleRad = FMath::Acos(DotProduct / (MuzzleDirection.Size() * MuzzleShootDirection.Size()));
	const float Angle = FMath::RadiansToDegrees(AngleRad);

	return FMath::Abs(Angle) <= 90.0f;
}

void ASTUBaseWeapon::MakeDamage(const FHitResult& HitResult)
{
	ASTUBaseCharacter* DamagedActor = Cast<ASTUBaseCharacter>(HitResult.GetActor());
	if (!DamagedActor) return;

	DamagedActor->TakeDamage(DamageAmount, FDamageEvent{}, GetPlayerController(), this);
}
