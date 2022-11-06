// Fill out your copyright notice in the Description page of Project Settings.


#include "WarriorCharacter.h"

AWarriorCharacter::AWarriorCharacter() {
	SetStats(0);
}
void AWarriorCharacter::BeginPlay()
{
	Super::BeginPlay();
}
void AWarriorCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UseSkill(MoveCloserTo(FindClosestEnemy()));
	CheckStatus();
}
void AWarriorCharacter::SetStats(int Level) {
	AIStats.Level = Level;
	OverallCoolDown = 0.0f;
	for (int x = 0; x < Level * 3; x++) {
		switch (FMath::RandRange(0, 4)) {
		case 0:
			AIStats.HP += 30 * FMath::RandRange(1, 2);
			break;
		case 1:
			AIStats.Deffence += 5 * FMath::RandRange(1, 2);
			break;
		case 2:
			AIStats.Damage += 8 * FMath::RandRange(1, 2);
			break;
		case 3:
			AIStats.MS += 100.0f * FMath::RandRange(1, 2);
			break;
		case 4:
			AIStats.CDBase *= 0.8f;
		default:
			break;
		}
	}
	AICurrentStats = AIStats;
	for (int x = 0; x < Level + 1; x++) {
		if (Level >= 0) {
			TArray<FMyEffect> EmptyCase;
			switch (FMath::RandRange(0, 2)) {
			case 0:
				GenerateADDskill("Smack", 1.0f, 1.0f, 1.0f, 5, false, false, 0, EmptyCase);
				break;
			case 1:
				GenerateADDskill("Wack", 1.5f, 1.5f, 1.1f, 7, false, false, 0, EmptyCase);
				break;
			case 2:
				GenerateADDskill("Bash", 2.0f, 1.0f, 1.0f, 10, false, false, 0, EmptyCase);
				break;
			default:
				break;
			}
		}
	}
}


void AWarriorCharacter::UseSkill(float Distance) {
	if (CurrentTarget != NULL) {
		for (int x = 0; x < Skills.Num(); x++) {
			if (Skills[x].CurrentCoolDown * AICurrentStats.CDBase <= 0.0 && OverallCoolDown * AICurrentStats.CDBase * AICurrentStats.AS <= 0.0f) {
				if (Skills[x].TargetSelf) {
					if (!Skills[x].IsTargetHealthRestriction) {
						AICurrentStats.HP += Skills[x].BaseStrength * AICurrentStats.SDamage;
						if (AICurrentStats.HP > AIStats.HP) {
							AICurrentStats.HP = AIStats.HP;
						}
						for (int y = 0; y < Skills[x].SkillEffects.Num(); y++) {
							ADDeffect(Skills[x].SkillEffects[y]);
						}
						Skills[x].CurrentCoolDown = Skills[x].BaseCoolDown;
						OverallCoolDown += Skills[x].OverallCoolDownContribution;
					}
					else {
						if (AIStats.HP - AICurrentStats.HP > Skills[x].TargetHealthRestriction) {
							AICurrentStats.HP += Skills[x].BaseStrength * AICurrentStats.SDamage;
							if (AICurrentStats.HP > AIStats.HP) {
								AICurrentStats.HP = AIStats.HP;
							}
							for (int y = 0; y < Skills[x].SkillEffects.Num(); y++) {
								ADDeffect(Skills[x].SkillEffects[y]);
							}
							Skills[x].CurrentCoolDown = Skills[x].BaseCoolDown;
							OverallCoolDown += Skills[x].OverallCoolDownContribution;
						}
					}
				}
				else {
					AAICharacter* TargetCharacter = Cast<AAICharacter>(CurrentTarget);
					if (Skills[x].RangeModifier * AICurrentStats.Range >= Distance) {
						if (!Skills[x].IsTargetHealthRestriction) {
							TargetCharacter->AICurrentStats.HP -= Skills[x].BaseStrength + AICurrentStats.Damage;
							for (int y = 0; y < Skills[x].SkillEffects.Num(); y++) {
								TargetCharacter->ADDeffect(Skills[x].SkillEffects[y]);
							}
							if (GEngine) {
								GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::FromInt(TargetCharacter->AICurrentStats.HP));
							}
							Skills[x].CurrentCoolDown = Skills[x].BaseCoolDown;
							OverallCoolDown += Skills[x].OverallCoolDownContribution;
						}
						else {
							if (TargetCharacter->AICurrentStats.HP < Skills[x].TargetHealthRestriction) {
								TargetCharacter->AICurrentStats.HP -= Skills[x].BaseStrength + AICurrentStats.Damage;
								for (int y = 0; y < Skills[x].SkillEffects.Num(); y++) {
									TargetCharacter->ADDeffect(Skills[x].SkillEffects[y]);
								}
								Skills[x].CurrentCoolDown = Skills[x].BaseCoolDown;
								OverallCoolDown += Skills[x].OverallCoolDownContribution;
							}
						}
					}
				}
			}
			else {
				Skills[x].CurrentCoolDown -= FApp::GetDeltaTime();
				OverallCoolDown -= FApp::GetDeltaTime();
			}
		}
	}
}
