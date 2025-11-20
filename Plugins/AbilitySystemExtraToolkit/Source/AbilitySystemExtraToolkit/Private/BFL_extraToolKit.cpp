// Fill out your copyright notice in the Description page of Project Settings.


#include "BFL_extraToolKit.h"

#include <string>

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameplayCueManager.h"
#include "ReplaySubsystem.h"
#include "Engine/DemoNetDriver.h"
#include "Engine/OverlapResult.h"
#include "Net/ReplayPlaylistTracker.h"

void UBFL_extraToolKit::levelUp(AActor* TargetActor, const FGameplayAbilitySpecHandle& AbilitySpecHandle, int32 AddToLevel)
{

	UAbilitySystemComponent* TargetASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!TargetASC)  return ;

	FGameplayAbilitySpec* TargetAbilitySpec = TargetASC->FindAbilitySpecFromHandle(AbilitySpecHandle);
	
	if(!TargetAbilitySpec) return ;
		
	TargetAbilitySpec->Level+=AddToLevel;
	TargetASC->MarkAbilitySpecDirty(*TargetAbilitySpec);
		
	
}

void UBFL_extraToolKit::InitAbilityActorInfo(AActor* OnwerActor, AActor* AvatorActor)
{
	UAbilitySystemComponent* TargetASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OnwerActor);
	if (!TargetASC)  return ;

	TargetASC->InitAbilityActorInfo(OnwerActor,AvatorActor);

}

FGameplayAbilitySpecHandle UBFL_extraToolKit::GiveAbilityByClass(AActor* TargetActor,
	const TSubclassOf<UGameplayAbility> AbilityClass,int32 Level,int32 InputID,UObject* Source)
{
	UAbilitySystemComponent* TargetASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!TargetASC)  return FGameplayAbilitySpecHandle();

	bool HasGive=HasGivedAbility(TargetActor,AbilityClass);
	if (HasGive) return TargetASC->FindAbilitySpecFromClass(AbilityClass)->Handle;

	return TargetASC->GiveAbility(MakeAbilitySpec( AbilityClass,Level, InputID, Source));

}

FGameplayAbilitySpec UBFL_extraToolKit::MakeAbilitySpec(TSubclassOf<UGameplayAbility> AbilityClass,
                                                        int32 Level,int32 InputID,UObject* Source)
{
	

	FGameplayAbilitySpec AbilitySpec(AbilityClass,Level,InputID,Source);
	
	return AbilitySpec;
	
	
	
}

FGameplayAbilitySpecHandle UBFL_extraToolKit::GiveAbilitySpec(AActor* TargetActor,const FGameplayAbilitySpec&  AbilitySpec)
{

	
	UAbilitySystemComponent* TargetASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!TargetASC)  return FGameplayAbilitySpecHandle();

	return TargetASC->GiveAbility(AbilitySpec);
	
}

FGameplayAbilitySpecHandle UBFL_extraToolKit::GiveAbilitySpecWithParam(AActor* TargetActor,TSubclassOf<UGameplayAbility> AbilityClass,
	int32 Level, int32 InputID, UObject* Source)
{

	
	UAbilitySystemComponent* TargetASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!TargetASC)  return FGameplayAbilitySpecHandle();

	FGameplayAbilitySpec AbilitySpec =MakeAbilitySpec(AbilityClass,Level,InputID, Source);
	return TargetASC->GiveAbility(AbilitySpec);
	
}

void UBFL_extraToolKit::RemoveAbility(AActor* TargetActor, const FGameplayAbilitySpecHandle& AbilitySpecHandle)
{
	UAbilitySystemComponent* TargetASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!TargetASC)  return ;

	TargetASC->ClearAbility(AbilitySpecHandle);
	
}

void UBFL_extraToolKit::RemoveAllAbilityByID(AActor* TargetActor, int32 InputID)
{
	UAbilitySystemComponent* TargetASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!TargetASC)  return ;

	TargetASC->ClearAllAbilitiesWithInputID(InputID);

}

void UBFL_extraToolKit::RemoveAllAbility(AActor* TargetActor)
{
	UAbilitySystemComponent* TargetASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!TargetASC)  return ;

	TargetASC->ClearAllAbilities();

}

void UBFL_extraToolKit::SetRemoveAbilityOnEnd(AActor* TargetActor, FGameplayAbilitySpecHandle AbilitySpecHandle)
{
	UAbilitySystemComponent* TargetASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!TargetASC)  return ;

	TargetASC->SetRemoveAbilityOnEnd(AbilitySpecHandle);
	
}

