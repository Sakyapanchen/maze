// Fill out your copyright notice in the Description page of Project Settings.

#include "MazeGenerator.h"

#include "Engine/Engine.h"
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"

// Sets default values
AMazeGenerator::AMazeGenerator(const FObjectInitializer& ObjectInitializer)	: Super(ObjectInitializer)
{

	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;
	MazeWallCellMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>("Maze Wall Cell Mesh");
	MazeWallCellMesh->SetupAttachment(RootComponent);
	MazeFloorCellMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>("Maze Floor Cell Mesh");
	MazeFloorCellMesh->SetupAttachment(RootComponent);
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

	GenerateSourceMatrix(success);
	CreatePass();

	int32 Row = 1;
	int32 Column = 1;

	MazeWallCellMesh->ClearInstances();
	MazeFloorCellMesh->ClearInstances();

	if (MazeCellRows.IsValidIndex(Row))
		if (MazeCellRows[Row].Cells.IsValidIndex(Column))
			StartCell = MazeCellRows[Row].Cells[Column];
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

	do
	{
		CurrentCell->bIsVisited = true;
		TArray<UMazeCell*> UnvisitedNeighbors;
		CurrentCell->GetUnvisitedNeighbors(UnvisitedNeighbors);

		if (UnvisitedNeighbors.Num() > 0)
		{
			int32 RandIndex = FMath::RandRange(0, UnvisitedNeighbors.Num() - 1);
			UMazeCell * NeighborCell = UnvisitedNeighbors[RandIndex];
			NeighborCell->bIsVisited = true;

			RemoveWall(CurrentCell, NeighborCell);

			if (UnvisitedNeighbors.Num() > 1)
				LastCellStack.AddUnique(CurrentCell);

			CurrentCell = NeighborCell;
		}
		else
		{
			if (LastCellStack.Num() > 0)
			{
				CurrentCell = LastCellStack.Last();
				LastCellStack.Remove(CurrentCell);
			}
		}
	}
	while (LastCellStack.Num() > 0);

	for (FCellsRow CurrentRow : MazeCellRows)
	{
		for (UMazeCell * ItrCell : CurrentRow.Cells)
		{
			FVector Location = FVector(ItrCell->Column * Step, ItrCell->Row * Step, 0.f);
			FTransform Transform = FTransform(Location);
			MazeFloorCellMesh->AddInstance(Transform);

			if(!ItrCell->bIsEmpty)
			{
				MazeWallCellMesh->AddInstance(Transform);
			}
			ItrCell->ConditionalBeginDestroy();
			ItrCell = nullptr;
		}
	}	
	MazeCellRows.Empty();
	GEngine->ForceGarbageCollection(true);
	OnMazeGenerationEnd.Broadcast();
}

void AMazeGenerator::GenerateSourceMatrix(bool & success)
{
	success = true;
	MazeCellRows.Empty();

	UMazeCell* CurrentCell = nullptr;
	for (int32 Row = 0; Row < Height; Row++)
	{
		MazeCellRows.Add(FCellsRow());

		for (int32 Column = 0; Column < Width; Column++)
		{
			CurrentCell = NewObject<UMazeCell>(UMazeCell::StaticClass());

			if (CurrentCell != nullptr)
			{
				CurrentCell->Column = Column;
				CurrentCell->Row = Row;
			}
			else
			{
				success = false;
				return;
			}
			if ((Row % 2 != 0 && Column % 2 != 0) && (Row < Height - 1 && Column < Width - 1))
				CurrentCell->bIsEmpty = true;

			int32 CellStep = 2;

			if (MazeCellRows.IsValidIndex(Row))
				if (MazeCellRows[Row].Cells.IsValidIndex(Column - CellStep))
				{
					CurrentCell->NeighboringCells.Add(MazeCellRows[Row].Cells[Column - CellStep]);
					MazeCellRows[Row].Cells[Column - CellStep]->NeighboringCells.Add(CurrentCell);
				}
			if (MazeCellRows.IsValidIndex(Row - CellStep))
				if (MazeCellRows[Row - CellStep].Cells.IsValidIndex(Column))
				{
					CurrentCell->NeighboringCells.Add(MazeCellRows[Row - CellStep].Cells[Column]);
					MazeCellRows[Row - CellStep].Cells[Column]->NeighboringCells.Add(CurrentCell);
				}
			MazeCellRows[Row].Cells.Add(CurrentCell);
		}	
	}
}

void AMazeGenerator::RemoveWall(UMazeCell * CurrentCell, UMazeCell * NextCell)
{
	int32 WallRow = CurrentCell->Row;
	int32 WallColumn = CurrentCell->Column;

	if (CurrentCell->Row != NextCell->Row)
		WallRow = CurrentCell->Row < NextCell->Row ? CurrentCell->Row + 1 : NextCell->Row + 1;
	if (CurrentCell->Column != NextCell->Column)
		WallColumn = CurrentCell->Column < NextCell->Column ? CurrentCell->Column + 1 : NextCell->Column + 1;

	MazeCellRows[WallRow].Cells[WallColumn]->bIsEmpty = true;
	MazeCellRows[WallRow].Cells[WallColumn]->bIsVisited = true;
}

void AMazeGenerator::CreatePass()
{
	int32 EnterRow = -1;
	int32 EnterColumn = -1;
	int32 ExitRow = -1;
	int32 ExitColumn = -1;
	if (FMath::RandBool())
	{
		EnterRow = 0;
		EnterColumn = FMath::RandRange(1, PassWidth) * 2 - 1;
		ExitRow = Height - 1;
		ExitColumn = FMath::RandRange(1, PassWidth) * 2 - 1;
	}
	else
	{
		EnterRow = FMath::RandRange(1, PassHeight) * 2 - 1;
		EnterColumn = 0;
		ExitRow = FMath::RandRange(1, PassHeight) * 2 - 1;
		ExitColumn = Width - 1;
	}
	if (MazeCellRows.Num() > 0)
	{
		MazeCellRows[EnterRow].Cells[EnterColumn]->bIsEmpty = true;
		MazeCellRows[ExitRow].Cells[ExitColumn]->bIsEmpty = true;
	}
}


