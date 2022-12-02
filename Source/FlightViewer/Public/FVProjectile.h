// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FVCharacter.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FVProjectile.generated.h"

class USphereComponent;
class UCameraComponent;
class UNiagaraSystem;

UCLASS()
class FLIGHTVIEWER_API AFVProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AFVProjectile();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	UStaticMeshComponent* ProjectileStaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components");
	UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	UNiagaraSystem* Effect;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	CameraPosition CurrentCamera;
	FVector PreviousCoordinates;
	bool IsImpact;

	UFUNCTION()
	void OnViewChanged();

	void PlayImpactFX(const FVector& Location,const FVector& Normal);
};
