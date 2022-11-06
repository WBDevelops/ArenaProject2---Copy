// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AICharacter.generated.h"

USTRUCT(BlueprintType)
struct FMyStats
{
	GENERATED_BODY()

	UPROPERTY()
		int Level;
	UPROPERTY()
		int HP;
	UPROPERTY()
		float AS;
	UPROPERTY()
		float MS;
	UPROPERTY()
		float Range;
	UPROPERTY()
		int Damage;
	UPROPERTY()
		int SDamage;
	UPROPERTY()
		int Deffence;
	UPROPERTY()
		float CDBase;

	FMyStats() {
		Level = 0;
		HP = 100; // Hit Points
		AS = 3; // How long between each attack.
		MS = 300; // movement Speed (scaling needs to be checked)
		Range = 100; // Range (scaling needs to be checked)
		Damage = 10; // Damage
		SDamage = 20; // Base Skill Damage
		Deffence = 0; // Base Deffence
		CDBase = 10; // Base Skill Cool Down
	}

	void SetStats(int hp, float as, float ms, float range, int Dmg, int SDmg, int Def, float cdBase) {
		HP = hp;
		AS = as;
		MS = ms;
		Range = range;
		Damage = Dmg;
		SDamage = SDmg;
		Deffence = Def;
		CDBase = cdBase;
	}
};

USTRUCT(BlueprintType)
struct FMyEffect
{
	GENERATED_BODY()

	UPROPERTY()
		float Duration;
	UPROPERTY()
		int ID;
	UPROPERTY()
		int Strength;

	FMyEffect() {
		Duration = 0.0f;
		ID = 0;
		Strength = 0;
	}

	void SetEffect(float duration, int id, int strength) {
		Duration = duration;
		ID = id;
		Strength = strength;
	}
};

USTRUCT(BlueprintType)
struct FMySkill
{
	GENERATED_BODY()

		UPROPERTY()
		FString Name;
	UPROPERTY()
		float CurrentCoolDown;
	UPROPERTY()
		float BaseCoolDown;
	UPROPERTY()
		float OverallCoolDownContribution;
	UPROPERTY()
		float RangeModifier;
	UPROPERTY()
		int BaseStrength;
	UPROPERTY()
		bool TargetSelf;
	UPROPERTY()
		bool IsTargetHealthRestriction;
	UPROPERTY()
		int TargetHealthRestriction;
	UPROPERTY()
		TArray<FMyEffect> SkillEffects;

	FMySkill() {
		Name = "Smack";
		BaseCoolDown = 1.0f;
		CurrentCoolDown = 0.0f;
		OverallCoolDownContribution = 1.0f;
		RangeModifier = 1.0f;
		BaseStrength = 1;
		TargetSelf = false;
		IsTargetHealthRestriction = false;
		TargetHealthRestriction = 0;
	}

	void SetSkill(FString name, float cooldown, float cooldownContribution, float rangeModifier, int baseStrength, bool targetSelf, bool isHPRestrict, int HPrestrict, TArray<FMyEffect> skillEffects) {
		Name = name;
		BaseCoolDown = cooldown;
		CurrentCoolDown = 0.0f;
		OverallCoolDownContribution = cooldownContribution;
		RangeModifier = rangeModifier;
		BaseStrength = baseStrength;
		IsTargetHealthRestriction = isHPRestrict;
		TargetHealthRestriction = HPrestrict;
		SkillEffects = skillEffects;
	}
};

UCLASS()
class ARENAPROJECT2_API AAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAICharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY()
		float OverallCoolDown;
	UPROPERTY()
		float DamageTick;

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
		FVector FindClosestEnemy();
	UFUNCTION()
		float MoveCloserTo(FVector TargetLocation);
	UFUNCTION()
		virtual void UseSkill(float Distance);
	UFUNCTION()
		void CheckStatus();
	UFUNCTION()
		void GenerateADDeffect(float Duration, int ID, int Strength);
	UFUNCTION()
		void ADDeffect(FMyEffect NewEffect);
	UFUNCTION()
		void GenerateADDskill(FString name, float cooldown, float cooldownContribution, float rangeModifier, int baseStrength, bool targetSelf, bool isHPRestrict, int HPrestrict, TArray<FMyEffect> skillEffects);
	UFUNCTION(BlueprintCallable, Category = "Health")
		int GetHP();
	UFUNCTION(BlueprintCallable, Category = "Health")
		int GetMaxHP();

	UPROPERTY()
		AActor* CurrentTarget;
	UPROPERTY(BlueprintReadWrite)
		FMyStats AICurrentStats;
	UPROPERTY()
		FMyStats AIStats;
	UPROPERTY()
		TArray<FMyEffect> CurrentEffects;
	UPROPERTY()
		TArray<FMySkill> Skills;
};

