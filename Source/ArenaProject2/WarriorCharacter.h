// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AICharacter.h"
#include "WarriorCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ARENAPROJECT2_API AWarriorCharacter : public AAICharacter
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	AWarriorCharacter();
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void UseSkill(float Distance);
	void SetStats(int Level);
};
