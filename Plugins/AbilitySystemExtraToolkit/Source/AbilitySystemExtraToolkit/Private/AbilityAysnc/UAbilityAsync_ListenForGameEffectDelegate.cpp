// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityAysnc/UAbilityAsync_ListenForGameEffectDelegate.h"

#include "AbilitySystemComponent.h"

UUAbilityAsync_ListenForGameEffectDelegate* UUAbilityAsync_ListenForGameEffectDelegate::ListenForGameEffectDelegate(
	AActor* listerActor)
{

	UUAbilityAsync_ListenForGameEffectDelegate* obj=NewObject<UUAbilityAsync_ListenForGameEffectDelegate>();
	obj->SetAbilityActor(listerActor);
	
	obj->RegisterWithGameInstance(listerActor);
	
	return obj;
	
}



void UUAbilityAsync_ListenForGameEffectDelegate::BindCallBack()
{
	
	GetAbilitySystemComponent()->OnGameplayEffectAppliedDelegateToSelf.AddLambda([this](UAbilitySystemComponent* ASC,
		const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
		{
		
		FActiveEffectInfo ActiveEffectInfo{};
		
		EffectSpec.GetAllAssetTags(ActiveEffectInfo.EffectSpecAssetTags);
		EffectSpec.GetAllGrantedTags(ActiveEffectInfo.EffectSpecGrantedTags);
		ActiveEffectInfo.SuccessfullyApplied=ActiveGameplayEffectHandle.WasSuccessfullyApplied();
		ActiveEffectInfo.Duration=EffectSpec.Duration;
		ActiveEffectInfo.ActiveGameplayEffectHandle=ActiveGameplayEffectHandle;


		
		const FActiveGameplayEffect* ActiveEffect= ASC->GetActiveGameplayEffect(ActiveGameplayEffectHandle);
			if (ActiveEffect && GetWorld())
			{
			ActiveEffectInfo.Remain=ActiveEffect->GetTimeRemaining(GetWorld()->GetTime().GetWorldTimeSeconds());
			ActiveEffectInfo.Period=ActiveEffect->GetPeriod();
		
			}
			else
			{
			ActiveEffectInfo.Period=ActiveEffectInfo.Remain=0;
			}

		
			OnEffectAppiedToSelf.Broadcast(ActiveEffectInfo);
		
		// 绑定GE移除，刷新等委托

		BindCallBackAfterAppied(ActiveEffectInfo,ActiveEffectInfo.ActiveGameplayEffectHandle);
	
		

		
		});

	
	

	

		GetAbilitySystemComponent()->OnImmunityBlockGameplayEffectDelegate.AddLambda([this](const FGameplayEffectSpec& EffectSpec,
		const FActiveGameplayEffect* ActiveGameplayEffect)
		{
		
		FActiveEffectInfo ActiveEffectInfo{};
				
		EffectSpec.GetAllAssetTags(ActiveEffectInfo.EffectSpecAssetTags);
		EffectSpec.GetAllGrantedTags(ActiveEffectInfo.EffectSpecGrantedTags);
		ActiveEffectInfo.SuccessfullyApplied=ActiveGameplayEffect->Handle.WasSuccessfullyApplied();
		ActiveEffectInfo.Duration=EffectSpec.Duration;
		ActiveEffectInfo.ActiveGameplayEffectHandle=ActiveGameplayEffect->Handle;
		
	
		ActiveEffectInfo.Period=ActiveEffectInfo.Remain=0;
		OnImmunityBlockGameplayEffect.Broadcast(ActiveEffectInfo);
		
		});


		GetAbilitySystemComponent()->OnActiveGameplayEffectAddedDelegateToSelf.AddLambda([this](UAbilitySystemComponent*ASC,
		const FGameplayEffectSpec&EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
		{
		FActiveEffectInfo ActiveEffectInfo{};
		
		
		EffectSpec.GetAllAssetTags(ActiveEffectInfo.EffectSpecAssetTags);
		EffectSpec.GetAllGrantedTags(ActiveEffectInfo.EffectSpecGrantedTags);
		ActiveEffectInfo.SuccessfullyApplied=ActiveGameplayEffectHandle.WasSuccessfullyApplied();
		ActiveEffectInfo.Duration=EffectSpec.Duration;
		ActiveEffectInfo.ActiveGameplayEffectHandle=ActiveGameplayEffectHandle;


		
		const FActiveGameplayEffect* ActiveEffect= ASC->GetActiveGameplayEffect(ActiveGameplayEffectHandle);
		if (ActiveEffect && GetWorld())
		{
		ActiveEffectInfo.Remain=ActiveEffect->GetTimeRemaining(GetWorld()->GetTime().GetWorldTimeSeconds());
		ActiveEffectInfo.Period=ActiveEffect->GetPeriod();
		
		}
		else
		{
		ActiveEffectInfo.Period=ActiveEffectInfo.Remain=0;
		}
		
		OnActiveGameplayEffectAdded.Broadcast(ActiveEffectInfo);
		
		
		});



		GetAbilitySystemComponent()->OnPeriodicGameplayEffectExecuteDelegateOnSelf.AddLambda([this](UAbilitySystemComponent*ASC,
		const FGameplayEffectSpec&EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
		
		{
		FActiveEffectInfo ActiveEffectInfo{};
		
		EffectSpec.GetAllAssetTags(ActiveEffectInfo.EffectSpecAssetTags);
		EffectSpec.GetAllGrantedTags(ActiveEffectInfo.EffectSpecGrantedTags);
		ActiveEffectInfo.SuccessfullyApplied=ActiveGameplayEffectHandle.WasSuccessfullyApplied();
		ActiveEffectInfo.Duration=EffectSpec.Duration;
		ActiveEffectInfo.ActiveGameplayEffectHandle=ActiveGameplayEffectHandle;


		
		const FActiveGameplayEffect* ActiveEffect= ASC->GetActiveGameplayEffect(ActiveGameplayEffectHandle);
		if (ActiveEffect && GetWorld())
		{
		ActiveEffectInfo.Remain=ActiveEffect->GetTimeRemaining(GetWorld()->GetTime().GetWorldTimeSeconds());
		ActiveEffectInfo.Period=ActiveEffect->GetPeriod();
		
		}
		else
		{
		ActiveEffectInfo.Period=ActiveEffectInfo.Remain=0;
		}
		
		OnPeriodicGameplayEffectExecute.Broadcast(ActiveEffectInfo);
		
		});

	
	


	

	
}

void UUAbilityAsync_ListenForGameEffectDelegate::BindCallBackAfterAppied(FActiveEffectInfo ActiveEffectInfo,FActiveGameplayEffectHandle& ActiveGameplayEffectHandle)
{

	 FActiveGameplayEffectEvents* ActiveGameplayEffectEvents=GetAbilitySystemComponent()->GetActiveEffectEventSet(ActiveGameplayEffectHandle);
	
	 if (!ActiveGameplayEffectEvents)  return ;
	 
	ActiveGameplayEffectEvents->OnEffectRemoved.AddLambda([this,ActiveEffectInfo]
			( FGameplayEffectRemovalInfo const & Info)
			{
		
				OnActiveGameplayEffectRemoved_Info.Broadcast(ActiveEffectInfo);
		
			});

	 	
	ActiveGameplayEffectEvents->OnStackChanged.AddLambda([this,&ActiveEffectInfo]
			 (FActiveGameplayEffectHandle EffectHandle, int32 NewStackCount, int32 PreviousStackCount)
			 {

			 const FActiveGameplayEffect* StackActiveEffect= GetAbilitySystemComponent()->GetActiveGameplayEffect(EffectHandle);
			 ActiveEffectInfo.Remain=StackActiveEffect->GetTimeRemaining(GetWorld()->GetTime().GetWorldTimeSeconds());
			 ActiveEffectInfo.Stack=NewStackCount;
			
			 OnActiveGameplayEffectRemoved_Info.Broadcast(ActiveEffectInfo);
			
			 });

	 
	ActiveGameplayEffectEvents->OnTimeChanged.AddLambda([this,&ActiveEffectInfo]
		 (FActiveGameplayEffectHandle EffectHandle, float NewStartTime, float NewDuration)
		 {

		 const FActiveGameplayEffect* TimeActiveEffect= GetAbilitySystemComponent()->GetActiveGameplayEffect(EffectHandle);
		 ActiveEffectInfo.Remain=TimeActiveEffect->GetTimeRemaining(GetWorld()->GetTime().GetWorldTimeSeconds());
			
		 OnActiveGameplayEffectRemoved_Info.Broadcast(ActiveEffectInfo);
			
		 });
	 	
	ActiveGameplayEffectEvents->OnInhibitionChanged.AddLambda([this,&ActiveEffectInfo]
		 (FActiveGameplayEffectHandle EffectHandle, bool bIsInhibited)
	 	{
			 ActiveEffectInfo.bIsInhibited=bIsInhibited;
			 OnActiveGameplayEffectRemoved_Info.Broadcast(ActiveEffectInfo);
	 	});
	 	
	 	
};
	
	 	
	 
	
	
	


	




void UUAbilityAsync_ListenForGameEffectDelegate::Activate()
{
	Super::Activate();

	if (!GetAbilitySystemComponent()) return ;
	
	BindCallBack();



	
}

void UUAbilityAsync_ListenForGameEffectDelegate::EndAction()
{

	
	
	Super::EndAction();
	
	
}

UUAbilityAsync_ListenForDuraingGameEffectDelegate* UUAbilityAsync_ListenForDuraingGameEffectDelegate::ListenForDuartingGameEffectDelegate
(AActor* listerActor,FGameplayTag listen_GE_GrantedTag)
{
	UUAbilityAsync_ListenForDuraingGameEffectDelegate* obj=NewObject<UUAbilityAsync_ListenForDuraingGameEffectDelegate>();
	obj->SetAbilityActor(listerActor);
	obj->listen_GE_GrantedTag=listen_GE_GrantedTag;
	
	obj->RegisterWithGameInstance(listerActor);
	
	return obj;
	
	
}

void UUAbilityAsync_ListenForDuraingGameEffectDelegate::GetActiveEffectEvents()
{
	UAbilitySystemComponent* ASC=GetAbilitySystemComponent();
	
	ASC->OnActiveGameplayEffectAddedDelegateToSelf.AddLambda([this
		](UAbilitySystemComponent*OwingASC, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
		{

	
			FGameplayTagContainer TagGrantedContainer;
			FGameplayTagContainer TagAssetContainer;
		
			EffectSpec.GetAllGrantedTags(TagGrantedContainer);
			EffectSpec.GetAllAssetTags(TagAssetContainer);
			TArray<FGameplayTag> TagGrantedArray;
			TagGrantedContainer.GetGameplayTagArray(TagGrantedArray);
		
			if (!TagGrantedContainer.HasTag(listen_GE_GrantedTag))
			{
				
				GEngine->AddOnScreenDebugMessage(-1,3,FColor::Red,"TagGrantedArray Has not  listen_GE_GrantedTag!!");
				return ;
				
			}
			
			
				ActiveGameplayEffectEvents=GetAbilitySystemComponent()->GetActiveEffectEventSet(ActiveGameplayEffectHandle);

				// 更新委托传递的信息
				FActiveEffectInfo ActiveEffectInfo=UpdateActiveEffectInfo(OwingASC,ActiveGameplayEffectHandle,EffectSpec);
				OnEffectAdded.Broadcast(ActiveEffectInfo);
				
				// 绑定激活的GE相关的状态变化的委托
				BindCallBack();
				
			

		
		
		
		});
}

void UUAbilityAsync_ListenForDuraingGameEffectDelegate::BindCallBack()
{
	if (!ActiveGameplayEffectEvents)
	{

		GEngine->AddOnScreenDebugMessage(-1,3,FColor::Red,"ActiveGameplayEffectEvents is InValid!!");
		return;
		
	}; 

	ActiveGameplayEffectEvents->OnEffectRemoved.AddLambda([this](const FGameplayEffectRemovalInfo& info)
	{
		FActiveEffectInfo ActiveEffectInfo=UpdateActiveEffectInfo(GetAbilitySystemComponent(), info.ActiveEffect->Handle,info.ActiveEffect->Spec);
		OnEffectRemoved.Broadcast(ActiveEffectInfo);
		
	});

	ActiveGameplayEffectEvents->OnInhibitionChanged.AddLambda([this](FActiveGameplayEffectHandle ActiveGameplayEffectHandle, bool IsInhibition)
{
	FActiveEffectInfo ActiveEffectInfo=UpdateActiveEffectInfo(GetAbilitySystemComponent(),
		ActiveGameplayEffectHandle,GetAbilitySystemComponent()->GetActiveGameplayEffect(ActiveGameplayEffectHandle)->Spec);
		ActiveEffectInfo.bIsInhibited=IsInhibition;

		OnInhibitionChanged.Broadcast(ActiveEffectInfo);
		
});

	ActiveGameplayEffectEvents->OnStackChanged.AddLambda([this](FActiveGameplayEffectHandle ActiveGameplayEffectHandle,
		int newValue, int oldValue)
{
FActiveEffectInfo ActiveEffectInfo=UpdateActiveEffectInfo(GetAbilitySystemComponent(),
	ActiveGameplayEffectHandle,GetAbilitySystemComponent()->GetActiveGameplayEffect(ActiveGameplayEffectHandle)->Spec);
	ActiveEffectInfo.Stack=newValue;

	OnStackChanged.Broadcast(ActiveEffectInfo);
		
});



	// TimeChangeDelegate 更新的是新的总持续时间
	
	ActiveGameplayEffectEvents->OnTimeChanged.AddLambda([this](FActiveGameplayEffectHandle ActiveGameplayEffectHandle, float NewStartTime, float NewDuration)
{
FActiveEffectInfo ActiveEffectInfo=UpdateActiveEffectInfo(GetAbilitySystemComponent(),
ActiveGameplayEffectHandle,GetAbilitySystemComponent()->GetActiveGameplayEffect(ActiveGameplayEffectHandle)->Spec);
ActiveEffectInfo.Remain=ActiveEffectInfo.Duration=NewDuration;

OnTimeChanged.Broadcast(ActiveEffectInfo);
		
});

	
}

FActiveEffectInfo UUAbilityAsync_ListenForDuraingGameEffectDelegate::UpdateActiveEffectInfo(UAbilitySystemComponent* ASC,
	FActiveGameplayEffectHandle handle, const FGameplayEffectSpec& spec)
{


	FActiveEffectInfo ActiveEffectInfo;
	
	FGameplayTagContainer TagGrantedContainer;
	FGameplayTagContainer TagAssetContainer;
	spec.GetAllGrantedTags(TagGrantedContainer);
	spec.GetAllAssetTags(TagAssetContainer);
	
	ActiveEffectInfo.EffectSpecGrantedTags=TagGrantedContainer;
	ActiveEffectInfo.EffectSpecAssetTags=TagAssetContainer;
	ActiveEffectInfo.Duration=spec.Duration;
	const FActiveGameplayEffect* ActiveGameplayEffect=ASC->GetActiveGameplayEffect(handle);
	if (ActiveGameplayEffect&&GetWorld())
	{
		ActiveEffectInfo.Remain=ActiveGameplayEffect->GetTimeRemaining(GetWorld()->GetTime().GetWorldTimeSeconds());
		ActiveEffectInfo.bIsInhibited=ActiveGameplayEffect->bIsInhibited;
	}
	
	ActiveEffectInfo.Period=spec.Period;
	ActiveEffectInfo.Stack=ASC->GetCurrentStackCount(handle);
	ActiveEffectInfo.SuccessfullyApplied=handle.WasSuccessfullyApplied();
	

	return ActiveEffectInfo;

}

void UUAbilityAsync_ListenForDuraingGameEffectDelegate::Activate()
{
	Super::Activate();

	if (!GetAbilitySystemComponent())  return ;

	GetActiveEffectEvents();

	

	
}

void UUAbilityAsync_ListenForDuraingGameEffectDelegate::EndAction()
{
	Super::EndAction();
}

