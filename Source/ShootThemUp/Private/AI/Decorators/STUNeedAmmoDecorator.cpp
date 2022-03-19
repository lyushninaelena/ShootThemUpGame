// Shoot Them Up Game. All Rights Reserved.

#include "AI/Decorators/STUNeedAmmoDecorator.h"
#include "AIController.h"
#include "STUUtils.h"
#include "Components/STUWeaponComponent.h"

 USTUNeedAmmoDecorator::USTUNeedAmmoDecorator()
{
	 NodeName = "Need Ammo";
 }

bool USTUNeedAmmoDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller)
	{
		return false;
	}

	USTUWeaponComponent* WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(Controller->GetPawn());
	if (!WeaponComponent)
	{
		return false;
	}

	return WeaponComponent->IsWeaponAmmoEmpty(WeaponType);
}