FAbilitySpecInfo UBFL_extraToolKit::GetInfoAtAbilitySpecHandle(AActor* TargetActor,const FGameplayAbilitySpecHandle& AbilitySpecHandle)
{

	FAbilitySpecInfo AbilityInfo;
	
	UAbilitySystemComponent* TargetASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	if (!TargetASC||!TargetASC->FindAbilitySpecFromHandle(AbilitySpecHandle))  return AbilityInfo;
	
	 const FGameplayAbilitySpec& Spec=*TargetASC->FindAbilitySpecFromHandle(AbilitySpecHandle);
	AbilityInfo.level=Spec.Level;
	AbilityInfo.ASC=TargetASC;

	
	if (const UGameplayAbility* Ability =Spec.GetAbilityInstances().Last())
	{
		/*
		 *计算CoolDown
		*/
		float RemainTime;
		float Duration;
		const FGameplayAbilityActorInfo& Info=Spec.GetPrimaryInstance()->GetActorInfo();
		Ability->GetCooldownTimeRemainingAndDuration(Ability->GetCurrentAbilitySpecHandle(), &Info,RemainTime,Duration);

		AbilityInfo.CoolDownDuration=Duration;
		AbilityInfo.CoolDownRemain=RemainTime;

		/*
		 * ******   计算cost,注意考虑多个修改器和执行器修改同一属性时，需要累加！！！
		 */
		
		UGameplayEffect* EffectDef=Ability->GetCostGameplayEffect();

	
		TSet<FGameplayAttribute> Attributes=GetAllModifyAttributeByEffecDef(EffectDef);
		for (auto Attribute : Attributes)
		{
			float ModifyValue=GetModifyValueToAttributeByEffecDef(TargetActor,EffectDef,Attribute,Spec.Level);
			AbilityInfo.Costs.Add(Attribute,ModifyValue);
			
		}
		
		
		
	}
	
	return AbilityInfo;
	
}


TArray<FGameplayAbilitySpecHandle> UBFL_extraToolKit::FindAbilitySpecHandle(AActor* TargetActor,EFindAbilityType Type,
	FGameplayTagQuery TagQuery ,FGameplayTagContainer Tag, int32 InputID)
{
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;
	UAbilitySystemComponent* TargetASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!TargetASC)  return AbilitySpecHandles;

	if (Type==EFindAbilityType::AbilityQuery)
	{
		TargetASC->FindAllAbilitiesMatchingQuery(AbilitySpecHandles,TagQuery);
	}

	if (Type==EFindAbilityType::Tag)
	{
		TargetASC->FindAllAbilitiesWithTags(AbilitySpecHandles,Tag);
	}

	if (Type==EFindAbilityType::InputID)
	{
		TargetASC->FindAllAbilitiesWithInputID(AbilitySpecHandles,InputID);
	}

	return AbilitySpecHandles;
}

bool UBFL_extraToolKit::HasGivedAbility(AActor* TargetActor, const TSubclassOf<UGameplayAbility> AbilityClass)
{
	UAbilitySystemComponent* TargetASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!TargetASC||!AbilityClass)  return false ;

	FGameplayAbilitySpec* AbilitySpec=TargetASC->FindAbilitySpecFromClass(AbilityClass);

	return AbilitySpec?true:false;

}

void UBFL_extraToolKit::AddAttributesByClass(AActor* TargetActor, TSubclassOf<UAttributeSet> AttributeSetClass)
{
	UAbilitySystemComponent* TargetASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!TargetASC||!AttributeSetClass.Get())  return ;

	TargetASC->AddSpawnedAttribute(AttributeSetClass.GetDefaultObject());
	
}

void UBFL_extraToolKit::RemoveAttributesByClass(AActor* TargetActor, TSubclassOf<UAttributeSet> AttributeSetClass)
{
	UAbilitySystemComponent* TargetASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!TargetASC||!AttributeSetClass.Get())  return ;

	TargetASC->RemoveSpawnedAttribute(AttributeSetClass.GetDefaultObject());
	
}

void UBFL_extraToolKit::RemoveAllAttributes(AActor* TargetActor)
{
	UAbilitySystemComponent* TargetASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!TargetASC)  return ;

	TargetASC->RemoveAllSpawnedAttributes();
	
}

void UBFL_extraToolKit::ApplyModToAttribute(AActor* TargetActor,  FGameplayAttribute Attribute,
	TEnumAsByte<EGameplayModOp::Type> ModifierOp, float ModifierMagnitude)
{
	UAbilitySystemComponent* TargetASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!TargetASC)  return ;

	TargetASC->ApplyModToAttribute(Attribute,ModifierOp, ModifierMagnitude);
	
}

  TMap<FGameplayAttribute,float> UBFL_extraToolKit::GetAllAttribute(AActor* TargetActor)
{
	TMap<FGameplayAttribute,float> AttributeMap;
	
	UAbilitySystemComponent* TargetASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!TargetASC)  return AttributeMap;

	TArray<FGameplayAttribute> Attributes;
	for (auto Attribute : Attributes)
	{
		AttributeMap.Add(Attribute,TargetASC->GetNumericAttribute(Attribute));
		
	}

	return AttributeMap;

}

