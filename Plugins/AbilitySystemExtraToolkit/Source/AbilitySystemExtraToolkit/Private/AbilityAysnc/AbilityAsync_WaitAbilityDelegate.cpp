// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityAysnc/AbilityAsync_WaitAbilityDelegate.h"
#include "AbilitySystemComponent.h"
#include "BFL_extraToolKit.h"

DEFINE_LOG_CATEGORY_STATIC(Abilitylog, Warning, All)


UAbilityAsync_WaitAbilityDelegate* UAbilityAsync_WaitAbilityDelegate::ListenForGameplayAbilityDelegate(AActor* TargetActor, EAbilityQueryType AbilityQueryType,
                                                                                                       FGameplayAbilitySpecHandle AbilitySpecHandle, int32 AbilityInputID,
                                                                                                       TSubclassOf<UGameplayAbility> AbilityClass,float CoolDownUpdateInterval)
{
	UAbilityAsync_WaitAbilityDelegate* obj=NewObject<UAbilityAsync_WaitAbilityDelegate>();

	obj->SetAbilityActor(TargetActor);
	
	obj->AbilityQueryType=AbilityQueryType;
	obj->AbilityClass=AbilityClass;
	obj->AbilityInputID=AbilityInputID;
	obj->AbilitySpecHandle=AbilitySpecHandle;
	
	
	obj->CoolDownUpdateInterval=CoolDownUpdateInterval;

	//为了使用定时器，必须注册到World中
	obj->RegisterWithGameInstance(TargetActor);

	
	return obj;
	
}

void UAbilityAsync_WaitAbilityDelegate::Activate()
{
	Super::Activate();

	if (!GetAbilitySystemComponent()) return ;

	
	if (!GetTimerManager())
	{
		UE_LOG(Abilitylog,Warning,TEXT("GetTimerManager() return a Nullptr "));
		return ;
	}
	
	UE_LOG(Abilitylog,Warning,TEXT("GetTimerManager() return is valid "));


	
	BindCallBack();

	

		
	
	
	
}

