// Shoot Them Up Game. All Rights Reserved.

#include "Pickups/STUBasePickup.h"
#include <Components/SphereComponent.h>

DEFINE_LOG_CATEGORY_STATIC(LogBasePickup, All, All)

ASTUBasePickup::ASTUBasePickup()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
	CollisionComponent->InitSphereRadius(CollisionSphereRadius);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SetRootComponent(CollisionComponent);
}

void ASTUBasePickup::BeginPlay()
{
	Super::BeginPlay();

	check(CollisionComponent)
	
	GenerateRotationYaw();
}

void ASTUBasePickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	APawn* Pawn = Cast<APawn>(OtherActor);
	if (GivePickupTo(Pawn))
	{
		PickupWasTaken();
	}
	else if (Pawn)
	{
		OverlappingPawns.Add(Pawn);

		if (!OverlappingPawnsTimerHandle.IsValid())
		{
			GetWorldTimerManager().SetTimer(
				OverlappingPawnsTimerHandle, this, &ASTUBasePickup::CheckOverlappingPaws, OverlappingPawnsCheckTime, true);
		}
	}
}

void ASTUBasePickup::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	const auto Pawn = Cast<APawn>(OtherActor);
	OverlappingPawns.Remove(Pawn);

	if (OverlappingPawns.Num() == 0 && OverlappingPawnsTimerHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(OverlappingPawnsTimerHandle);
	}
}

void ASTUBasePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalRotation(FRotator(0.0f, RotationYaw, 0.0f));
}

bool ASTUBasePickup::GivePickupTo(APawn* PlayerPawn)
{
	return false;
}

void ASTUBasePickup::PickupWasTaken()
{
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	if (GetRootComponent())
	{
		GetRootComponent()->SetVisibility(false, true);
	}

	GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ASTUBasePickup::Respawn, RespawnTime);
}

void ASTUBasePickup::Respawn()
{
	GenerateRotationYaw();

	if (GetRootComponent())
	{
		GetRootComponent()->SetVisibility(true, true);
	}

	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}

void ASTUBasePickup::GenerateRotationYaw()
{
	const float Direction = FMath::RandBool() ? 1.0f : -1.0f;
	RotationYaw = FMath::RandRange(1.0f, 2.0f) * Direction;
}

void ASTUBasePickup::CheckOverlappingPaws()
{
	for (APawn* Pawn : OverlappingPawns)
	{
		if (GivePickupTo(Pawn)) 
		{
			PickupWasTaken();
			break;
		}
	}
}

bool ASTUBasePickup::CouldBeTaken() const
{
	return !GetWorldTimerManager().IsTimerActive(RespawnTimerHandle);
}
