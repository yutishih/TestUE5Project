// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ASpawnCubeActor.generated.h"

UCLASS()
class TESTPROJECT_API AASpawnCubeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AASpawnCubeActor();

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* CubeMesh;

	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
