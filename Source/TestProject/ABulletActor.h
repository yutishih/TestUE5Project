#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ABulletActor.generated.h"

UCLASS()
class TESTPROJECT_API AABulletActor : public AActor
{
    GENERATED_BODY()
    
public:	
    AABulletActor();

protected:
    UPROPERTY(VisibleAnywhere)
    USphereComponent* CollisionComponent;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* BulletMesh;

    UPROPERTY(VisibleAnywhere)
    UProjectileMovementComponent* ProjectileMovement;

    virtual void BeginPlay() override;

public:	
    virtual void Tick(float DeltaTime) override;
};