/*FonAttributeChanged&   UBFL_extraToolKit::GetAttributeChangedDelegate(AActor* TargetActor)
{

	
	static  FonAttributeChanged OnAttributeChanged;
	
	UAbilitySystemComponent* TargetASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!TargetASC)  return OnAttributeChanged;

	for (auto Element : GetAllAttribute(TargetActor))
	{
		TargetASC->GetGameplayAttributeValueChangeDelegate(Element.Key).AddLambda([this](const FOnAttributeChangeData&Data)
		{
			OnAttributeChanged.Broadcast(Data.Attribute,Data.NewValue,Data.OldValue);
		});
	}
	
	return OnAttributeChanged;
	
}*/

void UBFL_extraToolKit::SetReplicationMode(AActor* TargetActor, EGameplayEffectReplicationMode NewReplicationMode)
{
	UAbilitySystemComponent* TargetASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!TargetASC)  return ;

	TargetASC->SetReplicationMode(NewReplicationMode);
	
}

FActiveGameplayEffectHandle UBFL_extraToolKit::MakeGameplayEffectCompletedCustomToApply(AActor* TargetActor,int32 ApplyLevel,
							EGameplayEffectDurationType DurationType,
							 FScalableFloat DurationMagnitudefloat,
							 FScalableFloat PeriodMagnitudefloat,
							FGameplayTag GrantTag,TArray<FAttributeModifyInfo> AttributeModifyInfos
							,
							FEffectStackInfo  EffectStackInfo
							)

{

	UAbilitySystemComponent* TargetASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!TargetASC)  return  FActiveGameplayEffectHandle();

	UGameplayEffect* GameplayEffect=NewObject<UGameplayEffect>();


	//定制执行策略
	GameplayEffect->DurationPolicy=EGameplayEffectDurationType::Instant;
	const FGameplayEffectModifierMagnitude DurationEffectModifierMagnitude{DurationMagnitudefloat};
	GameplayEffect->DurationMagnitude=DurationEffectModifierMagnitude;
	GameplayEffect->Period=PeriodMagnitudefloat;
	

	// 定制授予的标签
	UTargetTagsGameplayEffectComponent& TargetTagsGameplayEffectComponent= GameplayEffect->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();

	FInheritedTagContainer TagContainer;
	if (GrantTag.IsValid())
	{
		TagContainer.CombinedTags.AddTag(GrantTag);
	
	}
	TargetTagsGameplayEffectComponent.SetAndApplyTargetTagChanges(TagContainer);
	TargetTagsGameplayEffectComponent.OnGameplayEffectChanged();
	
	FString DeugTag=GameplayEffect->FindComponent<UTargetTagsGameplayEffectComponent>()->GetConfiguredTargetTagChanges().CombinedTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1,5,FColor::MakeRandomColor(),DeugTag);
	
	// 定制属性修改器数组

	for (auto AttributeModifyInfo:AttributeModifyInfos)
	{
		FGameplayModifierInfo GameplayModifierInfo;
		GameplayModifierInfo.Attribute=AttributeModifyInfo.Attribute;

		FGameplayEffectModifierMagnitude EffectModifierMagnitudeAttributefloat{AttributeModifyInfo.ScalableFloat};
		GameplayModifierInfo.ModifierMagnitude=EffectModifierMagnitudeAttributefloat;

		GameplayModifierInfo.ModifierOp=AttributeModifyInfo.AttributesModOp;
		GameplayEffect->Modifiers.Add(GameplayModifierInfo);
	}

	// 堆栈策略
	GameplayEffect->StackDurationRefreshPolicy=EffectStackInfo.EffectStackingDurationPolicy;
	GameplayEffect->StackingType=EffectStackInfo.EffectStackingType;
	GameplayEffect->StackExpirationPolicy=EffectStackInfo.EffectStackingExpirationPolicy;
	GameplayEffect->StackLimitCount=EffectStackInfo.StackLimitCount;
	GameplayEffect->StackPeriodResetPolicy=EffectStackInfo.EffectStackingPeriodPolicy;

	FGameplayEffectSpec EffectSpec(GameplayEffect,TargetASC->MakeEffectContext(),ApplyLevel);
	

	
	return TargetASC->ApplyGameplayEffectSpecToSelf(EffectSpec);
	

}

