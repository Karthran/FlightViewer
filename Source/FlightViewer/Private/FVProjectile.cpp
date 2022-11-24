

#include "FVProjectile.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogAFVProjectile, All, All)

AFVProjectile::AFVProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	CollisionComponent->InitSphereRadius(5.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	SetRootComponent(CollisionComponent);

	ProjectileStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("ProjectileStaticMesh");
	ProjectileStaticMesh->SetupAttachment(CollisionComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(CollisionComponent);
}

void AFVProjectile::BeginPlay()
{
	Super::BeginPlay();

	CurrentCamera = CameraPosition::HEAD;

	Cast<AFVCharacter>(GetOwner())->OnProjectileViewChange.AddDynamic(this, &AFVProjectile::OnViewChanged);
}

void AFVProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Coordinates = Cast<AFVCharacter>(GetOwner())->GetNextCoord();

	SetActorLocation(Coordinates);
}

void AFVProjectile::OnViewChanged()
{
	switch (CurrentCamera)
	{
	case CameraPosition::HEAD:
		CameraComponent->AddRelativeLocation(FVector(-470.0f, 0.0f, 100.0f));
//		CameraComponent->AddRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
		CurrentCamera = CameraPosition::BACK;
		break;
	case  CameraPosition::BACK:
		CameraComponent->AddRelativeLocation(FVector(470.0f, 0.0f, -100.0f));
//		CameraComponent->AddRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
		CurrentCamera = CameraPosition::HEAD;
		break;
	}
	UE_LOG(LogAFVProjectile, Error, TEXT("Now changed view"))
}

