// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbility/EnhanceGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "EnhanceAbilitySystemComponent.h"

UGameplayEffect* UEnhanceGameplayAbility::GetCostEffect()
{
	return GetCostGameplayEffect();
}

void UEnhanceGameplayAbility::levelUp(int32 AddToLevel)
{
	UEnhanceAbilitySystemComponent* ASC=Cast<UEnhanceAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo_Ensured());
	if (ASC)
	{
		ASC->levelUp(GetCurrentAbilitySpec()->Handle,AddToLevel);
	}
}

void UEnhanceGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo,
                                            const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	
	if (SkillActivePocliy==ESkillActivePocliy::OnGive)
	{
		GetAbilitySystemComponentFromActorInfo_Ensured()->TryActivateAbility(GetCurrentAbilitySpecHandle());
	}

	if (SkillActivePocliy==ESkillActivePocliy::InputId && GetCurrentAbilitySpec())
	{
		GetCurrentAbilitySpec()->InputID=StaticCast<int32>(InputID);
	}
	
	
}

void UEnhanceGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	CurrentActiveAccount++;
	
	if (SkillRemovePocliy==ESkillRemovePocliy::OnSkillEnd)
	{
		GetAbilitySystemComponentFromActorInfo_Ensured()->ClearAbility(GetCurrentAbilitySpecHandle());
	}
	
	
	// 技能逻辑结束后，判断是否移除技能
	if (SkillRemovePocliy==ESkillRemovePocliy::OnSkillActiveCount
		&& GetInstancingPolicy()==EGameplayAbilityInstancingPolicy::InstancedPerActor
		&& CurrentActiveAccount==ActiveAccount)
	{
		
		GetAbilitySystemComponentFromActorInfo_Ensured()->ClearAbility(GetCurrentAbilitySpecHandle());
	}
	
	
}

void UEnhanceGameplayAbility::CommitExecute(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{

	Super::CommitExecute(Handle, ActorInfo, ActivationInfo);
	
	OnSKillCommited.Broadcast();
	
	
}

void UEnhanceGameplayAbility::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilitySpec& Spec)
{
	

	Super::OnRemoveAbility(ActorInfo, Spec);
	
	OnSKillRemoved.Broadcast();
	
}
