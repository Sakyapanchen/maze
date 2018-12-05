// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MazeCell.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ENeighborCellPosition : uint8
{
	NPE_Up		UMETA(DisplayName = "Up"),
	NPE_Down 	UMETA(DisplayName = "Down"),
	NPE_Right	UMETA(DisplayName = "Right"),
	NPE_Left	UMETA(DisplayName = "Left"),
};


UCLASS()
class MAZE_API UMazeCell : public UObject
{
	GENERATED_BODY()
	
public:

	UMazeCell(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
		void GetUnvisitedNeighbors(TArray < UMazeCell *> & UnvisitedNeighbors);


	UPROPERTY(BlueprintReadOnly, Category = "Maze Generator|Cell")
		bool bIsEmpty = false;
	UPROPERTY(BlueprintReadOnly, Category = "Maze Generator|Cell")
		bool bIsVisited = false;
	UPROPERTY(BlueprintReadOnly, Category = "Maze Generator|Cell")
		int32 Row;
	UPROPERTY(BlueprintReadOnly, Category = "Maze Generator|Cell")
		int32 Column;
	UPROPERTY(BlueprintReadOnly, Category = "Maze Generator|Cell")
		TArray<UMazeCell *>NeighboringCells;
};
