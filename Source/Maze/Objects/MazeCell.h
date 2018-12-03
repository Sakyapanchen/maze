// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MazeCell.generated.h"

/**
 * 
 */
UCLASS()
class MAZE_API UMazeCell : public UObject
{
	GENERATED_BODY()
	
public:

	UMazeCell(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadOnly, Category = "Maze Generator|Cell")
		bool bIsEmpty = false;
	UPROPERTY(BlueprintReadOnly, Category = "Maze Generator|Cell")
		bool bIsVisited = false;
	UPROPERTY(BlueprintReadOnly, Category = "Maze Generator|Cell")
		FVector Location;
	UPROPERTY(BlueprintReadOnly, Category = "Maze Generator|Cell")
		TArray<UMazeCell *>NeighboringCells;
};
