// Shoot Them Up Game. All Rights Reserved.


#include "Pickups/STUAmmoPickup.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthPickup, All, All)

bool ASTUAmmoPickup::GivePickupTo(const APawn* PlayerPawn)
{
	UE_LOG(LogHealthPickup, Display, TEXT("Ammo was taken"))
	return true;
}
