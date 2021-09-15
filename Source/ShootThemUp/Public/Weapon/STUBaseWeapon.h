// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBaseWeapon.generated.h"

class USkeletalMeshComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	ASTUBaseWeapon();

	virtual void Fire();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FName MuzzleSocketName = "MuzzleSocket";

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float TraceMaxDistance = 1500.0f;
		
	virtual void BeginPlay() override;

	void MakeShot() const;
	APlayerController* GetPlayerController() const;
	bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;
	void GetMuzzleData(FVector& MuzzleLocation, FVector& MuzzleDirection) const;
	bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;
	bool MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const;
	bool IsForwardShot(const FVector& ImpactPoint, const FVector&MuzzleLocation, const FVector& MuzzleDirection) const;
};
