// Shoot Them Up Game. All Rights Reserved.

#include "Weapon/STULauncherWeapon.h"
#include "Weapon/STUProjectile.h"
#include "Kismet/GameplayStatics.h"

void ASTULauncherWeapon::StartFire()
{
	MakeShot();
}

void ASTULauncherWeapon::MakeShot()
{
	FVector MuzzleLocation, MuzzleDirection;
	GetMuzzleData(MuzzleLocation, MuzzleDirection);
	const FTransform SpawnTransform(FRotator::ZeroRotator, MuzzleLocation);
	auto Projectile = UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), ProjectileClass, SpawnTransform);
	// set projectile params

	UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);
}
