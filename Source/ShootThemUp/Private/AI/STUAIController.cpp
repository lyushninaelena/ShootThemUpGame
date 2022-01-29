// Shoot Them Up Game. All Rights Reserved.

#include "AI/STUAIController.h"
#include "AI/STUAICharacter.h"
#include "STUAIPerceptionComponent.h"
#include <BehaviorTree/BlackboardComponent.h>

 ASTUAIController::ASTUAIController()
{
	 STUAIPerceptionComponent = CreateDefaultSubobject<USTUAIPerceptionComponent>("STUAIPerceptionComponent");
	SetPerceptionComponent(*STUAIPerceptionComponent);
 }

void ASTUAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ASTUAICharacter* STUAICharacter = Cast<ASTUAICharacter>(InPawn);
	if (STUAICharacter)
	{
		RunBehaviorTree(STUAICharacter->BehaviorTreeAsset);
	}
}

void ASTUAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AActor* AimActor = GetFocusOnActor();
	SetFocus(AimActor);
}

AActor* ASTUAIController::GetFocusOnActor() const
{
	if (!GetBlackboardComponent())
	{
		return nullptr;
	}

	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
}
