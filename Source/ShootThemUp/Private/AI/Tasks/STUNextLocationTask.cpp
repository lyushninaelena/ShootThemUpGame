// Shoot Them Up Game. All Rights Reserved.


#include "AI/Tasks/STUNextLocationTask.h"
#include <BehaviorTree/BlackboardComponent.h>
#include <AIController.h>
#include <GameFramework/Pawn.h>
#include <NavigationSystem.h>

 USTUNextLocationTask::USTUNextLocationTask()
{
	 NodeName = "Next Location";
 }

EBTNodeResult::Type USTUNextLocationTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* Controller = OwnerComp.GetAIOwner();
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if (!Controller || !Blackboard)
	{
		return EBTNodeResult::Failed;
	}

	APawn* Pawn = Controller->GetPawn();
	if (!Pawn)
	{
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(Pawn);
	if (!NavSys)
	{
		return EBTNodeResult::Failed;
	}

	FNavLocation NavLocation;
	FVector Location = Pawn->GetActorLocation();

	if (SelfCenter == false)
	{
		AActor* CenterActor = Cast<AActor>(Blackboard->GetValueAsObject(CenterActorKey.SelectedKeyName));
		if (CenterActor == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		Location = CenterActor->GetActorLocation();
	}

	const bool Found = NavSys->GetRandomReachablePointInRadius(Location, Radius, NavLocation);
	if (!Found)
	{
		return EBTNodeResult::Failed;
	}

	Blackboard->SetValueAsVector(AimLocationKey.SelectedKeyName, NavLocation.Location);

	return EBTNodeResult::Succeeded;
}
