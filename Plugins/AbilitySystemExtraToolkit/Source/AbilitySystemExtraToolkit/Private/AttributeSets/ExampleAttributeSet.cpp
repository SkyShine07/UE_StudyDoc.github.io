// Fill out your copyright notice in the Description page of Project Settings.



#include "AttributeSets/ExampleAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"


void UExampleAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	
	
	if (Attribute==GetHealthAttribute())
	{
		NewValue=FMath::Clamp(NewValue,0.0,GetMaxHealth());
	}
	
	if (Attribute==GetManaAttribute())
	{
		NewValue=FMath::Clamp(NewValue,0.0,GetMaxMana());
	}
	
	Super::PreAttributeChange(Attribute, NewValue);

	
}

void UExampleAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute==GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(),0.0,GetMaxMana()));
	}
	
	if (Data.EvaluatedData.Attribute==GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(),0.0,GetMaxHealth()));
	}
	
	
	//************** 处理伤害事件  *****************
	
	if (Data.EvaluatedData.Attribute == GetInComingDamageAttribute())
	{
	
	//Todo: 计算伤害
	
		float finalDamage=GetInComingDamage();
	
	// 将伤害值应用到生命值
		
	Health=FMath::Clamp(GetHealth()-finalDamage,0.0,GetMaxHealth());
	OnRecieveDamageValue.Broadcast(finalDamage);
		
		
	}
	
}

void UExampleAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass,Health,COND_None,REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass,MaxHealth,COND_None,REPNOTIFY_Always)
		
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass,Mana,COND_None,REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass,MaxMana,COND_None,REPNOTIFY_Always)
	
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass,AttackPower,COND_None,REPNOTIFY_Always)
	
}

void UExampleAttributeSet::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);

}

void UExampleAttributeSet::OnRep_Health(const FGameplayAttributeData& Data)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass,Health,Data);
	
}

void UExampleAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& Data)
{	
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass,MaxHealth,Data);
	
}

void UExampleAttributeSet::OnRep_Mana(const FGameplayAttributeData& Data)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass,Mana,Data);
	
}

void UExampleAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& Data)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass,MaxMana,Data);
	
}

void UExampleAttributeSet::OnRep_AttackPower(const FGameplayAttributeData& Data)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass,AttackPower,Data);
	
}
