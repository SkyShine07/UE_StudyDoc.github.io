// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CollisionQueryParams.h"
#include "CollisionShape.h"
#include "Engine/EngineTypes.h"


#include "BFL_extraToolKit.generated.h"


using namespace ECollisionShape;
/**
 * 
 */

/*
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilitySpecChanged,int32,level);
*/


class UGameplayEffect;


USTRUCT(BlueprintType)
struct FAttributeModifyInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FGameplayAttribute Attribute;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FScalableFloat ScalableFloat;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TEnumAsByte<EGameplayModOp::Type> AttributesModOp=EGameplayModOp::Additive;
	
};

USTRUCT(BlueprintType)
struct FEffectStackInfo
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	EGameplayEffectStackingDurationPolicy EffectStackingDurationPolicy;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	EGameplayEffectStackingType  EffectStackingType;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	EGameplayEffectStackingExpirationPolicy EffectStackingExpirationPolicy;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	EGameplayEffectStackingPeriodPolicy EffectStackingPeriodPolicy;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 StackLimitCount;

	
};

USTRUCT(BlueprintType)
struct FAbilitySpecInfo
{
	GENERATED_BODY()

	
	UPROPERTY(BlueprintReadOnly)
	int32 level=1;

	UPROPERTY(BlueprintReadWrite)
	float CoolDownRemain=0;
	
	UPROPERTY(BlueprintReadOnly)
	float CoolDownDuration=0;
	
	UPROPERTY(BlueprintReadOnly)
	UAbilitySystemComponent* ASC;
	
	UPROPERTY(BlueprintReadOnly)
	TMap<FGameplayAttribute,float> Costs{};
	
	
};

UENUM(BlueprintType)
enum class  EFindAbilityType:uint8
{
	
	InputID,
	
	Tag,

	AbilityQuery
	
	
};
UENUM(BlueprintType)
enum class  EOverlapQueryType:uint8
{
	
	Channel,
	
	Object
	
	
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FonAttributeChanged,FGameplayAttribute ,Attribute,float,NewValue,float,OldValue);
	
UCLASS()
class ABILITYSYSTEMEXTRATOOLKIT_API UBFL_extraToolKit : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


