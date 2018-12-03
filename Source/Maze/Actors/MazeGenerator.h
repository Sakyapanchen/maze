// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MazeGenerator.generated.h"

UCLASS()
class MAZE_API AMazeGenerator : public AActor
{
	GENERATED_BODY()
	
public:	

	AMazeGenerator();

	UPROPERTY(EditDefaultsOnly, Category = "Settings", meta = (ClampMin = "2", UIMin = "2"))
		int32 Width;
	UPROPERTY(EditDefaultsOnly, Category = "Settings", meta = (ClampMin = "2", UIMin = "2"))
		int32 Height;
	UPROPERTY(EditDefaultsOnly, Category = "Settings", meta = (ClampMin = "0.0", UIMin = "0.0"))
		float Step;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
