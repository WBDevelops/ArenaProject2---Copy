// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector AAICharacter::FindClosestEnemy() {
	FVector Origin = this->GetActorLocation();
	// Finds all instances of AAICharacters in the current level. 
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAICharacter::StaticClass(), FoundActors); 
	if (FoundActors.Num() > 1) {
		float ShortestDistance = 999999999.0f;
		for (int x = 0; x < FoundActors.Num(); x++) {
			if (FoundActors[x] != NULL && Cast<AAICharacter>(FoundActors[x]) != this) {
				if (FVector::Dist(FoundActors[x]->GetActorLocation(), Origin) < ShortestDistance) {
					ShortestDistance = FVector::Dist(FoundActors[x]->GetActorLocation(), Origin);
					CurrentTarget = FoundActors[x];
				}
			}
		}
		return CurrentTarget->GetActorLocation();
	}
	else {
		CurrentTarget = NULL;
		return Origin;
	}
	
}

float AAICharacter::MoveCloserTo(FVector TargetLocation) {
	float distance = FVector::Dist(this->GetActorLocation(), TargetLocation);
	if (distance > AICurrentStats.Range) {
		FVector Origin = this->GetActorLocation();
		FVector DirectionLocation = TargetLocation - Origin;
		DirectionLocation = DirectionLocation.GetSafeNormal();
		FVector LocationTemp = this->GetActorLocation();
		LocationTemp += DirectionLocation * AICurrentStats.MS * FApp::GetDeltaTime();
		this->SetActorLocation(LocationTemp);
	}
	return distance;
}

void AAICharacter::UseSkill(float Distance) {

}

void AAICharacter::GenerateADDeffect(float Duration, int ID, int Strength) {
	FMyEffect NewEffect;
	NewEffect.SetEffect(Duration, ID, Strength);
	CurrentEffects.Add(NewEffect);
}

void AAICharacter::ADDeffect(FMyEffect NewEffect) {
	CurrentEffects.Add(NewEffect);
}

void AAICharacter::GenerateADDskill(FString name, float cooldown, float cooldownContribution, float rangeModifier, int baseStrength, bool targetSelf, bool isHPRestrict, int HPrestrict, TArray<FMyEffect> skillEffects) {
	FMySkill NewSkill;
	NewSkill.SetSkill(name, cooldown, cooldownContribution, rangeModifier, baseStrength, targetSelf, isHPRestrict, HPrestrict, skillEffects);
	Skills.Add(NewSkill);
	
}

void AAICharacter::CheckStatus() {
	for (int x = 0; x < CurrentEffects.Num() - 1; x++) {
		CurrentEffects[x].Duration -= FApp::GetDeltaTime();
		DamageTick += FApp::GetDeltaTime();
		if (DamageTick >= 1.0f) {
			DamageTick = 0.0f;
			switch (CurrentEffects[x].ID) {
			case 1: // Tick Damage
				AICurrentStats.HP -= CurrentEffects[x].Strength;
				break;
			case 2: // Tick Heal
				AICurrentStats.HP += CurrentEffects[x].Strength;
				if (AICurrentStats.HP > AIStats.HP)
					AICurrentStats.HP = AIStats.HP;
				break;
			}
			if (CurrentEffects[x].Duration <= 0.0f) {
				switch (CurrentEffects[x].ID) {
				case 3: // Damage Effect End
					AICurrentStats.Damage = AIStats.Damage;
					break;
				case 4: // Deffence Effect End
					AICurrentStats.Deffence = AIStats.Deffence;
					break;
				case 5: // Speed Effect End
					AICurrentStats.MS = AIStats.MS;
					break;
				}
				CurrentEffects.RemoveAt(x);
			}
		}
	}
	if (AICurrentStats.HP <= 0) {
		Destroy();
	}
}

int AAICharacter::GetHP() {
	return AICurrentStats.HP;
}
int AAICharacter::GetMaxHP() {
	return AIStats.HP;
}