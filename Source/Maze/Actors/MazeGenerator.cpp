// Fill out your copyright notice in the Description page of Project Settings.

#include "MazeGenerator.h"

// Sets default values
AMazeGenerator::AMazeGenerator(const FObjectInitializer& ObjectInitializer)	: Super(ObjectInitializer)
{

	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;
	MazeWallCellMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>("Maze Wall Cell Mesh");
	MazeWallCellMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMazeGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMazeGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMazeGenerator::GenerateMaze(bool & success)
{
	for (int32 i = 0; i < Height; i++) 
	{
		for (int32 j = 0; j < Width; j++)
		{
			UMazeCell* CurrentCell = NewObject<UMazeCell>(UMazeCell::StaticClass());

			if (CurrentCell != nullptr)
			{
				CurrentCell->Location = FVector(j * Step, i * Step, 0.f);
				//MazeCells.Add(CurrentCell);
			}
			else
			{
				success = false;
				return;
			}
			if ((i % 2 != 0 && j % 2 != 0) && (i < Height - 1 && j < Width - 1))
				CurrentCell->bIsEmpty = true;
			else
				MazeWallCellMesh->AddInstance(FTransform(CurrentCell->Location));
			CurrentCell->ConditionalBeginDestroy();
		}
	}
}

