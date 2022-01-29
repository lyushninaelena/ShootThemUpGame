// Shoot Them Up Game. All Rights Reserved.

#include "Components/STUAIPerceptionComponent.h"
#include <AIController.h>
#include "../../Public/Components/STUHealthComponent.h"
#include "../../Public/STUUtils.h"
#include <Perception/AISense_Sight.h>

AActor* USTUAIPerceptionComponent::GetClosestEnemy() const
{
	TArray<AActor*> PercieveActors;
	GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PercieveActors);

	if (PercieveActors.Num() == 0)
	{
		return nullptr;
	}

	AAIController* Controller = Cast<AAIController>(GetOwner());
	if (!Controller)
	{
		return nullptr;
	}

	APawn* Pawn = Controller->GetPawn();
	if (!Pawn)
	{
		return nullptr;
	}

	float BestDistance = MAX_FLT;
	AActor* BestPawn = nullptr;
	for (AActor* Actor : PercieveActors)
	{
		USTUHealthComponent* HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(Actor);
		if (HealthComponent && !HealthComponent->IsDead()) // TODO: check if enemies or not
		{
			float CurrentDistance = (Actor->GetActorLocation() - Pawn->GetActorLocation()).Size();
			if (CurrentDistance < BestDistance)
			{
				BestDistance = CurrentDistance;
				BestPawn = Actor;
			}
		}
	}

	return BestPawn;
}
