
#include "FVCharacter.h"
#include "FVProjectile.h"
#include "Camera/CameraComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogFVCharacter, All, All)

AFVCharacter::AFVCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(GetRootComponent());

	//LauncherMesh = CreateDefaultSubobject<USkeletalMeshComponent>("LauncherMesh");
	//LauncherMesh->SetupAttachment(GetRootComponent());

	IsCoordinatesLoaded = false;
}

FVector AFVCharacter::GetNextCoord()
{
	FVector Result;
	switch (CurrentViewerMode)
	{
	case ViewerMode::PLAY:
		if (CurrentCoordinatesIndex < Coordinates.Num())
		{
			Result = Coordinates[CurrentCoordinatesIndex++];
		}
		else
		{
			CurrentCoordinatesIndex = 0;
			Result = Coordinates[0];
		}
		break;
	case ViewerMode::PAUSE:
		UE_LOG(LogFVCharacter, Error, TEXT("CurrentCoordinatesIndex: %d"), CurrentCoordinatesIndex)
		Result = Coordinates[CurrentCoordinatesIndex];
		break;
	case ViewerMode::NEXT:
		if (CurrentCoordinatesIndex < Coordinates.Num() - 1)
		{
			Result = Coordinates[CurrentCoordinatesIndex++];
		}
		else
		{
			CurrentCoordinatesIndex = 0;
			Result = Coordinates[CurrentCoordinatesIndex];
		}
		CurrentViewerMode = ViewerMode::PAUSE;
		break;
	case ViewerMode::PREVIOUS:
		if (CurrentCoordinatesIndex > 0)
		{
			Result = Coordinates[--CurrentCoordinatesIndex];
		}
		else
		{
			CurrentCoordinatesIndex = Coordinates.Num() - 1;
			Result = Coordinates[CurrentCoordinatesIndex];
		}
		CurrentViewerMode = ViewerMode::PAUSE;
		break;
	}
	return Result;
}

void AFVCharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentCamera = CameraPosition::BACK;
	CurrentActor = CameraOwner::LAUNCHER;
	CurrentViewerMode = ViewerMode::PAUSE;
}

void AFVCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFVCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Start", IE_Pressed, this, &AFVCharacter::OnStart);
	PlayerInputComponent->BindAction("Load", IE_Pressed, this, &AFVCharacter::OnLoad);
	PlayerInputComponent->BindAction("CameraChange", IE_Pressed, this, &AFVCharacter::OnCameraChange);
	PlayerInputComponent->BindAction("ProjectileCameraChange", IE_Pressed, this, &AFVCharacter::OnProjectileCameraChange);

	PlayerInputComponent->BindAction("ActorChange", IE_Pressed, this, &AFVCharacter::OnActorChange);

	PlayerInputComponent->BindAction("ViewerPlay", IE_Pressed, this, &AFVCharacter::OnViewerPlay);
	PlayerInputComponent->BindAction("ViewerPause", IE_Pressed, this, &AFVCharacter::OnViewerPause);
	PlayerInputComponent->BindAction("ViewerNext", IE_Pressed, this, &AFVCharacter::OnViewerNext);
	PlayerInputComponent->BindAction("ViewerPrevious", IE_Pressed, this, &AFVCharacter::OnViewerPrevious);

	DECLARE_DELEGATE_OneParam(FFileLoadSignature, int);
	PlayerInputComponent->BindAction<FFileLoadSignature>("400m", IE_Pressed, this, &AFVCharacter::OpenFile, 400);
	PlayerInputComponent->BindAction<FFileLoadSignature>("500m", IE_Pressed, this, &AFVCharacter::OpenFile, 500);
	PlayerInputComponent->BindAction<FFileLoadSignature>("600m", IE_Pressed, this, &AFVCharacter::OpenFile, 600);
	PlayerInputComponent->BindAction<FFileLoadSignature>("700m", IE_Pressed, this, &AFVCharacter::OpenFile, 700);
	PlayerInputComponent->BindAction<FFileLoadSignature>("800m", IE_Pressed, this, &AFVCharacter::OpenFile, 800);

}

void AFVCharacter::OnStart()
{
	if (!IsCoordinatesLoaded) return;

	FVector Location = GetMesh()->GetComponentLocation();
	Location.X += 50.0f;
	Location.Z += 10.0f;

	FRotator Rotator = GetRootComponent()->GetComponentRotation();

	UE_LOG(LogFVCharacter, Warning, TEXT("Current Direction: %f %f %f"), Location.X, Location.Y, Location.Z)

	const FTransform SpawnTransform(Rotator, Location);

	Projectile = GetWorld()->SpawnActorDeferred<AFVProjectile>(ProjectileClass, SpawnTransform);
	if (!Projectile)
		return;
	Projectile->SetOwner(this);
	Projectile->FinishSpawning(SpawnTransform);
}

void AFVCharacter::OnLoad()
{
	in_stream.open("D:\\800_m.txt");
	float valueX, valueY, valueZ;

	while (in_stream >> valueX >> valueY >> valueZ)
	{
		Coordinates.Push(FVector(valueX, valueY, valueZ));
	}

	CurrentCoordinatesIndex = 0;
	IsCoordinatesLoaded = true;
}

void AFVCharacter::OnCameraChange()
{
	switch (CurrentCamera)
	{
	case CameraPosition::BACK:
		CameraComponent->AddRelativeLocation(FVector(470.0f, 470.0f, 0.0f));
		CameraComponent->AddRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
		CurrentCamera = CameraPosition::SIDE;
		break;
	case  CameraPosition::SIDE:
		CameraComponent->AddRelativeLocation(FVector(-470.0f, -470.0f, 0.0f));
		CameraComponent->AddRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
		CurrentCamera = CameraPosition::BACK;
		break;
	}
}

void AFVCharacter::OnProjectileCameraChange()
{
	OnProjectileViewChange.Broadcast();
}

void AFVCharacter::OnActorChange()
{
	const auto PlayerController = Cast<APlayerController>(GetController());

	switch (CurrentActor)
	{
	case CameraOwner::LAUNCHER:
		PlayerController->SetViewTarget(Projectile);
		CurrentActor = CameraOwner::MISSILE;
		break;
	case CameraOwner::MISSILE:
		PlayerController->SetViewTarget(this);
		CurrentActor = CameraOwner::LAUNCHER;
		break;
	}

}

void AFVCharacter::OnViewerPlay()
{
	CurrentViewerMode = ViewerMode::PLAY;
}

void AFVCharacter::OnViewerPause()
{
	CurrentViewerMode = ViewerMode::PAUSE;
}

void AFVCharacter::OnViewerNext()
{
	CurrentViewerMode = ViewerMode::NEXT;
}

void AFVCharacter::OnViewerPrevious()
{
	CurrentViewerMode = ViewerMode::PREVIOUS;
}

void AFVCharacter::OpenFile(int Range)
{
	if (in_stream.is_open()) in_stream.close();

	auto Name = FString("D:\\") + FString::FromInt(Range) + FString("_m.txt");
	std::string const file_name = TCHAR_TO_UTF8(*Name);
	in_stream.open(file_name);

	float valueX, valueY, valueZ;

	Coordinates.Empty();

	while (in_stream >> valueX >> valueY >> valueZ)
	{
		Coordinates.Push(FVector(valueX, valueY, valueZ));
	}

	CurrentCoordinatesIndex = 0;
	
	IsCoordinatesLoaded = true;
}

