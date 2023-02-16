#pragma once

#include <iostream>
#include <fstream>

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FVCharacter.generated.h"


class UCameraComponent;
class AFVProjectile;
class ATcpNetActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnProjectileViewChange);

UENUM(BlueprintType)
enum class CameraPosition : uint8 {
	HEAD = 0 UMETA(DisplayName = "HEAD"),
	BACK = 1 UMETA(DisplayName = "BACK"),
	SIDE = 2 UMETA(DisplayName = "SIDE"),
};

UENUM(BlueprintType)
enum class CameraOwner : uint8 {
	LAUNCHER = 0 UMETA(DisplayName = "LAUNCHER"),
	MISSILE = 1  UMETA(DisplayName = "MISSILE"),
};

UENUM(BlueprintType)
enum class ViewerMode : uint8 {
	PLAY = 0 UMETA(DisplayName = "PLAY"),
	PAUSE = 1 UMETA(DisplayName = "PAUSE"),
	NEXT = 2 UMETA(DisplayName = "NEXT"),
	PREVIOUS = 3 UMETA(DisplayName = "PREVIOUS"),
	STOP = 4 UMETA(DisplayName = "STOP")
};


UCLASS()
class FLIGHTVIEWER_API AFVCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AFVCharacter();

	FVector GetNextCoord();

	FOnProjectileViewChange OnProjectileViewChange;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components");
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* LauncherMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<AFVProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> GUIWidgetClass;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	//ATcpNetActor* TcpSocketActor;

	void OnFindTcpActor();

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	FVector GetCurrentCoordinates() const;

private:

	std::ifstream in_stream;
	size_t CurrentCoordinatesIndex;
	TArray<FVector> Coordinates;
	CameraPosition CurrentCamera;
	CameraOwner CurrentActor;
	ViewerMode CurrentViewerMode;
	AFVProjectile* Projectile;
	bool IsCoordinatesLoaded;
	UUserWidget* GUIWidget;
	FVector LastCoordinates;

	void OnStart();
	void OnLoad();
	void OnCameraChange();
	void OnProjectileCameraChange();

	void OnActorChange();

	void OnViewerPlay();
	void OnViewerPause();
	void OnViewerNext();
	void OnViewerPrevious();

	void OpenFile(int Range);

	UFUNCTION()
		void OnMessageReceived(FString Message);

	void ParseAndSave(FString& Message);

};
