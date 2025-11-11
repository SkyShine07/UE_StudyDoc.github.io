// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "ATargetActor_BlueprintBase.generated.h"

/**
 * 
 */
UCLASS()
class ABILITYSYSTEMEXTRATOOLKIT_API ATargetActor_BlueprintBase : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()


public:

	ATargetActor_BlueprintBase();

	//限制用户确认目标时，ConfirmTargetingAndContinue()的调用条件
	UFUNCTION(BlueprintImplementableEvent,BlueprintPure)
	bool BP_IsConfirmTargetingAllowed();

	//TargetActor被abilityTask_waitTargetData生成之后就调用
	UFUNCTION(BlueprintImplementableEvent)
	void BP_StartTargeting(UGameplayAbility* Ability);

	//用户确认目标时调用
	UFUNCTION(BlueprintImplementableEvent)
	void BP_ConfirmTargetingAndContinue();
	
	//用户取消目标时调用
	UFUNCTION(BlueprintImplementableEvent)
	void BP_CancelTargetinge();
	



	virtual void Tick(float DeltaSeconds) override;
	
	virtual bool IsConfirmTargetingAllowed() override;
	virtual void StartTargeting(UGameplayAbility* Ability) override;
	virtual void ConfirmTargetingAndContinue() override;
	virtual void CancelTargeting() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>&) const override;
protected:


	//向服务器发送TargetData数据
	UFUNCTION(BlueprintCallable)
	void TargetDataReady(const FGameplayAbilityTargetDataHandle& TargetDataHandle);
	
	UPROPERTY(Replicated,EditAnywhere,Category=TargetActor,BlueprintReadOnly,meta=(ExposeOnSpawn))
	bool bSetLoactionUnderCursorHit=true;



private:
	
	void SetActorLocationUndercursorHit();

	
};