float UBFL_extraToolKit::GetModifyValueToAttributeByEffecDef(AActor* SourceActor, UGameplayEffect* EffecDef,FGameplayAttribute Attribute,int32 level)
{
	
	UAbilitySystemComponent* SourceASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor);
	//UAbilitySystemComponent* ApplyToTargetASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ApplyToActor);
	
	if (!SourceASC/*||!ApplyToActor*/||!EffecDef||!Attribute.IsValid())  return  0;

	FGameplayEffectContextHandle EffectContextHandle=MakeEffectContextWithParam(SourceActor,SourceActor,SourceActor);
	
	FGameplayEffectSpecHandle EffectSpec=SourceASC->MakeOutgoingSpec(EffecDef->GetClass(),level,EffectContextHandle);
	
	/*
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle =ApplyToTargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data);
	float ModifyValue=ApplyToTargetASC->GetGameplayEffectMagnitude(ActiveGameplayEffectHandle,Attribute);
	*/
	
	float AttributeModTotalNum=0;
	
	TArray<FGameplayModifierInfo> ModifierInfos;
	TArray<FGameplayEffectExecutionScopedModifierInfo> EffectExecutionScopedModifierInfos;
	
	for (auto Execution:EffecDef->Executions)
	{
		EffectExecutionScopedModifierInfos+=Execution.CalculationModifiers;
	}
	ModifierInfos+=EffecDef->Modifiers;
	
	for (auto Modifier : ModifierInfos)
	{
		if (Modifier.Attribute==Attribute)
		{
			float currentModNum;
			Modifier.ModifierMagnitude.AttemptCalculateMagnitude(*EffectSpec.Data,currentModNum);
			AttributeModTotalNum+=currentModNum;
			
		}
					
	}

	for (auto Modifier : EffectExecutionScopedModifierInfos)
	{
		if (Modifier.CapturedAttribute.AttributeToCapture==Attribute)
		{
			float currentModNum;
			Modifier.ModifierMagnitude.AttemptCalculateMagnitude(*EffectSpec.Data,currentModNum);
			AttributeModTotalNum+=currentModNum;
			
		}
		
					
	}

	
	return AttributeModTotalNum;
				
}

float UBFL_extraToolKit::GetModifyValueToAttributeByEffecClass(AActor* SourceActor,
	TSubclassOf<UGameplayEffect> Effecclass, FGameplayAttribute Attribute, int32 level)
{
	return GetModifyValueToAttributeByEffecDef(SourceActor,Effecclass.GetDefaultObject(),Attribute,level);
}


TSet<FGameplayAttribute> UBFL_extraToolKit::GetAllModifyAttributeByEffecDef(UGameplayEffect* EffecDef)
{
	
	TSet<FGameplayAttribute> Attributes;
	if (!EffecDef) return Attributes;
	
	TArray<FGameplayModifierInfo> ModifierInfos;
	TArray<FGameplayEffectExecutionScopedModifierInfo> EffectExecutionScopedModifierInfos;
	
	for (auto Execution:EffecDef->Executions)
	{
		EffectExecutionScopedModifierInfos+=Execution.CalculationModifiers;
	}
	ModifierInfos+=EffecDef->Modifiers;

	for (auto ModifierInfo : ModifierInfos)
	{
		Attributes.Add(ModifierInfo.Attribute);
	}
	for (auto ModifierInfo : EffectExecutionScopedModifierInfos)
	{
		Attributes.Add(ModifierInfo.CapturedAttribute.AttributeToCapture);
	}
	
	
	return Attributes;
}

TSet<FGameplayAttribute> UBFL_extraToolKit::GetAllModifyAttributeByEffecClass(TSubclassOf<UGameplayEffect> Effecclass)
{
	return GetAllModifyAttributeByEffecDef(Effecclass.GetDefaultObject());
}

TMap<FGameplayAttribute, float> UBFL_extraToolKit::GetAllModifyInfosByEffectDef(AActor* TargetActor,UGameplayEffect*EffecDef ,int32 level)
{
	TMap<FGameplayAttribute, float> GameplayAttributeMap;
	if (!EffecDef) return GameplayAttributeMap;

	TSet<FGameplayAttribute> Attribute=GetAllModifyAttributeByEffecDef(EffecDef);
	for (auto GameplayAttribute : Attribute)
	{
		float ModifyValue=GetModifyValueToAttributeByEffecDef(TargetActor,EffecDef,GameplayAttribute,level);
		GameplayAttributeMap.Add(GameplayAttribute,ModifyValue);
		
	}

	return GameplayAttributeMap;
	
}

TMap<FGameplayAttribute, float> UBFL_extraToolKit::GetAllModifyInfosByEffectClass(AActor* TargetActor,TSubclassOf<UGameplayEffect> Effecclass ,int32 level)
{
	return  GetAllModifyInfosByEffectDef(TargetActor,Effecclass.GetDefaultObject(),level);
}

