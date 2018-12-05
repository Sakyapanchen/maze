// Fill out your copyright notice in the Description page of Project Settings.

#include "MazeCell.h"

UMazeCell::UMazeCell(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}


void UMazeCell::GetUnvisitedNeighbors(TArray < UMazeCell *> & UnvisitedNeighbors)
{
	for (auto CurrentCell : NeighboringCells)
	{
		if(!CurrentCell->bIsVisited)
			UnvisitedNeighbors.Add(CurrentCell);
	}
}
