// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Objects/MazeCell.h"
#include "Runtime/Engine/Classes/Components/InstancedStaticMeshComponent.h"
#include "MazeGenerator.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE (FMazeGenerationResult);

USTRUCT(BlueprintType)
struct FCellsRow
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UMazeCell*> Cells;
};


UCLASS()
class MAZE_API AMazeGenerator : public AActor
{
	GENERATED_BODY()
	
public:	

	AMazeGenerator(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, Category = "Settings", meta = (DisplayName = "Width", ClampMin = "2", UIMin = "2"))
		int32 PassWidth = 2;
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (DisplayName = "Height", ClampMin = "2", UIMin = "2"))
		int32 PassHeight = 2;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", meta = (ClampMin = "0.0", UIMin = "0.0"))
		float Step;
	UPROPERTY(BlueprintReadOnly, Category = "Maze Generator")
		TArray<FCellsRow> MazeCellRows;

	UPROPERTY(BlueprintReadOnly, Category = "Maze Generator")
		int32 Width = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Maze Generator")
		int32 Height = 0;
	UPROPERTY(BlueprintAssignable, Category = "Maze Generator")
		FMazeGenerationResult OnMazeGenerationEnd;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Maze Generator")
		void GenerateMaze(bool & success);

private:

	UFUNCTION(CallInEditor, Category = "Maze Generator")
		void GenerateSourceMatrix(bool & success);
	UFUNCTION(CallInEditor, Category = "Maze Generator")
		void RemoveWall(UMazeCell * CurrentCell, UMazeCell * NextCell);
	UFUNCTION(CallInEditor, Category = "Maze Generator")
		void CreatePass();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Maze Generator",  meta = (AllowPrivateAccess = "true"))
		UInstancedStaticMeshComponent* MazeWallCellMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Maze Generator", meta = (AllowPrivateAccess = "true"))
		UInstancedStaticMeshComponent* MazeFloorCellMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Maze Generator", meta = (AllowPrivateAccess = "true"))
		USceneComponent* Root;

};
