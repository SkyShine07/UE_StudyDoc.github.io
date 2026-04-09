// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "Components/ActorComponent.h"
#include "ItemFragment.h"
#include "StructUtils/InstancedStruct.h"
#include "ActiveGameplayEffectHandle.h"

#include "CommonInvenstoryComponent.generated.h"




USTRUCT(BlueprintType)
struct FItemDef:public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ID;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FGameplayTag  ItemType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TInstancedStruct<FItemFragmentBase>> ItemFragments;
	
	bool operator==(const FItemDef& ItemDef) const
	{
		return ItemDef.ID==ID;
	};

	// 获得ItemFragment的模板函数
	template<class T>
	T* GetItemFragmentRefByTag(FGameplayTag Tag,bool& bHasFrag )
	{
		if (ItemFragments.Num()<=0)
		{
			bHasFrag=false;
			return nullptr;
		}
		
		for (auto& ItemFrag : ItemFragments)
		{
			if (ItemFrag.Get<FItemFragmentBase>().FragmentTag==Tag)
			{
				bHasFrag=true;
				return ItemFrag.GetMutablePtr<T>();
			};
		}
		
		bHasFrag=false;
		return nullptr;
		
	};
	
	
};


USTRUCT(BlueprintType)
struct FEquipmentSlot
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag EquipmentSlot;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText DisplayNameSlot;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 ItemID=-1;
	
	UPROPERTY(BlueprintReadOnly)
	USkeletalMeshComponent* EquipmentSKC;
	
	UPROPERTY(BlueprintReadOnly)
	UStaticMeshComponent* EquipmentSM;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemNumChanged,int32 ,ItemID,int32,AddedNum);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemStackFull,int32 ,ItemID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemSlotChanged,FEquipmentSlot ,EquipmentSlot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemSwap) ;

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class COMMONINVENTORYSYSTEM_API UCommonInvenstoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UCommonInvenstoryComponent();
	

	

	
	//******************    背包Items的 增，删，改，查           ****************************
	

	
	// 改变Item数量
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool TryChangedItemNum(int32 ItemID , int32 Quantity = 1);
	
	// 交换Item位置
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void  TrySwapItemByIndex(int32 ItemID_1_index,int32 ItemID_2_index);
	
	
	// 添加物品到背包
	bool TryAddItem(int32 ItemID , int32 Quantity = 1);
	bool TryRemoveItem(int32 ItemID, int32 Quantity = 1);
	
	
	// 排序
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SortItem();
	
	// 查询Item 所有类型
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	TArray<FGameplayTag>  GetOwnerAllItemTypes() ;
	
	// 查询Item
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "Inventory")
	void   GetAllItemsByType( FGameplayTag ItemType,TArray<FItemDef>& OutItems) ;

	// 检查物品是否存在
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "Inventory")
	bool HasItem(int32 ItemID, int32& ItemQuantity ) ;
	
	// 检查物品是否可以堆叠
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "Inventory")
	bool  CanStackableItem(int32 ItemID ) ;
	
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "Inventory")
	bool  HasAlreadyMaxStack(int32 ItemID) ;
	

	
	
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "Inventory")
	FItemDef GetItemDefByIDFromDT(int32 ItemID);
	
	// 获取 实例Item的显示属性片段
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool  GetItemFragment_Display(int32 ItemID,FItemFragment_Display& ItemFragment_Display)  ;
	
	// 获取 实例Item的堆叠属性片段
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetItemFragment_Stackable(int32 ItemID,FItemFragment_Stackable& ItemFragment_Stackable)  ;
	
	
	// 获取 背包中实例Item
	FItemDef* GetItemByID(int32 ItemID);
	int32  GetItemIndexByID(int32 ItemID);
	
	//****************************  Item装备 装备功能 ******************
	
	
	// 装备装备、卸载装备
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool TryEquipItem(int32 ItemID );
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool TryUnEquipItem(int32 ItemID );
	
	//获取装备槽信息
	FEquipmentSlot*  GetEquipmentSlot(FGameplayTag SlotTag);
	
	UFUNCTION(BlueprintCallable, BlueprintPure,Category = "Inventory")
	bool CanEquipItem(int32 ItemID);
	
	UFUNCTION(BlueprintCallable, BlueprintPure,Category = "Inventory")
	bool HasEquippedItem(int32 ItemID);
	
	
	
	//    *********  Comsue Item 的使用
	
	/*
	//使用 Item
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool  TryUseItem(int32 ItemID);
	*/
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool TryConsumeItem(int32 ItemID , int32 Quantity = 1);
	
	
	//*******  批量应用GE，授予GA *********
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ApplyGEsAndGrantAbilitiesToOwner(const TArray<TSoftClassPtr<UGameplayEffect>>& GEs,const TArray<TSoftClassPtr<UGameplayAbility>>& Abilities,int32 ItemID);
	
	// 获取Item的属性静态数据片段
	FItemFragmentBase* GetItemFragmentDefByTagFromDT(int32 ItemID, const FGameplayTag& FragmentTag);
	
	// 获得背包中实例Item对应Tag的ItemFragment的模板函数
	template<class T>
	T* GetItemFragmentRefByTag(int32 ItemID,FGameplayTag Tag,bool& bHasFrag )
	{
		FItemDef* ItemDef=GetItemByID(ItemID);
		if (ItemDef==nullptr) return nullptr;

		return ItemDef->GetItemFragmentRefByTag<T>(Tag,bHasFrag);
		
	}
	
	
protected:


	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UDataTable* ItemDataTable;
	

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	TArray<FItemDef> Items;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FEquipmentSlot> EquipmentSlots;
	
private:
	
	TMap<FActiveGameplayEffectHandle,int32> AppliedEffects;
	TMap<FGameplayAbilitySpecHandle,int32> GrantAbilities;
	
	
public:
	
	UPROPERTY(BlueprintAssignable)
	FOnItemNumChanged OnItemNumChanged;
	
	UPROPERTY(BlueprintAssignable)
	FOnItemStackFull OnItemStackFull;

	UPROPERTY(BlueprintAssignable)
	FOnItemSwap OnItemSwap;
	
	UPROPERTY(BlueprintAssignable)
	FOnItemSlotChanged OnItemEquiped;
	
	UPROPERTY(BlueprintAssignable)
	FOnItemSlotChanged OnItemUnEquiped;
};