void UAbilityAsync_WaitAbilityDelegate::EndAction()
{
	
	
	if (TimerHandle.IsValid()&& GetWorld()&& GetWorld()->GetTimerManager().TimerExists(TimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}

	
	
	Super::EndAction();

	/*if (OnAbilityActiveHandle.IsValid() && AbilitySpecDirtiedHandle.IsValid() &&
		OnAbilityCommitHandle.IsValid() && OnAbilityEndle.IsValid() && OnAbilityFailHandle.IsValid())
	{
		GetAbilitySystemComponent()->AbilityActivatedCallbacks.Remove(OnAbilityActiveHandle);
		GetAbilitySystemComponent()->AbilitySpecDirtiedCallbacks.Remove(AbilitySpecDirtiedHandle);
		GetAbilitySystemComponent()->AbilityCommittedCallbacks.Remove(OnAbilityCommitHandle);
		GetAbilitySystemComponent()->AbilityEndedCallbacks.Remove(OnAbilityEndle);
		GetAbilitySystemComponent()->AbilityFailedCallbacks.Remove(OnAbilityFailHandle);

	}*/
	
}

void UAbilityAsync_WaitAbilityDelegate::OnStartCoolDown()
{
	
	if (GetTimerManager())
	{
		GetTimerManager()->SetTimer(TimerHandle,[this]()
	{

			FAbilityInfo AbilityInfo=GetCurrentAbilityInfo();
			if (AbilityInfo.CoolDownRemain<=0.f)
			{
				if (TimerHandle.IsValid()&& GetTimerManager())
				{
					GetTimerManager()->ClearTimer(TimerHandle);
					return ;
				}
				
			}
			
			OnAbilityCoolDownUpdate.Broadcast(AbilityInfo);
			
				
	},
	CoolDownUpdateInterval,
	true);
	}

		
}


FGameplayAbilitySpec* UAbilityAsync_WaitAbilityDelegate::GetCurrentAbilitySpec()
{
	
	if (!GetAbilitySystemComponent())
	{
		return nullptr;
	}

	if (AbilityQueryType==EAbilityQueryType::AbilitySpecHanlde )
	{
		
		return GetAbilitySystemComponent()->FindAbilitySpecFromHandle(AbilitySpecHandle);
	}
	
	
	if (AbilityQueryType==EAbilityQueryType::AbilityInputID)
	{
		
		return GetAbilitySystemComponent()->FindAbilitySpecFromInputID(AbilityInputID);
	}

	if (AbilityQueryType==EAbilityQueryType::AbilityClass  && AbilityClass)
	{
		
		return GetAbilitySystemComponent()->FindAbilitySpecFromClass(AbilityClass);
	}

	

	
	return nullptr;
	
	
}

FAbilityInfo UAbilityAsync_WaitAbilityDelegate::GetCurrentAbilityInfo()
{
	FAbilityInfo AbilityInfo;

	if (!GetCurrentAbilitySpec())
	{
		return FAbilityInfo();
	}
	
	const FGameplayAbilitySpec& Spec=*GetCurrentAbilitySpec();
	

	AbilityInfo.name=Spec.Ability->GetName();
	AbilityInfo.level=Spec.Level;
	AbilityInfo.ASC=GetAbilitySystemComponent();
	AbilityInfo.AbilitySpecHandle=Spec.Handle;

	FAbilitySpecInfo AbilitySpecInfo=UBFL_extraToolKit::GetInfoAtAbilitySpecHandle(GetAbilitySystemComponent()->GetOwner(),GetCurrentAbilitySpec()->Handle);

	AbilityInfo.Costs=AbilitySpecInfo.Costs;
	AbilityInfo.CoolDownDuration=AbilitySpecInfo.CoolDownDuration;
	AbilityInfo.CoolDownRemain=AbilitySpecInfo.CoolDownRemain;
	

	return AbilityInfo;

	
}

void UAbilityAsync_WaitAbilityDelegate::BindCallBack()
{

	if ( !ShouldBroadcastDelegates())
	{
		return ;
	}
	
	OnAbilityActiveHandle = GetAbilitySystemComponent()->AbilityActivatedCallbacks.AddLambda([this](UGameplayAbility* Ability)
		{
		
		if (GetCurrentAbilitySpec()&& Ability->GetCurrentAbilitySpec() == GetCurrentAbilitySpec())
		{
			
			OnAbilityActive.Broadcast(GetCurrentAbilityInfo());
		  
		}
	});


	OnAbilityCommitHandle= GetAbilitySystemComponent()->AbilityCommittedCallbacks.AddLambda([&](UGameplayAbility* Ability)->void

	{
		if (GetCurrentAbilitySpec()&& Ability->GetCurrentAbilitySpec()==GetCurrentAbilitySpec())
		{
			
			OnAbilityCommit.Broadcast(GetCurrentAbilityInfo());
			
			OnStartCoolDown();
			
			//GEngine->AddOnScreenDebugMessage(-1,5,FColor::MakeRandomColor()," bindCallBack() has called!!"/*+Ability->GetName()*/);

		}
	});

	OnAbilityEndle= GetAbilitySystemComponent()->AbilityEndedCallbacks.AddLambda([this]( UGameplayAbility* Ability)
	{
			if (GetCurrentAbilitySpec()&& Ability->GetCurrentAbilitySpec()==GetCurrentAbilitySpec())
				OnAbilityEnd.Broadcast(GetCurrentAbilityInfo());
		
	});
	
	OnAbilityFailHandle= GetAbilitySystemComponent()->AbilityFailedCallbacks.AddLambda([this]( const UGameplayAbility* Ability,const FGameplayTagContainer& TagContainer)
	{
		if (GetCurrentAbilitySpec() && Ability->GetCurrentAbilitySpec()==GetCurrentAbilitySpec() )
			OnAbilityFail.Broadcast(GetCurrentAbilityInfo());
	});


	//判断当自己的能力信息发生变化时，需要更新AbilityInfo

	/*
	AbilitySpecDirtiedHandle= GetAbilitySystemComponent()->AbilitySpecDirtiedCallbacks.AddLambda([&]( const FGameplayAbilitySpec  & Spec)  
	{
		
		if (Spec.Handle==GetCurrentAbilitySpec()->Handle)
		{ 
			AbilitySpecDirtied.Broadcast(GetCurrentAbilityInfo());
			
		}
		

		
	});*/




	
	
}



	