bool UBFL_extraToolKit::CanApplyAttributeModifiers(AActor* TargetActor,const UGameplayEffect* GameplayEffect, float Level,
	const FGameplayEffectContextHandle& EffectContext)
{
	
	UAbilitySystemComponent* ASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!ASC)  return  false;
	
	return ASC->CanApplyAttributeModifiers(GameplayEffect,Level,EffectContext);
	
}




FGameplayEffectContextHandle UBFL_extraToolKit::MakeEffectContextWithParam(AActor*Instigator ,AActor*Casuer,UObject* SourceObj,FVector Orgin,
                                                                           const FHitResult&HitResult )
{
	FGameplayEffectContextHandle Context = FGameplayEffectContextHandle(UAbilitySystemGlobals::Get().AllocGameplayEffectContext());

	Context.AddInstigator(Instigator,Casuer);
	Context.AddOrigin(Orgin);
	Context.AddHitResult(HitResult);
	Context.AddSourceObject(SourceObj);

	
	
	return Context;
}

FActiveGameplayEffectHandle UBFL_extraToolKit::ApplyGEToTarget(AActor* Who, AActor* Target,
	TSubclassOf<UGameplayEffect> EffectClass, int32 level)
{
	UAbilitySystemComponent* TargetASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	
	if (!Who||!Target||!TargetASC||!EffectClass.Get()||!EffectClass.GetDefaultObject())  return  0;

	FGameplayEffectContextHandle EffectContextHandle=MakeEffectContextWithParam(Who,Who,Who);

	return TargetASC->ApplyGameplayEffectToSelf(EffectClass.GetDefaultObject(),level,EffectContextHandle);

	
}

 FActiveGameplayEffect  UBFL_extraToolKit::GetActiveEffect(AActor*Target,
	const FActiveGameplayEffectHandle& ActiveGameplayEffectHandle)
{
	UAbilitySystemComponent* TargetASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	if (!TargetASC) return FActiveGameplayEffect();

	
	const FActiveGameplayEffect*ActiveGameplayEffect=TargetASC->GetActiveGameplayEffect(ActiveGameplayEffectHandle);
	return *ActiveGameplayEffect;
	
	
}

TArray<FActiveGameplayEffectHandle> UBFL_extraToolKit::GetActiveEffectwithAllTag(AActor* Target, FGameplayTagContainer Tag)
{
	UAbilitySystemComponent* TargetASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	if (!TargetASC) return TArray<FActiveGameplayEffectHandle>();

	return TargetASC->GetActiveEffectsWithAllTags(Tag);

}

const UGameplayEffect* UBFL_extraToolKit::GetGameplayEffectCDO(AActor* Target, const FActiveGameplayEffectHandle Handle)
{
	UAbilitySystemComponent* TargetASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	if (!TargetASC) return nullptr;
	
	return TargetASC->GetGameplayEffectCDO(Handle);
	
}

const UGameplayEffect* UBFL_extraToolKit::GetGameplayEffectCDOByClass(TSubclassOf<UGameplayEffect> Effect)
{
	return Effect.GetDefaultObject();
}

/*
TArray<FActiveGameplayEffectHandle>  UBFL_extraToolKit::GetAllActiveEffects(AActor* Target)
{
	const UAbilitySystemComponent* TargetASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	if (!TargetASC) return TArray<FActiveGameplayEffectHandle>();
	
	const FActiveGameplayEffectsContainer ActiveGameplayEffectsContainer=TargetASC->GetActiveGameplayEffects();
	return ActiveGameplayEffectsContainer.GetAllActiveEffectHandles();

}
*/




void UBFL_extraToolKit::SendTargetDataToServer(AActor* TargetActor,FGameplayAbilitySpecHandle AbilitySpecHandle, FGameplayAbilityTargetDataHandle TargetData)
{
	UAbilitySystemComponent* TargetASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!TargetASC) return ;
	
	FScopedPredictionWindow ScopedPredictionWindow(TargetASC);
	FGameplayAbilitySpec* AbilitySpec=TargetASC->FindAbilitySpecFromHandle(AbilitySpecHandle);
	const FPredictionKey OldKey=AbilitySpec->GetPrimaryInstance()->GetCurrentActivationInfo().GetActivationPredictionKey();
	
	TargetASC->ServerSetReplicatedTargetData(AbilitySpecHandle,OldKey,TargetData,FGameplayTag(),TargetASC->ScopedPredictionKey);
	
}

;


