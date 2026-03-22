// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "EnhanceAbilitySystemComponent.generated.h"


class UEnhanceGameplayAbility;



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCommonAbilityDelegate,FGameplayAbilitySpecHandle,SpecHandle);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityLevelUpDelegate,uint32,NewLevel);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ABILITYSYSTEMEXTRATOOLKIT_API UEnhanceAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	UEnhanceAbilitySystemComponent();
	
	void BindCallBack();
	
	//技能升级
	UFUNCTION(BlueprintCallable)
	void levelUp(const FGameplayAbilitySpecHandle& AbilitySpecHandle, int32 AddToLevel=1);

	
	virtual void OnGiveAbility(FGameplayAbilitySpec& AbilitySpec) override;
	
	virtual void OnRemoveAbility(FGameplayAbilitySpec& AbilitySpec) override;

public:
	
	
	UPROPERTY(BlueprintAssignable)
	FCommonAbilityDelegate OnAbilityGived;

	UPROPERTY(BlueprintAssignable)
	FCommonAbilityDelegate OnAbilityRemoved;

	UPROPERTY(BlueprintAssignable,Blueprintable)
	FCommonAbilityDelegate OnAbilityCommited;


	UPROPERTY(BlueprintAssignable)
	FCommonAbilityDelegate OnAbilitySpecDirty;

	
	UPROPERTY(BlueprintAssignable)
	FOnAbilityLevelUpDelegate OnAbilityLevelUp;

};
