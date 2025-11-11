// Fill out your copyright notice in the Description page of Project Settings.


#include "ExampleAttributeSet.h"
#include "GameplayEffectExtension.h"




void UExampleAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	
	Super::PreAttributeChange(Attribute, NewValue);

	
	
}

void UExampleAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute==GetManaAttribute())
	{
		//
	}
	
	
}

void UExampleAttributeSet::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);

}
