// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GAS_Character.h"

#include "EnhanceAbilitySystemComponent.h"
#include "AttributeSets/ExampleAttributeSet.h"

AGAS_Character::AGAS_Character()
{

	ASC=CreateDefaultSubobject<UEnhanceAbilitySystemComponent>(FName("ASC"));
	ASC->SetReplicationMode(ReplicationMode);
	
	AS=CreateDefaultSubobject<UExampleAttributeSet>(FName("AS"));

	
}

UAbilitySystemComponent* AGAS_Character::GetAbilitySystemComponent() const
{
	return ASC;
}

void AGAS_Character::InitASCData()
{
	if (!ASC) return;
	
	ASC->InitAbilityActorInfo(this,this);

	// 调用蓝图版本
	BP_InitASCData();
	
}

void AGAS_Character::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	InitASCData();
	
}

void AGAS_Character::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();


	InitASCData();

	
}
