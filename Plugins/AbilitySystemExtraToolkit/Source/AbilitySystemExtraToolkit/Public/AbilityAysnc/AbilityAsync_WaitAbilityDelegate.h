// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Async/AbilityAsync.h"
#include "AbilityAsync_WaitAbilityDelegate.generated.h"




#define UE_API GAMEPLAYABILITIES_API

/**
 * 
 */
struct FGameplayAbilitySpecHandle;
class UGameplayAbility;
class UAbilitySystemComponent;
/**
 * 
 */

UENUM(BlueprintType)
enum class  EAbilityQueryType:uint8
{

	AbilitySpecHanlde,

	AbilityInputID,
	
	AbilityClass
	
	
};


USTRUCT(BlueprintType)
struct FAbilityInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString name=" ";
	
	UPROPERTY(BlueprintReadOnly)
	int32 level=1;

	UPROPERTY(BlueprintReadWrite)
	float CoolDownRemain=0;
	
	UPROPERTY(BlueprintReadOnly)
	float CoolDownDuration=0;

	UPROPERTY(BlueprintReadOnly)
	FGameplayAbilitySpecHandle AbilitySpecHandle;

	UPROPERTY(BlueprintReadOnly)
	UAbilitySystemComponent* ASC;
	
	UPROPERTY(BlueprintReadOnly)
	TMap<FGameplayAttribute,float> Costs{};
	
	
	
};





UCLASS()
class   UAbilityAsync_WaitAbilityDelegate : public UAbilityAsync
{
	GENERATED_BODY()


public:



	UFUNCTION(BlueprintCallable, Category = "Ability|Async",meta = (DefaultToSelf = "TargetActor", BlueprintInternalUseOnly = "TRUE",DisplayName="ListenForGameplayAbilityInfo"))
	static UAbilityAsync_WaitAbilityDelegate* ListenForGameplayAbilityDelegate(AActor* TargetActor, EAbilityQueryType AbilityQueryType,
														  FGameplayAbilitySpecHandle AbilitySpecHandle, int32 AbilityInputID,
														  TSubclassOf<UGameplayAbility> AbilityClass,float CoolDownUpdateInterval);

	virtual void Activate() override;
	virtual void EndAction() override;
	

protected:

	/*
	* **********辅助函数*******************
	*/

	FGameplayAbilitySpec* GetCurrentAbilitySpec();
	FAbilityInfo  GetCurrentAbilityInfo();
	void BindCallBack();


	/*********
 * ***-----------------能力委托------------------------------
 */

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityDelegate,FAbilityInfo,info);


	UPROPERTY(BlueprintAssignable)
	FOnAbilityDelegate OnAbilityActive;

	UPROPERTY(BlueprintAssignable)
	FOnAbilityDelegate OnAbilityCommit;

	UPROPERTY(BlueprintAssignable)
	FOnAbilityDelegate AbilitySpecDirtied;

	UPROPERTY(BlueprintAssignable)
	FOnAbilityDelegate OnAbilityEnd;

	UPROPERTY(BlueprintAssignable)
	FOnAbilityDelegate OnAbilityFail;


	FDelegateHandle AbilitySpecDirtiedHandle;
	FDelegateHandle OnAbilityActiveHandle;
	FDelegateHandle OnAbilityCommitHandle;
	FDelegateHandle OnAbilityEndle;
	FDelegateHandle OnAbilityFailHandle;

	/*
	 * ****************升级***************
	 */

	
	UFUNCTION(BlueprintCallable)
	void levelUp(int32 AddToLevel)
	{
		
		if(!OwnAbilitySpec) return ;
		OwnAbilitySpec->Level+=AddToLevel;
		GetAbilitySystemComponent()->MarkAbilitySpecDirty(*OwnAbilitySpec);
		
	};

	
	/*
	 * 动态监听能力的cooldown变化
	 *
	 * 注意，要使用TimerManager，必要要将对象注册到Gameinstance中
	 */

	//更新cooldown的剩余时间

	UPROPERTY(BlueprintAssignable)
	FOnAbilityDelegate OnAbilityCoolDownUpdate;

	FTimerHandle TimerHandle;

	float CoolDownUpdateInterval;

	UFUNCTION()
	void OnStartCoolDown();
	
	
	/*
	 * *************************** 成员变量 ***************************
	 */
	
	UPROPERTY()
	EAbilityQueryType AbilityQueryType;

	
	UPROPERTY()
	FGameplayAbilitySpecHandle AbilitySpecHandle;


	UPROPERTY()
	int8 AbilityInputID;

	UPROPERTY()
	TSubclassOf<UGameplayAbility> AbilityClass;



private:

	
	FGameplayAbilitySpec* OwnAbilitySpec;
	
	
};
