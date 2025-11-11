// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_WaitDataFromMouse.generated.h"

/**
 * 
 */

UENUM()
enum EQueryType:uint8
{
	EQT_CollisionChannel,
	ObjectTypeQuery
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetDataValid,const FGameplayAbilityTargetDataHandle,TargetData);
UCLASS()
class ABILITYSYSTEMEXTRATOOLKIT_API UAbilityTask_WaitDataFromMouse : public UAbilityTask
{
	GENERATED_BODY()


public:


	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAbilityTask_WaitDataFromMouse* WaitDataFromHitResultUnderCursor(UGameplayAbility* OwningAbility,TEnumAsByte<EQueryType>QueryType,
		ECollisionChannel CollisionChannel, const TArray<TEnumAsByte<EObjectTypeQuery> >ObjectTypeQuery);



	virtual void Activate() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	FGameplayAbilityTargetDataHandle  GetHitResultUnderCursor();

	
protected:

	UPROPERTY(BlueprintAssignable)
	FOnTargetDataValid OnTargetDataValid;
	
	TArray<TEnumAsByte<EObjectTypeQuery> >ObjectTypeQuery ;
	ECollisionChannel CollisionChannel;
	
	EQueryType QueryType;
};
