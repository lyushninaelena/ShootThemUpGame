// Shoot Them Up Game. All Rights Reserved.


#include "Weapon/STUProjectile.h"
#include "Components/SphereComponent.h"

ASTUProjectile::ASTUProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
	CollisionComponent->InitSphereRadius(CollisionSphereRadius);
	SetRootComponent(CollisionComponent);
}

void ASTUProjectile::BeginPlay()
{
	Super::BeginPlay();
}
