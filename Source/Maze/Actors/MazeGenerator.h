// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Objects/MazeCell.h"
#include "Runtime/Engine/Classes/Components/InstancedStaticMeshComponent.h"
#include "MazeGenerator.generated.h"

USTRUCT(BlueprintType)
struct FCellsRow
{
	GENERATED_USTRUCT_BODY();
	TArray<UMazeCell*> Cells;
};


UCLASS()
class MAZE_API AMazeGenerator : public AActor
{
	GENERATED_BODY()
	
public:	

	AMazeGenerator(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, Category = "Settings", meta = (ClampMin = "2", UIMin = "2"))
		int32 Width;
	UPROPERTY(EditDefaultsOnly, Category = "Settings", meta = (ClampMin = "2", UIMin = "2"))
		int32 Height;
	UPROPERTY(EditDefaultsOnly, Category = "Settings", meta = (ClampMin = "0.0", UIMin = "0.0"))
		float Step;
	UPROPERTY(BlueprintReadOnly, Category = "Maze Generator")
		TArray<FCellsRow> MazeCellRows;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Maze Generator",  meta = (AllowPrivateAccess = "true"))
		UInstancedStaticMeshComponent* MazeWallCellMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Maze Generator", meta = (AllowPrivateAccess = "true"))
		USceneComponent* Root;

};
