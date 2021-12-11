// Shoot Them Up Game. All Rights Reserved.


#include "Pickups/STUAmmoPickup.h"
#include "STUHealthComponent.h"
#include "STUWeaponComponent.h"
#include "STUUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthPickup, All, All)

bool ASTUAmmoPickup::GivePickupTo(APawn* PlayerPawn)
{
	const USTUHealthComponent* HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(PlayerPawn);
	if (!HealthComponent || HealthComponent->IsDead()) return false;

	USTUWeaponComponent* WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(PlayerPawn);
	if (!WeaponComponent) return false;
	
	return WeaponComponent->TryToAddAmmo(WeaponType, ClipsAmount);
}
