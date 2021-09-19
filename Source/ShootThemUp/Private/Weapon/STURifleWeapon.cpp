// Shoot Them Up Game. All Rights Reserved.


#include "Weapon/STURifleWeapon.h"
#include "DrawDebugHelpers.h"
#include "STUBaseCharacter.h"
#include "Engine/World.h"

void ASTURifleWeapon::StartFire()
{
	MakeShot();
	GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASTURifleWeapon::MakeShot, TimeBetweenShots, true);
}

void ASTURifleWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(ShotTimerHandle);
}

void ASTURifleWeapon::MakeShot()
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

bool ASTURifleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
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


void ASTURifleWeapon::MakeDamage(const FHitResult& HitResult)
{
	ASTUBaseCharacter* DamagedActor = Cast<ASTUBaseCharacter>(HitResult.GetActor());
	if (!DamagedActor) return;

	DamagedActor->TakeDamage(DamageAmount, FDamageEvent{}, GetPlayerController(), this);
}
