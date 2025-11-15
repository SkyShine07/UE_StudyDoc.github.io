// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "GAS_Character.generated.h"

/**
 * 
 */
UCLASS()
class ABILITYSYSTEMEXTRATOOLKIT_API AGAS_Character : public ACharacter
{
	GENERATED_BODY()


public:
	
	AGAS_Character();

	UAbilitySystemComponent* GetAbilitySystemComponent() const;
	


	//******** 服务器和客户端初始化ASC中的actorInfo   *****************
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	
	void InitASCData();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_InitASCData();
	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category="GAS")
	UAbilitySystemComponent* ASC;

	UPROPERTY(EditDefaultsOnly,Category="GAS")
	EGameplayEffectReplicationMode ReplicationMode=EGameplayEffectReplicationMode::Mixed;
	
};