public:


	//初始化能力系统组件的actorInfo
	UFUNCTION(BlueprintCallable,Category="AbilitySystem|extraToolKit",meta=(DefaultToSelf="OnwerActor"))
	static  void  InitAbilityActorInfo(AActor* OnwerActor,AActor* AvatorActor);

	
	/*
	 *
	 * ************* Ability **********************
	 */

	//制作一个能力句柄
	UFUNCTION(BlueprintCallable,Category="AbilitySystem|extraToolKit",meta=(DefaultToSelf="TargetActor"))
	static  FGameplayAbilitySpec MakeAbilitySpec(TSubclassOf<UGameplayAbility> AbilityClass,
	int32 Level,int32 InputID,UObject* Source);

	//授予一个能力句柄
	UFUNCTION(BlueprintCallable,Category="AbilitySystem|extraToolKit",meta=(DefaultToSelf="TargetActor"))
	static  FGameplayAbilitySpecHandle GiveAbilitySpec(AActor* TargetActor,const FGameplayAbilitySpec&  AbilitySpec);

	//授予能力并带有参数，列如等级，类型，绑定ID,源对象
	UFUNCTION(BlueprintCallable,Category="AbilitySystem|extraToolKit",meta=(DefaultToSelf="TargetActor"))
	static  FGameplayAbilitySpecHandle GiveAbilitySpecWithParam (AActor* TargetActor,TSubclassOf<UGameplayAbility> AbilityClass,
	int32 Level,int32 InputID,UObject* Source);

	//移除技能
	UFUNCTION(BlueprintCallable,Category="AbilitySystem|extraToolKit",meta=(DefaultToSelf="TargetActor"))
	static  void RemoveAbility(AActor* TargetActor,const FGameplayAbilitySpecHandle& AbilitySpecHandle);

	//移除技能
	UFUNCTION(BlueprintCallable,Category="AbilitySystem|extraToolKit",meta=(DefaultToSelf="TargetActor"))
	static  void RemoveAllAbilityByID(AActor* TargetActor,int32 InputID);

	//移除所有能力
	UFUNCTION(BlueprintCallable,Category="AbilitySystem|extraToolKit",meta=(DefaultToSelf="TargetActor"))
	static  void RemoveAllAbility(AActor* TargetActor);

	//设置当技能结束时移除技能
	UFUNCTION(BlueprintCallable,Category="AbilitySystem|extraToolKit",meta=(DefaultToSelf="TargetActor"))
	static  void SetRemoveAbilityOnEnd(AActor* TargetActor,FGameplayAbilitySpecHandle AbilitySpecHandle);
	
	//技能升级
	UFUNCTION(BlueprintCallable,Category="AbilitySystem|extraToolKit",meta=(DefaultToSelf="TargetActor"))
	static  void levelUp(AActor* TargetActor,const FGameplayAbilitySpecHandle& AbilitySpecHandle, int32 AddToLevel=1);

	
	//获取可激活的技能一些属性
	UFUNCTION(BlueprintCallable,BlueprintPure,Category="AbilitySystem|extraToolKit",meta=(DefaultToSelf="TargetActor"))
	static FAbilitySpecInfo GetInfoAtAbilitySpecHandle(AActor* TargetActor,const FGameplayAbilitySpecHandle&  AbilitySpecHandle);

	//查询可激活的能力
	UFUNCTION(BlueprintCallable,BlueprintPure,Category="AbilitySystem|extraToolKit",meta=(DefaultToSelf="TargetActor"))
	static TArray<FGameplayAbilitySpecHandle> FindAbilitySpecHandle (AActor* TargetActor,EFindAbilityType Type,
	FGameplayTagQuery TagQuery ,FGameplayTagContainer Tag, int32 InputID);
	
	/*
	 * ********************  Attribute  ********************
	 */
	

	//动态添加属性
	UFUNCTION(BlueprintCallable,Category="AbilitySystem|extraToolKit",meta=(DefaultToSelf="TargetActor"))
	static  void AddAttributesByClass(AActor* TargetActor,TSubclassOf<UAttributeSet> AttributeSetClass);

	//移除属性
	UFUNCTION(BlueprintCallable,Category="AbilitySystem|extraToolKit",meta=(DefaultToSelf="TargetActor"))
	static  void RemoveAttributesByClass(AActor* TargetActor,TSubclassOf<UAttributeSet> AttributeSetClass);

	//移除所有属性
	UFUNCTION(BlueprintCallable,Category="AbilitySystem|extraToolKit",meta=(DefaultToSelf="TargetActor"))
	static  void RemoveAllAttributes(AActor* TargetActor);

	// 直接修改属性值，不用GE
	UFUNCTION(BlueprintCallable,Category="AbilitySystem|extraToolKit",meta=(DefaultToSelf="TargetActor"))
	static void ApplyModToAttribute(AActor* TargetActor, FGameplayAttribute Attribute, TEnumAsByte<EGameplayModOp::Type> ModifierOp, float ModifierMagnitude);

	//查询拥有的属性,返回一个带值的Map
	UFUNCTION(BlueprintCallable,BlueprintPure,Category="AbilitySystem|extraToolKit",meta=(DefaultToSelf="TargetActor"))
	static    TMap<FGameplayAttribute,float> GetAllAttribute(AActor* TargetActor);

	// 返回一些关于属性变化的委托
	/*UFUNCTION(BlueprintCallable,Category="AbilitySystem|extraToolKit",meta=(DefaultToSelf="TargetActor"))
	static FonAttributeChanged&    GetAttributeChangedDelegate(AActor* TargetActor);
	*/
	
	

	/*
	 * ********************  GameplayEffect  ********************
	 */

	//设置EGameplayEffectReplicationMode，即GE的复制模式：Full,mix,Min.
	UFUNCTION(BlueprintCallable,Category="AbilitySystem|extraToolKit",meta=(DefaultToSelf="TargetActor"))
	static void SetReplicationMode(AActor* TargetActor,EGameplayEffectReplicationMode NewReplicationMode);

	//动态创建GE（运行时生成）
	UFUNCTION(BlueprintCallable,Category="AbilitySystem|extraToolKit",meta=(DefaultToSelf="TargetActor"))
	static FActiveGameplayEffectHandle MakeGameplayEffectCompletedCustomToApply(AActor* TargetActor,int32 ApplyLevel,EGameplayEffectDurationType DurationType,FScalableFloat DurationMagnitudefloat,
							 FScalableFloat PeriodMagnitudefloat,FGameplayTag GrantTag,TArray<FAttributeModifyInfo> AttributeModifyInfos,FEffectStackInfo  EffectStackInfo);


	//生成EffectContext，用于执行GE所需，可以设置更多的参数
	UFUNCTION(BlueprintCallable,BlueprintPure,Category="AbilitySystem|extraToolKit",meta=(DefaultToSelf="Instigator"))
	static FGameplayEffectContextHandle  MakeEffectContextWithParam(AActor*Instigator ,AActor*Casuer,UObject* SourceObj=nullptr,FVector Orgin=FVector::ZeroVector,const FHitResult&HitResult=FHitResult());

	//给目标施加GE效果
	UFUNCTION(BlueprintCallable,Category="AbilitySystem|extraToolKit",meta=(DefaultToSelf="Who"))
	static FActiveGameplayEffectHandle ApplyGEToTarget(AActor* Who,AActor* Target,TSubclassOf<UGameplayEffect> EffectClass,int32 level);

	//通过activeeffecthandle得到ActiveGameplayEffect
	UFUNCTION(BlueprintCallable,BlueprintPure,Category="AbilitySystem|extraToolKit",meta=(DefaultToSelf="Target"))
	static   FActiveGameplayEffect GetActiveEffect(AActor*Target,const FActiveGameplayEffectHandle& ActiveGameplayEffectHandle);


	//通过GameplayTag得到ActiveGameplayEffect
	UFUNCTION(BlueprintCallable,BlueprintPure,Category="AbilitySystem|extraToolKit",meta=(DefaultToSelf="Target"))
	static   TArray<FActiveGameplayEffectHandle> GetActiveEffectwithAllTag(AActor* Target, FGameplayTagContainer Tag);

	/*
	UFUNCTION(BlueprintCallable,BlueprintPure,Category="AbilitySystem|extraToolKit",meta=(DefaultToSelf="Target"))
	static TArray<FActiveGameplayEffectHandle>  GetAllActiveEffects(AActor*Target);
	*/

	// 得到类默认对象
	UFUNCTION(BlueprintCallable,BlueprintPure,Category="AbilitySystem|extraToolKit",meta=(DefaultToSelf="Target"))
	static  const UGameplayEffect* GetGameplayEffectCDO(AActor* Target,const FActiveGameplayEffectHandle Handle);


	// 得到类默认对象
	UFUNCTION(BlueprintCallable,BlueprintPure,Category="AbilitySystem|extraToolKit",meta=(DefaultToSelf="Target"))
	static  const  UGameplayEffect* GetGameplayEffectCDOByClass(TSubclassOf<UGameplayEffect> Effect);


	/*
	 * TODO:对于GE的移除，和动态修改功能，GE设计是自动根据执行策略移除和刷新状态的，更多扩展需求尚不明确
	 */

	

	//查询GE对目标某一属性的修改值，包括所有的修改器和执行器的情况，返回的为聚合所有计算的最终值
	UFUNCTION(BlueprintCallable,BlueprintPure,Category="AbilitySystem|extraToolKit",meta=(DefaultToSelf="SourceActor"))
	static float GetModifyValueToAttributeByEffecDef(AActor* SourceActor,UGameplayEffect* EffecDef ,FGameplayAttribute Attribute,int32 level);

	//查询GE对目标某一属性的修改值，包括所有的修改器和执行器的情况，返回的为聚合所有计算的最终值
	UFUNCTION(BlueprintCallable,BlueprintPure,Category="AbilitySystem|extraToolKit",meta=(DefaultToSelf="SourceActor"))
	static float GetModifyValueToAttributeByEffecClass(AActor* SourceActor,TSubclassOf<UGameplayEffect>Effecclass ,FGameplayAttribute Attribute,int32 level);

	//返回GE修改的涉及所有属性
	UFUNCTION(BlueprintCallable,BlueprintPure,Category="AbilitySystem|extraToolKit",meta=(DefaultToSelf="TargetActor"))
	static TSet<FGameplayAttribute>  GetAllModifyAttributeByEffecDef(UGameplayEffect*EffecDef);

	//返回GE修改的涉及所有属性
	UFUNCTION(BlueprintCallable,BlueprintPure,Category="AbilitySystem|extraToolKit",meta=(DefaultToSelf="TargetActor"))
	static TSet<FGameplayAttribute>  GetAllModifyAttributeByEffecClass(TSubclassOf<UGameplayEffect>Effecclass );

	
	//返回GE对目标所有属性修改的结果 TMap<FGameplayAttribute ,float >
	UFUNCTION(BlueprintCallable,BlueprintPure,Category="AbilitySystem|extraToolKit",meta=(DefaultToSelf="TargetActor"))
	static TMap<FGameplayAttribute ,float > GetAllModifyInfosByEffectDef(AActor* TargetActor,UGameplayEffect*EffecDef ,int32 level);

	//返回GE对目标所有属性修改的结果 TMap<FGameplayAttribute ,float >
	UFUNCTION(BlueprintCallable,BlueprintPure,Category="AbilitySystem|extraToolKit",meta=(DefaultToSelf="TargetActor"))
	static TMap<FGameplayAttribute ,float > GetAllModifyInfosByEffectClass(AActor* TargetActor,TSubclassOf<UGameplayEffect> Effecclass ,int32 level);

	//判断能否应用GE
	UFUNCTION(BlueprintCallable,BlueprintPure,Category="AbilitySystem|extraToolKit",meta=(DefaultToSelf="TargetActor"))
	static bool  CanApplyAttributeModifiers(AActor* TargetActor,const UGameplayEffect *GameplayEffect, float Level, const FGameplayEffectContextHandle& EffectContext);


	/*
	 *
	 * ****************** TargetData  ****************
	 */

	//向服务器发送数据
	UFUNCTION(BlueprintCallable,Category="AbilitySystem|extraToolKit",meta=(DefaultToSelf="TargetActor"))
	static void SendTargetDataToServer(AActor* TargetActor,FGameplayAbilitySpecHandle AbilitySpecHandle,FGameplayAbilityTargetDataHandle TargetData);



	/*
	 * *************  世界检测（射线）对象      *************
	 */

	/*
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="AbilitySystem|extraToolKit",meta=(DefaultToSelf="TargetActor"))
	static TSet<AActor*> FindOverlapActorsByChannelOrObject(AActor* TargetActor,EOverlapQueryType OverlapQueryType,
		FVector Origin,ECollisionChannel CollisionChannel,FCollisionObjectQueryParams ObjectQueryParams,
		ECollisionShape::Type CollisionShapeType, FVector Extend);
		*/


