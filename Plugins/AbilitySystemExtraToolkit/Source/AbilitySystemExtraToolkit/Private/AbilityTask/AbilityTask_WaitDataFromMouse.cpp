// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityTask/AbilityTask_WaitDataFromMouse.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemExtraToolKitDebug.h"
#include "Net/UnrealNetwork.h"

UAbilityTask_WaitDataFromMouse* UAbilityTask_WaitDataFromMouse::WaitDataFromHitResultUnderCursor(UGameplayAbility* OwningAbility,TEnumAsByte<EQueryType>QueryType,
		ECollisionChannel CollisionChannel, const TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypeQuery)
{
	UAbilityTask_WaitDataFromMouse*  obj=NewAbilityTask<UAbilityTask_WaitDataFromMouse>(OwningAbility);

	obj->ObjectTypeQuery=ObjectTypeQuery;
	obj->CollisionChannel=CollisionChannel;
	obj->QueryType=QueryType;
	
	return obj;
}

FGameplayAbilityTargetDataHandle  UAbilityTask_WaitDataFromMouse::GetHitResultUnderCursor()
{

	FGameplayAbilityTargetDataHandle LocalTargetDataHandle;
	FHitResult HitResult;
	TWeakObjectPtr<APlayerController> PlayerController;
	
	// 得到cursor的击中结果
	
	if (Ability&&Ability->GetCurrentActorInfo())
	{
		PlayerController=Ability->GetCurrentActorInfo()->PlayerController;
	}

	checkf(PlayerController.Get(),TEXT("PlayerController is inVailid  Line: %d,you Maybe Not Init ActorInfo In AbilitySystemComponent!!"  ),__LINE__);
	
	if (!PlayerController.Get())
	{
		return LocalTargetDataHandle;
	}


	switch (QueryType)
	{
		case EQueryType::EQT_CollisionChannel:
		
		PlayerController->GetHitResultUnderCursor(CollisionChannel,false,HitResult);;
		break;

	case EQueryType::ObjectTypeQuery:
		PlayerController->GetHitResultUnderCursorForObjects(ObjectTypeQuery,false,HitResult);
		break;
		
	default:
		
		PlayerController->GetHitResultUnderCursor(CollisionChannel,false,HitResult);
		break;
	}
	
	
	if (!HitResult.IsValidBlockingHit())
	{
		ASDebug::PrintText(TEXT("No Find Target To Make TargetData"));
		return LocalTargetDataHandle;
	};
	
	LocalTargetDataHandle=FGameplayAbilityTargetDataHandle(new FGameplayAbilityTargetData_SingleTargetHit(HitResult));
	
	return LocalTargetDataHandle;
	
}


void UAbilityTask_WaitDataFromMouse::Activate()
{
	Super::Activate();

	FScopedPredictionWindow ScopedPredictionWindow(AbilitySystemComponent.Get());
		
	
	FGameplayAbilityActivationInfo ActivationInfo=Ability->GetCurrentActivationInfo();
	bool bHasAuthorityOrPredictionKey=Ability->HasAuthorityOrPredictionKey(Ability->GetCurrentActorInfo(),&ActivationInfo);
	const FGameplayAbilitySpecHandle AbilitySpecHandle=Ability->GetCurrentAbilitySpecHandle();
	
	
	// 本地客户端发送TargetData
	if (Ability->IsLocallyControlled())
	{
		
		FGameplayAbilityTargetDataHandle CacheTargetDataHandle;
	
		CacheTargetDataHandle=GetHitResultUnderCursor();
	
		AbilitySystemComponent.Get()->CallServerSetReplicatedTargetData(AbilitySpecHandle,
			ActivationInfo.GetActivationPredictionKey(),
			CacheTargetDataHandle,
			FGameplayTag(),
			AbilitySystemComponent->ScopedPredictionKey);

		/*AbilitySystemComponent->ServerSetReplicatedTargetData(AbilitySpecHandle,
			ActivationInfo.GetActivationPredictionKey(),
			CacheTargetDataHandle,
			FGameplayTag(),
			AbilitySystemComponent->ScopedPredictionKey);*/

		ASDebug::PrintText(TEXT("客户端 :发送TargetData,数量：%d"),CacheTargetDataHandle.Data.Num());
		
		
	}
	
	// 服务器接受Data

	
	if (bHasAuthorityOrPredictionKey)
	{

		// 首先绑定监听TargetData的委托
		AbilitySystemComponent->AbilityTargetDataSetDelegate(AbilitySpecHandle,ActivationInfo.GetActivationPredictionKey()).AddLambda([this
			](const FGameplayAbilityTargetDataHandle& InTargetDataHandle ,FGameplayTag Tag)
		{
			// 广播数据已经到达
			OnTargetDataValid.Broadcast(InTargetDataHandle);
			
			ASDebug::PrintText(TEXT("服务器 :服务器等到了TargetData,数量：%d"),InTargetDataHandle.Data.Num());
			
		});


		// 如果服务器的TargetData已经到达，立即广播委托（执行到这里TargetData已经设置好了，直接广播CallBack）
		/*
		 * 两种情况：
		 * 1.服务器执行到这里，TargetData已经先到了。由于在前面先绑定了我们监听的委托，我们自己手动调用这里CallReplicatedTargetDataDelegatesIfSet（）会调用所有绑定的回调函数，
		 * 包括前面我们自己写。
		 * 2..服务器执行到这里，TargetData还未到达服务器。只需要绑定好回调函数，当TargetData到达时，自动调用。
		 */
		bool bHasSet=AbilitySystemComponent->CallReplicatedTargetDataDelegatesIfSet(Ability->GetCurrentAbilitySpecHandle(),ActivationInfo.GetActivationPredictionKey());
		if (bHasSet)
		{
			ASDebug::PrintText(TEXT("服务器 :太速度了，执行到这里TargetData已经设置好了，主动广播CallBack"));
		}
		
		
	}
	
}

void UAbilityTask_WaitDataFromMouse::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	/*
	DOREPLIFETIME(ThisClass,TargetDataHandle);
	*/
	

	
}
