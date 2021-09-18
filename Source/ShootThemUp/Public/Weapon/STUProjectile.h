// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "STUProjectile.generated.h"

class USphereComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ASTUProjectile();

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
	USphereComponent* CollisionComponent;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "1.0"))
	float CollisionSphereRadius = 5.0f;
	
	virtual void BeginPlay() override;
};
