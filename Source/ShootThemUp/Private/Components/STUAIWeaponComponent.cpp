// Shoot Them Up Game. All Rights Reserved.


#include "Components/STUAIWeaponComponent.h"
#include "Weapon/STUBaseWeapon.h"

void USTUAIWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentWeaponIndex = FMath::Rand() % (Weapons.Num() - 1);
	EquipWeapon(CurrentWeaponIndex);
}

void USTUAIWeaponComponent::StartFire()
{
	if (!CanFire())
	{
		return;
	}

	if (CurrentWeapon->IsAmmoEmpty())
	{
		NextWeapon();
	}
	else
	{
		CurrentWeapon->StartFire();
	}
}

void USTUAIWeaponComponent::NextWeapon()
{
	if (!CanEquip())
	{
		return;
	}

	int32 NextIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
	while (NextIndex != CurrentWeaponIndex)
	{
		if (!Weapons[NextIndex]->IsAmmoEmpty())
		{
			break;
		}
		NextIndex = (NextIndex + 1) % Weapons.Num();
	}

	if (CurrentWeaponIndex != NextIndex)
	{
		CurrentWeaponIndex = NextIndex;
		EquipWeapon(CurrentWeaponIndex);
	}
}