/*
TSet<AActor*> UBFL_extraToolKit::FindOverlapActorsByChannelOrObject(AActor* TargetActor,EOverlapQueryType OverlapQueryType,FVector Origin,
			ECollisionChannel CollisionChannel,FCollisionObjectQueryParams ObjectQueryParams,ECollisionShape::Type CollisionShapeType,FVector Extend)
{
	if (!TargetActor) return  TSet<AActor*>();

	 UWorld* world=TargetActor->GetWorld();
	if (!world) return TSet<AActor*>();
	
	TArray<FOverlapResult> OverlapResults;
	
	FCollisionShape CollisionShape;
	CollisionShape.SetShape(CollisionShapeType,Extend);
	
	if (OverlapQueryType==EOverlapQueryType::Channel)
	{
		world->OverlapMultiByChannel(OverlapResults,Origin,FQuat(),CollisionChannel,CollisionShape);
	}
	if (OverlapQueryType==EOverlapQueryType::Object)
	{
		world->OverlapMultiByObjectType(OverlapResults,Origin,FQuat(),ObjectQueryParams,CollisionShape);
	}
	if (OverlapResults.IsEmpty()) return TSet<AActor*>();
	
	TSet<AActor*> AActors;
	for (auto OverlapResult : OverlapResults)
	{
		if (OverlapResult.bBlockingHit)
		{
			if (!OverlapResult.GetActor())
			{
				continue;	
			}
			AActors.Add(OverlapResult.GetActor());
			
		};
		
		
	};
	
	return AActors;
	
	
};
*/

void UBFL_extraToolKit::StartRecordingReplay(UObject* WorldContextObject,FString ReplayName)
{
	if (!WorldContextObject) return ;
	UWorld* World = WorldContextObject->GetWorld();
	if (!World) return ;
	
	UGameInstance* GameInstance=World->GetGameInstance();
	if (!GameInstance) return ;

	UReplaySubsystem* ReplaySubsystem=GameInstance->GetSubsystem<UReplaySubsystem>();
	checkf(ReplaySubsystem,TEXT("%hs:(%d) ReplaySubsystem is Not Vaild!"),__FILE__,__LINE__);
	if (!ReplaySubsystem) return;
	
	ReplaySubsystem->RecordReplay(ReplayName,FString(),TArray<FString>(),nullptr);
	UE_LOG(LogTemp,Warning,TEXT("Replay Start!"));
	
	
}

void UBFL_extraToolKit::StopRecordReplay(UObject* WorldContextObject)
{
	if (WorldContextObject)
	{
		UWorld* World = WorldContextObject->GetWorld();
		if (World)
		{
			UGameInstance* GameInstance=World->GetGameInstance();
			if (GameInstance)
			{
				static int32 index=1;
				GameInstance->StopRecordingReplay();

				
			}
		}
	}
}

void UBFL_extraToolKit::PlayRecordReplay(UObject* WorldContextObject,FString ReplayName)
{
	if (!WorldContextObject) return ;
	UWorld* World = WorldContextObject->GetWorld();
	if (!World) return ;
	
	UGameInstance* GameInstance=World->GetGameInstance();
	if (!GameInstance) return ;

	UReplaySubsystem* ReplaySubsystem=GameInstance->GetSubsystem<UReplaySubsystem>();
	checkf(ReplaySubsystem,TEXT("%hs:(%d) ReplaySubsystem is Not Vaild!"),__FILE__,__LINE__);
	if (!ReplaySubsystem) return;
	
	ReplaySubsystem->PlayReplay(ReplayName,World,TArray<FString>());
	UE_LOG(LogTemp,Warning,TEXT("Replay Playing!"));
	
	
}

void UBFL_extraToolKit::StopReplay(UObject* WorldContextObject)
{
	if (!WorldContextObject) return ;
	UWorld* World = WorldContextObject->GetWorld();
	if (!World) return ;
	
	UGameInstance* GameInstance=World->GetGameInstance();
	if (!GameInstance) return ;

	UReplaySubsystem* ReplaySubsystem=GameInstance->GetSubsystem<UReplaySubsystem>();
	checkf(ReplaySubsystem,TEXT("%hs:(%d) ReplaySubsystem is Not Vaild!"),__FILE__,__LINE__);
	if (!ReplaySubsystem) return ;
	
	ReplaySubsystem->StopReplay();
	UE_LOG(LogTemp,Warning,TEXT("Replay Stopped!"));
	
	
	
}

