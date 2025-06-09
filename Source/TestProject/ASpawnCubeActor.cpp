// Fill out your copyright notice in the Description page of Project Settings.

#include "ASpawnCubeActor.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AASpawnCubeActor::AASpawnCubeActor()
{
	PrimaryActorTick.bCanEverTick = true;

	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
	RootComponent = CubeMesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeAsset.Succeeded())
	{
		CubeMesh->SetStaticMesh(CubeAsset.Object);
	}
}

// Called when the game starts or when spawned
void AASpawnCubeActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AASpawnCubeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

