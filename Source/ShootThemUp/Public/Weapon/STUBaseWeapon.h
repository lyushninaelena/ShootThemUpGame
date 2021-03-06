// Shoot Them Up Game. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUCoreTypes.h"
#include "STUBaseWeapon.generated.h"

class USkeletalMeshComponent;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	ASTUBaseWeapon();

	FOnClipEmptySingnature OnClipEmpty;

	virtual void StartFire();
	virtual void StopFire();
	bool IsFiring() const { return FireInProgress; }

	void ChangeClip();
	bool CanReload() const;
	
	FWeaponUIData GetUIData() const { return UIData; }
	FAmmoData GetAmmoData() const { return CurrentAmmo; }
	bool IsAmmoEmpty() const;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FName MuzzleSocketName = "MuzzleSocket";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float TraceMaxDistance = 1500.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FAmmoData DefaultAmmo{15, 10, false};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	FWeaponUIData UIData;

	virtual void BeginPlay() override;
	
	virtual void MakeShot();
	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;

	APlayerController* GetPlayerController() const;
	bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;
	void GetMuzzleData(FVector& MuzzleLocation, FVector& MuzzleDirection) const;
	bool MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const;
	bool IsForwardShot(const FVector& ImpactPoint, const FVector& MuzzleLocation, const FVector& MuzzleDirection) const;

	void DecreaseAmmo();
	bool IsClipEmpty() const;
	bool IsAmmoFull() const;

	void LogAmmo();

private:
	FAmmoData CurrentAmmo;

	bool FireInProgress = false;

public:
	bool TryToAddAmmo(int32 ClipsAmount);
};