TArray<FVector> UBFL_extraToolKit::GetPointsInConeByEnvQueryGenerator(AActor* TargetActor, float InConeDegreesValue,float AlignedPointDistanceValue,
		float AngleStepValue,float GenerationRange)
{
	if (!TargetActor)
	{
		return TArray<FVector>();
		
	}
	
	const float ConeDegreesValue = FMath::Clamp(InConeDegreesValue, 0.f, 359.f);
	
	if (ConeDegreesValue == 0)
	{
		return TArray<FVector>();
	}
	
	const int32 MaxPointsPerAngleValue = FMath::FloorToInt(GenerationRange / AlignedPointDistanceValue);

	TArray<FVector> GeneratedItems;
	GeneratedItems.Reserve( FMath::CeilToInt(ConeDegreesValue / AngleStepValue) * MaxPointsPerAngleValue + 1);

	//Generate points for each actor

		const FVector ForwardVector =TargetActor->GetActorForwardVector();
		const FVector ActorLocation = TargetActor->GetActorLocation();
		
		for (float Angle = -(ConeDegreesValue * 0.5f); Angle < (ConeDegreesValue * 0.5f); Angle += AngleStepValue)
		{
			const FVector AxisDirection = ForwardVector.RotateAngleAxis(Angle, FVector::UpVector);

			// skipping PointIndex == 0 as that's the context's location
			for (int32 PointIndex = 1; PointIndex <= MaxPointsPerAngleValue; PointIndex++)
			{
				const FVector GeneratedLocation = ActorLocation + (AxisDirection * PointIndex * AlignedPointDistanceValue);
				GeneratedItems.Add(GeneratedLocation);
			}
		}

	return GeneratedItems;
	
}

TArray<FVector> UBFL_extraToolKit::GetPointsInConeByEnvQueryGeneratorWithParam(FVector ActorLocation, FVector ForwardVector,
	float InConeDegreesValue, float AlignedPointDistanceValue, float AngleStepValue, float GenerationRange)
{
	
	const float ConeDegreesValue = FMath::Clamp(InConeDegreesValue, 0.f, 359.f);
	
	if (ConeDegreesValue == 0)
	{
		return TArray<FVector>();
	}
	
	const int32 MaxPointsPerAngleValue = FMath::FloorToInt(GenerationRange / AlignedPointDistanceValue);

	TArray<FVector> GeneratedItems;
	GeneratedItems.Reserve( FMath::CeilToInt(ConeDegreesValue / AngleStepValue) * MaxPointsPerAngleValue + 1);

	//Generate points for each actor


	for (float Angle = -(ConeDegreesValue * 0.5f); Angle < (ConeDegreesValue * 0.5f); Angle += AngleStepValue)
	{
		const FVector AxisDirection = ForwardVector.RotateAngleAxis(Angle, FVector::UpVector);

		// skipping PointIndex == 0 as that's the context's location
		for (int32 PointIndex = 1; PointIndex <= MaxPointsPerAngleValue; PointIndex++)
		{
			const FVector GeneratedLocation = ActorLocation + (AxisDirection * PointIndex * AlignedPointDistanceValue);
			GeneratedItems.Add(GeneratedLocation);
		}
	}

	return GeneratedItems;
	
}




void UBFL_extraToolKit::GotoTimeInSeconds(UObject* WorldContextObject,float TimeInSeconds )
{

	if (!WorldContextObject) return ;
	UWorld* World = WorldContextObject->GetWorld();
	if (!World) return ;
	
	UDemoNetDriver* DemoNetDriver=World->GetDemoNetDriver();
	if (!DemoNetDriver) return ;

	
	DemoNetDriver->GotoTimeInSeconds( TimeInSeconds);

	

	
}

void UBFL_extraToolKit::PauseRecordingReplay(UObject* WorldContextObject,bool bPauseRecording )
{
	if (!WorldContextObject) return ;
	UWorld* World = WorldContextObject->GetWorld();
	if (!World) return ;
	
	UDemoNetDriver* DemoNetDriver=World->GetDemoNetDriver();
	if (!DemoNetDriver) return ;
	
	DemoNetDriver->PauseRecording(bPauseRecording);
	
	
}



float UBFL_extraToolKit::GetNearestActor(AActor* TargetActor, const TArray<AActor*>& CheckActors, AActor*& NearestActor)
{
	if (!TargetActor||CheckActors.Num()<= 0) return 0.f;
	
	float MinDistance=TNumericLimits<float>::Max();

	for (auto Element : CheckActors)
	{
		if (!Element) continue;
		
		float Distance=FVector::Distance(TargetActor->GetActorLocation(),Element->GetActorLocation());
		
		if (Distance <= MinDistance)
		{
			MinDistance = Distance;
			NearestActor=Element;
		}
		
	}
	
	return MinDistance;
	
}



void  UBFL_extraToolKit::SortActorsByDistance(AActor* TargetActor, bool bIsAscending, TArray<AActor*>& CheckActors)
{

	TArray<AActor*> SortedActors;
	
	if (!TargetActor || CheckActors.Num() == 0)
		return;
    
	// 获取目标Actor的位置
	FVector TargetLocation = TargetActor->GetActorLocation();
    
	// 使用Lambda表达式按距离排序（升序）
		CheckActors.Sort([TargetLocation,bIsAscending](const AActor& A, const AActor& B) {
		if (!&A || !&B) return false;
        
		float DistanceA = FVector::DistSquared(TargetLocation, A.GetActorLocation());
		float DistanceB = FVector::DistSquared(TargetLocation, B.GetActorLocation());
        
		return bIsAscending?DistanceA < DistanceB:DistanceA > DistanceB;
	});

	
	
}

