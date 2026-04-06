// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "Abilities/GameplayAbility.h"
#include "EnhanceGameplayAbility.generated.h"

/**
 * 
 */

UENUM()
enum class ESkillActivePocliy:uint8
{
	//主动激活，由玩家控制"
	Positive ,
	
	// 输入id激活技能
	InputId,
	
	//授予时激活
	OnGive 

};

UENUM(BlueprintType)
enum class EInputID :uint8
{
	SKill_1,
	SKill_2,
	SKill_3,
	SKill_4,
	SKill_5,
	SKill_6
	
};

UENUM()
enum class ESkillRemovePocliy :uint8
{
	
	//主动移除，由玩家控制"
	Positive ,

	//技能释放结束后自动移除
	OnSkillEnd ,

	//技能激活n次后自动移除
	OnSkillActiveCount 

};




USTRUCT(BlueprintType)
struct FAbilityConfigFragment
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	FGameplayTag Tag;
	
};

UE_DECLARE_GAMEPLAY_TAG_EXTERN(AbilityConfigFragmentTag_UI);

USTRUCT(BlueprintType)
struct FAbilityConfigFragment_UI:public FAbilityConfigFragment
{

	GENERATED_BODY()
	FAbilityConfigFragment_UI()
	{
		Tag = AbilityConfigFragmentTag_UI;
	};
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	FText Name;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FText Describle;
	
	
};



DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSKillRemoved);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSKillCommited);

UCLASS()
class ABILITYSYSTEMEXTRATOOLKIT_API UEnhanceGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()


public:

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TInstancedStruct<FAbilityConfigFragment>> AbilityConfigStructs;

	UPROPERTY(BlueprintAssignable)
	FOnSKillRemoved OnSKillRemoved; 

	UPROPERTY( BlueprintAssignable)
	FOnSKillCommited OnSKillCommited;
	
public:

	UFUNCTION(BlueprintCallable,BlueprintPure)
	bool IsShowInMenu(FAbilityConfigFragment_UI& AbilityConfigFragment_UI);

	
	UFUNCTION(BlueprintCallable, Category = "Cost")
	UGameplayEffect* GetCostEffect();

	UFUNCTION(BlueprintCallable)
	void levelUp(int32 AddToLevel=1);

	
	UFUNCTION(BlueprintPure)
	FORCEINLINE uint8  GetCurrentActiveAccount() const {return CurrentActiveAccount;} ;
	UFUNCTION(BlueprintPure)
	FORCEINLINE uint8  GetRemainActiveAccount() const {return ActiveAccount-CurrentActiveAccount;} ;
    
	/*
	UFUNCTION(BlueprintPure)
	bool NowCanActivateAbility() const ;
	*/
	
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual  void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual void OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	
	
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pocliy")
	ESkillActivePocliy SkillActivePocliy=ESkillActivePocliy::Positive;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pocliy",meta=(EditCondition="SkillActivePocliy==ESkillActivePocliy::InputId"))
	EInputID InputID;
	
	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pocliy")
	ESkillRemovePocliy SkillRemovePocliy=ESkillRemovePocliy::Positive;
	
	UPROPERTY(EditDefaultsOnly,Category = "Pocliy",meta=(EditCondition="SkillRemovePocliy==ESkillRemovePocliy::OnSkillActiveCount"))
	uint8 ActiveAccount=0;

private:
	
	
	uint8 CurrentActiveAccount=0;
	
	
};
