// Fill out your copyright notice in the Description page of Project Settings.

#include "MazeGenerator.h"
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"

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
	Width = PassWidth * 2 + 1;
	Height = PassHeight * 2 + 1;

	success = true;
	UMazeCell* StartCell = nullptr;
	UMazeCell* CurrentCell = nullptr;
	TArray<UMazeCell*> LastCellStack;

	int32 VisitedEmptyCells = PassHeight * PassWidth;

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
	CurrentCell = StartCell;

	while (VisitedEmptyCells > 0)
	{
		CurrentCell->bIsEmpty = true;
		CurrentCell->bIsVisited = true;

		VisitedEmptyCells--;

		if (CurrentCell->NeighboringCells.Num() > 0)
		{
			int32 RandIndex = FMath::RandRange(0, CurrentCell->NeighboringCells.Num() - 1);

			TArray<ENeighborCellPosition> Keys;

			Keys.Empty();
			CurrentCell->NeighboringCells.GenerateKeyArray(Keys);
			UMazeCell * NeighborCell = *CurrentCell->NeighboringCells.Find(Keys[RandIndex]);
			CurrentCell->NeighboringCells.Remove(Keys[RandIndex]);

			Keys.Empty();
			CurrentCell->NeighboringWalls.GenerateKeyArray(Keys);
			UMazeCell * NeighborWall = *CurrentCell->NeighboringWalls.Find(Keys[RandIndex]);
			CurrentCell->NeighboringWalls.Remove(Keys[RandIndex]);

			if (CurrentCell->NeighboringCells.Num() > 0)
				LastCellStack.AddUnique(CurrentCell);
			NeighborWall->bIsEmpty = true;
			NeighborWall->bIsVisited = true;
			CurrentCell = NeighborCell;
		}
		else
		{
			if (LastCellStack.Num() > 0)
			{
				int32 LastIndex = LastCellStack.Num() - 1;
				CurrentCell = LastCellStack.Last();
				LastCellStack.Remove(CurrentCell);
			}
		}
	}

	for (FCellsRow CurrentRow : MazeCellRows)
	{
		for (UMazeCell * CurrentCell : CurrentRow.Cells)
		{
			if(!CurrentCell->bIsEmpty)
			{
				FVector Location = FVector(CurrentCell->j * Step, CurrentCell->i * Step, 0.f);
				FTransform Transform = FTransform(Location);
				MazeWallCellMesh->AddInstance(Transform);
			}
		}
	}	
		//NeighboringCells.GRandRange(0, NeighboringCells.Num() - 1)
	//CurrentCell->NeighboringCells.Num
	//CurrentCell->ConditionalBeginDestroy();
}

void AMazeGenerator::GenerateSourceMatrix(bool & success)
{
	success = true;
	MazeCellRows.Empty();

	UMazeCell* CurrentCell = nullptr;
	for (int32 i = 0; i < Height; i++)
	{
		MazeCellRows.Add(FCellsRow());

		for (int32 j = 0; j < Width; j++)
		{
			CurrentCell = NewObject<UMazeCell>(UMazeCell::StaticClass());

			if (CurrentCell != nullptr)
			{
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

			for (int32 Itr = 1; Itr < 3; Itr++)
			{
				if (Itr == 1)
				{
					if (MazeCellRows.IsValidIndex(i))
						if (MazeCellRows[i].Cells.IsValidIndex(j - Itr))
						{
							CurrentCell->NeighboringWalls.Add(ENeighborCellPosition::NPE_Left, MazeCellRows[i].Cells[j - Itr]);
							MazeCellRows[i].Cells[j - Itr]->NeighboringWalls.Add(ENeighborCellPosition::NPE_Right, CurrentCell);
						}
					if (MazeCellRows.IsValidIndex(i - Itr))
						if (MazeCellRows[i - Itr].Cells.IsValidIndex(j))
						{
							CurrentCell->NeighboringWalls.Add(ENeighborCellPosition::NPE_Down, MazeCellRows[i - Itr].Cells[j]);
							MazeCellRows[i - Itr].Cells[j]->NeighboringWalls.Add(ENeighborCellPosition::NPE_Up, CurrentCell);
						}
				}
				else
				{
					if (MazeCellRows.IsValidIndex(i))
						if (MazeCellRows[i].Cells.IsValidIndex(j - Itr))
						{
							CurrentCell->NeighboringCells.Add(ENeighborCellPosition::NPE_Left, MazeCellRows[i].Cells[j - Itr]);
							MazeCellRows[i].Cells[j - Itr]->NeighboringCells.Add(ENeighborCellPosition::NPE_Right, CurrentCell);
						}
					if (MazeCellRows.IsValidIndex(i - Itr))
						if (MazeCellRows[i - Itr].Cells.IsValidIndex(j))
						{
							CurrentCell->NeighboringCells.Add(ENeighborCellPosition::NPE_Down, MazeCellRows[i - Itr].Cells[j]);
							MazeCellRows[i - Itr].Cells[j]->NeighboringCells.Add(ENeighborCellPosition::NPE_Up, CurrentCell);
						}
				}
			}
			MazeCellRows[i].Cells.Add(CurrentCell);
		}	
	}
}


