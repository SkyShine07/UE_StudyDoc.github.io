// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "ExampleAttributeSet.generated.h"

/**
 * 
 */

	#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class ABILITYSYSTEMEXTRATOOLKIT_API UExampleAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	ATTRIBUTE_ACCESSORS(ThisClass,Health);
	ATTRIBUTE_ACCESSORS(ThisClass,Mana);
	ATTRIBUTE_ACCESSORS(ThisClass,MaxHealth);
	ATTRIBUTE_ACCESSORS(ThisClass,MaxMana);
	ATTRIBUTE_ACCESSORS(ThisClass,InComingDamage);


	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	virtual void Serialize(FArchive& Ar) override;
protected:

	UPROPERTY()
	FGameplayAttributeData Health=100;
	UPROPERTY()
	FGameplayAttributeData MaxHealth=100;
	
	UPROPERTY()
	FGameplayAttributeData Mana=100;
	UPROPERTY()
	FGameplayAttributeData MaxMana=100;

/*
 * ****************m元数据***********************
 */
	FGameplayAttributeData InComingDamage;

	
};