/*
 * ******************* 回放系统*******************
 */

	/* ***** 常用的回放相关命令 *************
	- "PlayDemo [ReplayName]"    // 播放回放
	- "StopDemo"                 // 停止回放  
	- "StartRecording [Name]"    // 开始录制
	- "StopRecording"            // 停止录制
	- "Demo.Pause"              // 暂停回放
	- "Demo.Speed [Value]"*/
	
	UFUNCTION(BlueprintCallable,Category="replay",meta=(DefaultToSelf="WorldContextObject"))
	static void StartRecordingReplay(UObject* WorldContextObject,FString ReplayName="Demo_01");

	UFUNCTION(BlueprintCallable,Category="replay",meta=(DefaultToSelf="WorldContextObject"))
	static void StopRecordReplay(UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable,Category="replay",meta=(DefaultToSelf="WorldContextObject"))
	static void PlayRecordReplay(UObject* WorldContextObject,FString ReplayName);
	
	UFUNCTION(BlueprintCallable,Category="replay",meta=(DefaultToSelf="WorldContextObject"))
	static void GotoTimeInSeconds(UObject* WorldContextObject,float  TimeInSeconds );

	UFUNCTION(BlueprintCallable,Category="replay",meta=(DefaultToSelf="WorldContextObject"))
	static void PauseRecordingReplay(UObject* WorldContextObject,bool bPauseRecording );

	UFUNCTION(BlueprintCallable,Category="replay",meta=(DefaultToSelf="WorldContextObject"))
	static void StopReplay(UObject* WorldContextObject );


	/*
	 * ****************** 数学计算 *********************
	 * 
	 */


	// 计算当前actor前方锥形范围内等间隔的点
	UFUNCTION(BlueprintCallable,Category="AbilitySystem|extraToolKit",meta=(DefaultToSelf="TargetActor"))
	static TArray<FVector> GetPointsInConeByEnvQueryGenerator(AActor* TargetActor, float InConeDegreesValue=60,
		float AlignedPointDistanceValue=20,float AngleStepValue=20,float GenerationRange=20);

	// 计算当前actor前方锥形范围内等间隔的点
	UFUNCTION(BlueprintCallable,Category="AbilitySystem|extraToolKit",meta=(DefaultToSelf="TargetActor"))
	static TArray<FVector> GetPointsInConeByEnvQueryGeneratorWithParam(FVector ActorLocation, FVector ForwardVector,float InConeDegreesValue=60,
		float AlignedPointDistanceValue=20,float AngleStepValue=20,float GenerationRange=20);

	
};




