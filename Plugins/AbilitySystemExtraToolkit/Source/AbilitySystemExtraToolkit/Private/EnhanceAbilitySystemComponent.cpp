// Fill out your copyright notice in the Description page of Project Settings.


#include "EnhanceAbilitySystemComponent.h"
#include "GameplayAbility/EnhanceGameplayAbility.h"


void UEnhanceAbilitySystemComponent::BindCallBack()
{
	AbilityCommittedCallbacks.AddLambda([this](UGameplayAbility* Ability)
	{
		OnAbilityCommited.Broadcast(Ability->GetCurrentAbilitySpecHandle());
		
	});
	
	AbilitySpecDirtiedCallbacks.AddLambda([this](const FGameplayAbilitySpec& Spec)
	{
		OnAbilitySpecDirty.Broadcast(Spec.Handle);
		
		
	});
	
	
	
}

UEnhanceAbilitySystemComponent::UEnhanceAbilitySystemComponent()
{

	PrimaryComponentTick.bCanEverTick = true;
	
	BindCallBack();

}

void UEnhanceAbilitySystemComponent::levelUp(const FGameplayAbilitySpecHandle& AbilitySpecHandle, int32 AddToLevel)
{
	FGameplayAbilitySpec* TargetAbilitySpec = FindAbilitySpecFromHandle(AbilitySpecHandle);
	
	if(!TargetAbilitySpec) return ;
		
	TargetAbilitySpec->Level+=AddToLevel;
	MarkAbilitySpecDirty(*TargetAbilitySpec);
		
	OnAbilityLevelUp.Broadcast(TargetAbilitySpec->Level);
	
}

void UEnhanceAbilitySystemComponent::OnGiveAbility(FGameplayAbilitySpec& AbilitySpec)
{
	Super::OnGiveAbility(AbilitySpec);
	
	OnAbilityGived.Broadcast(AbilitySpec.Handle);
	
	
}

void UEnhanceAbilitySystemComponent::OnRemoveAbility(FGameplayAbilitySpec& AbilitySpec)
{
	Super::OnRemoveAbility(AbilitySpec);
	
	OnAbilityRemoved.Broadcast(AbilitySpec.Handle);
	
}




