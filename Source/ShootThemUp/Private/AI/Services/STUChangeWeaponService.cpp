// Shoot Them Up Game. All Rights Reserved.


#include "AI/Services/STUChangeWeaponService.h"
#include <AIController.h>
#include "STUWeaponComponent.h"
#include "STUUtils.h"


 USTUChangeWeaponService::USTUChangeWeaponService()
{
	NodeName = "Change Weapon";
}

void USTUChangeWeaponService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* Controller = OwnerComp.GetAIOwner();
	if (Controller == nullptr)
	{
		return;
	}

	USTUWeaponComponent* WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(Controller->GetPawn());
	if (WeaponComponent == nullptr)
	{
		return;
	}

	if (Probability > 0.0f && FMath::FRand() <= Probability)
	{
		WeaponComponent->NextWeapon();
	}
}