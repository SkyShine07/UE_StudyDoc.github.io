#pragma once

#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"
#include "StructUtils/InstancedStruct.h"

#include "ItemFragment.generated.h"

class UGameplayAbility;
class UGameplayEffect;


namespace ItemFragmentTag
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ConsumableFragmentTag);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(EquipmentableFragmentTag);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(DisplayFragmentTag);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(TradableFragmentTag);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(RestrictionsFragmentTag);
	

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(StackableFragmentTag);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(CraftableFragmentTag);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(DecomposableFragmentTag);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(DiscardableFragmentTag);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(UpgradableFragmentTag);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(AcquisitionMethodFragmentTag);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(QuestFragmentTag);
	
	
}






// 片段基类结构
USTRUCT(BlueprintType)
struct FItemFragmentBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag FragmentTag;

};

USTRUCT(BlueprintType)
struct FItemFragment_Effect : public FItemFragmentBase
{
	GENERATED_BODY()

	// 使用效果
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSoftClassPtr<UGameplayEffect>> Effects;
	
	// 授予能力
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSoftClassPtr<UGameplayAbility>> Abilities;
	

};
// 消耗品片段
USTRUCT(BlueprintType)
struct FItemFragment_Consumable : public FItemFragment_Effect
{
	GENERATED_BODY()
    
	FItemFragment_Consumable(){FragmentTag=ItemFragmentTag::ConsumableFragmentTag;};
	
	// 使用时是否消耗物品
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bConsumeOnUse = true;
	
	

};



// 基础显示属性片段
USTRUCT(BlueprintType)
struct FItemFragment_Display : public FItemFragmentBase
{
	GENERATED_BODY()
    
	FItemFragment_Display(){FragmentTag=ItemFragmentTag::DisplayFragmentTag;};
	
	// 显示名称 (多语言支持)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText DisplayName;
    
	// 详细描述
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Description;
    
	// 物品图标
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> Icon;
    
	// 稀有度等级
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 RarityLevel = 0;
   
};



// 堆叠属性片段
USTRUCT(BlueprintType)
struct FItemFragment_Stackable: public FItemFragmentBase
{
	GENERATED_BODY()

	FItemFragment_Stackable(){FragmentTag=ItemFragmentTag::StackableFragmentTag;};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentStack = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxStackSize = 1;
};


// 装备片段
USTRUCT(BlueprintType)
struct FItemFragment_Equipable : public FItemFragment_Effect
{
	GENERATED_BODY()

	FItemFragment_Equipable(){FragmentTag= ItemFragmentTag::EquipmentableFragmentTag;};
	// 装备槽位
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag EquipmentSlot;
	
	// 3D预览模型
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<USkeletalMesh> SK_EquipmentMesh;
    
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UStaticMesh> SM_EquipmentMesh;
	
	// 装备的Socket
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName AttachSocket;
	
	//装备切换的链接动画示例
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UAnimInstance> LinkABP;
	
	
};


// 可合成的
USTRUCT(BlueprintType)
struct FItemFragment_Craftable : public FItemFragmentBase
{
	GENERATED_BODY()
	
	FItemFragment_Craftable(){FragmentTag=ItemFragmentTag::CraftableFragmentTag;};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TargetItemID=-1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int32,int32> RequireItemIDAndCountMap;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DuratingTime=3.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SuccessRate=1.f;
	
};

//  可分解的
USTRUCT(BlueprintType)
struct FItemFragment_Decomposable: public FItemFragmentBase
{
	GENERATED_BODY()
	
	FItemFragment_Decomposable(){FragmentTag=ItemFragmentTag::DiscardableFragmentTag;};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int32,int32> DecomposeItemIDs;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DuratingTime=2.f;
	
	
	
};

//  可丢弃的
USTRUCT(BlueprintType)
struct FItemFragment_Discardable: public FItemFragmentBase
{
	GENERATED_BODY()
	
	FItemFragment_Discardable(){FragmentTag=ItemFragmentTag::DiscardableFragmentTag;};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<AActor> DropItemClass;
	

	
};

//  可升级的

USTRUCT(BlueprintType)
struct FItemUpGradeItemInfo
{
	GENERATED_BODY()
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int32,int32> MaterialItemIDAndCountMap;
	
};

USTRUCT(BlueprintType)
struct FItemFragment_Upgradable: public FItemFragmentBase
{
	GENERATED_BODY()
	
	FItemFragment_Upgradable(){FragmentTag=ItemFragmentTag::UpgradableFragmentTag;};	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentLevel = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxLevel = 99;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int32,FItemUpGradeItemInfo>  UpgradeInfoAtLevelMap;
	
	
};

//  获得方式
USTRUCT(BlueprintType)
struct FItemFragment_AcquisitionMethod: public FItemFragmentBase
{
	GENERATED_BODY()
	
	FItemFragment_AcquisitionMethod(){FragmentTag=ItemFragmentTag::AcquisitionMethodFragmentTag;};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DropRate=0.5;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32  QuestID=10001;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FText>  OtherMethDesc;
	
	
};


// 任务片段

USTRUCT(BlueprintType)
struct FItemFragment_Quest : public FItemFragmentBase
{
	GENERATED_BODY()
	FItemFragment_Quest() { FragmentTag=ItemFragmentTag::QuestFragmentTag; }

	// 1. 关联的任务ID（可能是主任务或支线任务的唯一标识）
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName QuestID;

	// 2. 任务步骤/阶段索引（如果任务有多个阶段）
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 QuestStep = 0;


	// 4. 任务物品是否需要被消耗（例如：收集10个草药，每个使用后消耗）
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bConsumedOnQuestComplete = true;

	// 5. 是否必须保留才能完成任务（例如：任务关键道具不能丢弃）
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRequiredForCompletion = true;
	

	// 7. 提示文本（当任务物品无法丢弃/使用时显示）
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText QuestHint;
	

	
};



// 交易属性片段
USTRUCT(BlueprintType)
struct FItemFragment_Tradable: public FItemFragmentBase
{
	GENERATED_BODY()

	FItemFragment_Tradable(){FragmentTag=ItemFragmentTag::TradableFragmentTag;};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BuyPrice=0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SellPrice=0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag CurrencyTag; // 金币，砖石等
	
};

// 限制属性片段
USTRUCT(BlueprintType)
struct FItemFragment_Restrictions: public FItemFragmentBase
{
	GENERATED_BODY()
	
	FItemFragment_Restrictions(){FragmentTag=ItemFragmentTag::RestrictionsFragmentTag;};
	
	

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MinLevel=0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTagContainer RoleTypes;
	
};


UCLASS()
class UItemFragmentStatic:public UObject
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable,BlueprintPure)
	static   bool GetItemFragmentByTag( TArray<TInstancedStruct<FItemFragmentBase>> Fragments,FGameplayTag Tag ,TInstancedStruct<FItemFragmentBase>&ItemFragment )
	{
		for (auto Element : Fragments)
		{
			if (Element.Get<FItemFragmentBase>().FragmentTag==Tag)
			{
				ItemFragment=Element;
			
				return true;
			}
		}
		
			return false;
		
	}; 
	
	
	
	
};
