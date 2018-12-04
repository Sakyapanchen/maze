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
	success = true;
	UMazeCell* StartCell = nullptr;

	GenerateSourceMatrix(success);
	int32 i = 1;
	int32 j = 1;
	MazeWallCellMesh->ClearInstances();
	if (MazeCellRows.IsValidIndex(i))
		if (MazeCellRows[i].Cells.IsValidIndex(j))
			StartCell = MazeCellRows[i].Cells[j];
		else
		{
			success = false;
			return;
		}
	else
	{
		success = false;
		return;
	}

	StartCell->bIsEmpty = true;
	//CurrentCell->ConditionalBeginDestroy();
}

void AMazeGenerator::GenerateSourceMatrix(bool & success)
{
	success = true;

	UMazeCell* CurrentCell = nullptr;
	for (int32 i = 0; i < Height; i++)
	{
		MazeCellRows.Add(FCellsRow());
		for (int32 j = 0; j < Width; j++)
		{
			CurrentCell = NewObject<UMazeCell>(UMazeCell::StaticClass());

			if (CurrentCell != nullptr)
			{
				CurrentCell->Location = FVector(j * Step, i * Step, 0.f);
				CurrentCell->j = j;
				CurrentCell->i = i;
			}
			else
			{
				success = false;
				return;
			}
			if ((i % 2 != 0 && j % 2 != 0) && (i < Height - 1 && j < Width - 1))
				CurrentCell->bIsEmpty = true;

			if (MazeCellRows.IsValidIndex(i))
				if (MazeCellRows[i].Cells.IsValidIndex(j - 1))
				{
					CurrentCell->NeighboringCells.Add(ENeighborCellPosition::NPE_Left, MazeCellRows[i].Cells[j - 1]);
					MazeCellRows[i].Cells[j - 1]->NeighboringCells.Add(ENeighborCellPosition::NPE_Right, CurrentCell);
				}
			if (MazeCellRows.IsValidIndex(i - 1))
				if (MazeCellRows[i - 1].Cells.IsValidIndex(j))
				{
					CurrentCell->NeighboringCells.Add(ENeighborCellPosition::NPE_Down, MazeCellRows[i - 1].Cells[j]);
					MazeCellRows[i - 1].Cells[j]->NeighboringCells.Add(ENeighborCellPosition::NPE_Up, CurrentCell);
				}
			MazeCellRows[i].Cells.Add(CurrentCell);
		}
	}
}