void UBFL_extraToolKit::SplitActorsByDirection(AActor* TargetActor, const TArray<AActor*>& Actors,
	TArray<AActor*>& LeftActors, TArray<AActor*>& RightActors)
{
	LeftActors.Empty();
	RightActors.Empty();
    
	if (!TargetActor || Actors.Num() == 0)
		return;
    
	// 获取目标Actor的朝向和位置
	FVector TargetForward = TargetActor->GetActorForwardVector();
	FVector TargetLocation = TargetActor->GetActorLocation();
    
	for (AActor* Actor : Actors)
	{
		if (!Actor) continue;
        
		// 计算从目标指向Actor的向量
		FVector ToActor = Actor->GetActorLocation() - TargetLocation;
        
		// 使用点积判断左右（使用目标Actor的右向量作为参考）
		// 点积 > 0 表示在右侧，点积 < 0 表示在左侧
		float DotProduct = FVector::DotProduct(ToActor, TargetActor->GetActorRightVector());
        
		if (DotProduct >= 0.0f)
		{
			RightActors.Add(Actor);
		}
		else
		{
			LeftActors.Add(Actor);
		}
	}
}



// 使用Atan2计算0-360度角度
float UBFL_extraToolKit::CalculateAngleUsingAtan2(const FVector& ActorLocation, const FVector& TargetLocation, 
							  const FVector& TargetForward, const FVector& TargetRight)
{
	// 计算相对位置向量
	FVector RelativeVec = ActorLocation - TargetLocation;
    
	// 将相对向量转换到目标Actor的局部坐标系
	float ForwardComponent = FVector::DotProduct(RelativeVec, TargetForward);
	float RightComponent = FVector::DotProduct(RelativeVec, TargetRight);
    
	// 使用Atan2计算角度（返回-180到180度）
	float Angle = FMath::RadiansToDegrees(FMath::Atan2(RightComponent, ForwardComponent));
    
	// 转换为0-360度
	if (Angle < 0)
	{
		Angle += 360.0f;
	}
    
	return Angle;
}



float UBFL_extraToolKit::CalculateFullAngle(float DotProduct, const FVector& CrossProduct, const FVector& RightVector)
{
	// 基础角度（0-180度）
	float BaseAngle = FMath::RadiansToDegrees(FMath::Acos(FMath::Clamp(DotProduct, -1.0f, 1.0f)));
    
	// 使用叉积与右向量的点积判断方向（正值为右侧，负值为左侧）
	float DirectionDot = FVector::DotProduct(CrossProduct, RightVector);
    
	// 如果在左侧（逆时针方向），则角度为360-基础角度
	if (DirectionDot < 0)
	{
		return 360.0f - BaseAngle;
	}
    
	return BaseAngle;
}


void UBFL_extraToolKit::SortActorsByFullViewAngle(AActor* TargetActor, UPARAM(ref)TArray<AActor*>& Actors)
{
	if (!TargetActor || Actors.Num() == 0)
		return;
    
	// 获取目标Actor的位置、前向向量和右向量
	FVector TargetLocation = TargetActor->GetActorLocation();
	FVector TargetForward = TargetActor->GetActorForwardVector();
	FVector TargetRight = TargetActor->GetActorRightVector();
    
	// 使用Lambda表达式按视角夹角排序（升序，0-360度）
	Actors.Sort([TargetLocation, TargetForward, TargetRight](const AActor& A, const AActor& B) {
		if (!&A || !&B) return false;
        
		// 计算从目标指向Actor的方向向量
		FVector ToA = (A.GetActorLocation() - TargetLocation).GetSafeNormal();
		FVector ToB = (B.GetActorLocation() - TargetLocation).GetSafeNormal();
        
		// 计算点积（夹角余弦值）
		float DotA = FVector::DotProduct(TargetForward, ToA);
		float DotB = FVector::DotProduct(TargetForward, ToB);
        
		// 计算叉积（用于判断左右方向）
		FVector CrossA = FVector::CrossProduct(TargetForward, ToA);
		FVector CrossB = FVector::CrossProduct(TargetForward, ToB);
        
		// 使用点积和右向量的点积来确定0-360度角度
		float AngleA = CalculateFullAngle(DotA, CrossA, TargetRight);
		float AngleB = CalculateFullAngle(DotB, CrossB, TargetRight);
        
		return AngleA < AngleB; // 升序排序
	});
}

