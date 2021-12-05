// Shoot Them Up Game. All Rights Reserved.

#include "Weapon/STULauncherWeapon.h"
#include "Weapon/STUProjectile.h"

void ASTULauncherWeapon::StartFire()
{
	Super::StartFire();

	MakeShot();
}

void ASTULauncherWeapon::MakeShot()
{
	if (!GetWorld() || IsAmmoEmpty()) return;
	
	FVector TraceStart, TraceEnd;
	if (!GetTraceData(TraceStart, TraceEnd)) return;

	FVector MuzzleLocation, MuzzleDirection;
	GetMuzzleData(MuzzleLocation, MuzzleDirection);
	
	FHitResult HitResult;
	if (!MakeHit(HitResult, TraceStart, TraceEnd)) return;

	const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
	const FVector Direction = (EndPoint - MuzzleLocation).GetSafeNormal();
	
	const FTransform SpawnTransform(FRotator::ZeroRotator, MuzzleLocation);
	ASTUProjectile* Projectile = GetWorld()->SpawnActorDeferred<ASTUProjectile>(ProjectileClass, SpawnTransform);
	if (Projectile)
	{
		Projectile->SetShotDirection(Direction);
		Projectile->SetOwner(GetOwner());
		Projectile->FinishSpawning(SpawnTransform);
	}

	DecreaseAmmo();
}
