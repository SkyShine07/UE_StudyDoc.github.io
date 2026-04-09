#pragma once

#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"

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


	UE_DECLARE_GAMEPLAY_TAG_EXTERN(DurabilityFragmentTag);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(StackableFragmentTag);
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
	TSoftClassPtr<UAnimInstance> LinkABP;
	
	
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

// 耐久度片段
USTRUCT(BlueprintType)
struct FItemFragment_Durability: public FItemFragmentBase
{
	GENERATED_BODY()

	FItemFragment_Durability(){FragmentTag=ItemFragmentTag::DurabilityFragmentTag;};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentDurability = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxDurability = 1;
	
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


// 任务片段

UENUM(BlueprintType)
enum class EQuestItemRole : uint8
{
	Objective,      // 任务目标物品（需要收集/使用）
	Reward,         // 任务奖励物品
	Key,            // 任务钥匙（开启特定区域）
	Trigger,        // 触发任务对话或事件
	Misc
};

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

	// 3. 物品在任务中的作用类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EQuestItemRole Role = EQuestItemRole::Objective;  // 目标、奖励、钥匙、触发器等

	// 4. 任务物品是否需要被消耗（例如：收集10个草药，每个使用后消耗）
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bConsumedOnQuestComplete = true;

	// 5. 是否必须保留才能完成任务（例如：任务关键道具不能丢弃）
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRequiredForCompletion = true;

	// 6. 任务物品的堆叠限制（通常任务物品不可堆叠或有限制）
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxStackSizeForQuest = 1;

	// 7. 提示文本（当任务物品无法丢弃/使用时显示）
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText QuestHint;
	
	/*           动态数据    */
	// 进度
	UPROPERTY()
	int32 CurrentProgress = 0;

	// 是否已触发任务更新（避免重复触发）
	UPROPERTY()
	bool bQuestUpdateTriggered = false;
	
};

