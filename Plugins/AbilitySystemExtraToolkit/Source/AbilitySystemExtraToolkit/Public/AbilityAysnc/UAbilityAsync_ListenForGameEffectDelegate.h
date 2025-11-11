// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Abilities/Async/AbilityAsync.h"
#include "UAbilityAsync_ListenForGameEffectDelegate.generated.h"

USTRUCT(BlueprintType)
struct FActiveEffectInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FGameplayTagContainer EffectSpecAssetTags;

	UPROPERTY(BlueprintReadWrite)
	FGameplayTagContainer EffectSpecGrantedTags;
		
	UPROPERTY(BlueprintReadWrite)
	bool  SuccessfullyApplied;

	UPROPERTY(BlueprintReadWrite)
	float   Remain;


	// TimeChangeDelegate 更新的是新的总持续时间
	UPROPERTY(BlueprintReadWrite)
	float   Duration;

	UPROPERTY(BlueprintReadWrite)
	float   Period;

	UPROPERTY(BlueprintReadWrite)
	int32   Stack;

	UPROPERTY(BlueprintReadWrite)
	bool bIsInhibited;

	UPROPERTY(BlueprintReadWrite)
	FActiveGameplayEffectHandle    ActiveGameplayEffectHandle;
	
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEffectAppied ,const FActiveEffectInfo,ActiveEffectInfo);

UCLASS()
class ABILITYSYSTEMEXTRATOOLKIT_API UUAbilityAsync_ListenForGameEffectDelegate : public UAbilityAsync
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category = "Ability|Async",meta = (DefaultToSelf = "listerActor", BlueprintInternalUseOnly = "TRUE",DisplayName="ListenForAllGameEffectAppliedEvent"))
	static UUAbilityAsync_ListenForGameEffectDelegate* ListenForGameEffectDelegate(AActor* listerActor);


	
	void BindCallBack();
	void BindCallBackAfterAppied(FActiveEffectInfo  ActiveEffectInfo,FActiveGameplayEffectHandle& ActiveGameplayEffectHandle);
	

	
	
	virtual  void Activate() override;
	virtual  void EndAction() override;



	UPROPERTY(BlueprintAssignable)
	FOnEffectAppied OnEffectAppiedToSelf;
	
	UPROPERTY(BlueprintAssignable)
	FOnEffectAppied OnImmunityBlockGameplayEffect;

	UPROPERTY(BlueprintAssignable)
	FOnEffectAppied OnActiveGameplayEffectAdded;

	UPROPERTY(BlueprintAssignable)
	FOnEffectAppied OnPeriodicGameplayEffectExecute;

	UPROPERTY(BlueprintAssignable)
	FOnEffectAppied OnActiveGameplayEffectRemoved_Info;

	UPROPERTY(BlueprintAssignable)
	FOnEffectAppied OnGameplayEffectStackChangeDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnEffectAppied OnGameplayEffectTimeChangeDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnEffectAppied OnGameplayEffectInhibitionChangedDelegate;


	
	
	
};

UCLASS()
class ABILITYSYSTEMEXTRATOOLKIT_API UUAbilityAsync_ListenForDuraingGameEffectDelegate : public UAbilityAsync
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Ability|Async",meta = (DefaultToSelf = "listerActor", BlueprintInternalUseOnly = "TRUE",DisplayName="ListenForDuartingGameEffectEvent"))
	static UUAbilityAsync_ListenForDuraingGameEffectDelegate* ListenForDuartingGameEffectDelegate(AActor* listerActor,FGameplayTag listen_GE_GrantedTag);


	void GetActiveEffectEvents();
	void BindCallBack();

	FActiveEffectInfo UpdateActiveEffectInfo(UAbilitySystemComponent* ASC,FActiveGameplayEffectHandle handle,const FGameplayEffectSpec& spec );

	virtual  void Activate() override;
	virtual  void EndAction() override;

protected:

	UPROPERTY(BlueprintAssignable)
	FOnEffectAppied OnEffectAdded ;


	UPROPERTY(BlueprintAssignable)
	FOnEffectAppied OnEffectRemoved;

	UPROPERTY(BlueprintAssignable)
	FOnEffectAppied OnInhibitionChanged;


	UPROPERTY(BlueprintAssignable)
	FOnEffectAppied OnStackChanged;

	UPROPERTY(BlueprintAssignable)
	FOnEffectAppied OnTimeChanged;

private:
	
	FGameplayTag listen_GE_GrantedTag;

	FActiveGameplayEffectEvents* ActiveGameplayEffectEvents;
	
};