// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonInvenstoryComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "NativeGameplayTags.h"
#include "ItemFragment.h"


UCommonInvenstoryComponent::UCommonInvenstoryComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

}

bool UCommonInvenstoryComponent::TryChangedItemNum(int32 ItemID, int32 Quantity)
{
	bool ReturnValue=false;
	
	if (Quantity>0)
	{
		ReturnValue=TryAddItem( ItemID, Quantity);
		if (ReturnValue)
		{
			int32 ItemNum;
			HasItem(ItemID,ItemNum);
			OnItemNumChanged.Broadcast(ItemID,ItemNum,Quantity);
		}
	}

	else
	{
		ReturnValue=TryRemoveItem( ItemID, Quantity*-1);
		if (ReturnValue)
		{
			int32 ItemNum;
			HasItem(ItemID,ItemNum);
			OnItemNumChanged.Broadcast(ItemID,ItemNum,Quantity);
		}
	}
	
	return ReturnValue;
	
}

void  UCommonInvenstoryComponent::TrySwapItemByIndex(int32 ItemID_1_index , int32 ItemID_2_index )
{
	Items.Swap(ItemID_1_index,ItemID_1_index);
	OnItemSwap.Broadcast();
	
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1,3,FColor::Green,
		FString::Printf(TEXT("%d and %d Has Swap!!"),ItemID_1_index,ItemID_2_index));
	}
	
	//UE_LOG(LogTemp,Warning,TEXT("%d and %d Has Swap!!"),ItemID_1_index,ItemID_2_index);
	
}

bool  UCommonInvenstoryComponent::TryUseItem(int32 ItemID)
{
	int32 ItemCurrentQuantity;
	if (!HasItem(ItemID,ItemCurrentQuantity)) return false;

	TryEquipItem(ItemID);

	return false;
}


bool UCommonInvenstoryComponent::TryAddItem(int32 ItemID,int32 Quantity)
{
	
	int32 ItemCurrentQuantity;
	//bool bReturnValue=false;
	
	if (HasItem(ItemID,ItemCurrentQuantity))
	{
		// 背包中已经存在且满时，广播通知
		
		if (CanStackableItem(ItemID)&&HasAlreadyMaxStack(ItemID))
		{
			OnItemStackFull.Broadcast(ItemID);
		}
		
		// 背包中已经存在且未满时，继续添加堆叠物品
		if (CanStackableItem(ItemID)&&!HasAlreadyMaxStack(ItemID))
		{
			FItemDef* Item=GetItemByID(ItemID);
			if (!Item) return false;
			bool bHasFrag;
			FItemFragment_Stackable* Fragment_Stackable=Item->GetItemFragmentRefByTag<FItemFragment_Stackable>(ItemFragmentTag::StackableFragmentTag,bHasFrag);
			if (bHasFrag&&Fragment_Stackable)
			{
				Fragment_Stackable->CurrentStack+=Quantity;
				if (Fragment_Stackable->CurrentStack>=Fragment_Stackable->MaxStackSize)
				{
					Fragment_Stackable->CurrentStack=Fragment_Stackable->MaxStackSize;
					OnItemStackFull.Broadcast(ItemID);
				}
				return true;
		
			}
		}
	
		//继续添加不可堆叠物品时
		if (!CanStackableItem(ItemID))
		{
			FItemDef ItemDef=GetItemDefByIDFromDT(ItemID);

			for (int i = 0; i < Quantity; ++i)
			{
				Items.Add(ItemDef);
			}
			
			return true;
			
		}
		
		
		
	}

	else
	{
		// 背包中未存在，第一次添加可堆叠物品时
		if (CanStackableItem(ItemID))
		{
			FItemDef ItemDef=GetItemDefByIDFromDT(ItemID);
			bool bHasFrag;
			FItemFragment_Stackable* Fragment_Stackable=ItemDef.GetItemFragmentRefByTag<FItemFragment_Stackable>(ItemFragmentTag::StackableFragmentTag,bHasFrag);
			if (bHasFrag&&Fragment_Stackable)
			{
				Fragment_Stackable->CurrentStack=Quantity;
				Items.Add(ItemDef);
				return true;
			}
		}
	
		//继续添加不可堆叠物品时
		else
		{
			FItemDef ItemDef=GetItemDefByIDFromDT(ItemID);

			for (int i = 0; i < Quantity; ++i)
			{
				Items.Add(ItemDef);
			}
			
			return true;
			
			
		}
		
	}
	
	return false;
	
}

bool UCommonInvenstoryComponent::TryRemoveItem(int32 ItemID, int32 Quantity)
{
	int32 ItemCurrentQuantity;
	if (!HasItem(ItemID,ItemCurrentQuantity)||ItemCurrentQuantity==0||Quantity==0) return false;
	
	Quantity=FMath::Clamp(Quantity,0,ItemCurrentQuantity);

	
	
	// 可堆叠的item 移除
	if (CanStackableItem(ItemID))
	{
		FItemDef* Item=GetItemByID(ItemID);
		if (!Item) return false;
		
		bool bHasFrag;
		FItemFragment_Stackable* ItemFragment_Stackable=Item->GetItemFragmentRefByTag<FItemFragment_Stackable>(ItemFragmentTag::StackableFragmentTag,bHasFrag);
		
		if (bHasFrag && ItemFragment_Stackable)
		{
			ItemFragment_Stackable->CurrentStack-=Quantity;
		
			if (ItemFragment_Stackable->CurrentStack<=0 &&!Items.IsEmpty())
			{
				
				//Items.Remove(*Item);
				int32 Index=Items.IndexOfByPredicate([&](const FItemDef& localItem)
				{
					return localItem.ID==ItemID;
				});
		
				Items.RemoveAt(Index);
				
			}
			
			
			return true;
		}
		
		return false;
		
	};
	
	// 不可堆叠的item 移除
	if (!CanStackableItem(ItemID))
	{
		for (int32 i =0; i < Quantity; ++i)
		{
			/*Items.FindByPredicate([&](const FItemDef& Item)
			{
				return Item.ID==ItemID;
			});*/

			int32 Index=Items.IndexOfByPredicate([&](const FItemDef& Item)
			{
				return Item.ID==ItemID;
			});
			
			if (!Items.IsValidIndex(Index)) return false;
			Items.RemoveAt(Index);

			/*if (GetItemByID(ItemID))
			{
				Items.Remove(*GetItemByID(ItemID));
			}*/
			
		};
		
		return true;
		
	};

	return false;
	
}

bool UCommonInvenstoryComponent::TryEquipItem(int32 ItemID)
{
	bool bHasFrag;
	FItemFragment_Equipable* ItemFragment_Equipable= GetItemFragmentRefByTag<FItemFragment_Equipable>(ItemID,ItemFragmentTag::EquipmentableFragmentTag,bHasFrag);
	if (!ItemFragment_Equipable||!GetOwner()) return false;
	
	ACharacter* Character=Cast<ACharacter>(GetOwner());
	if (!Character) return false;
	USkeletalMeshComponent* CharactorSKC=Character->GetMesh();
	UAbilitySystemComponent* ASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Character);
	
	FEquipmentSlot* EquipmentSlot=GetEquipmentSlot(ItemFragment_Equipable->EquipmentSlot);
	if (EquipmentSlot->ItemID==ItemID) return false;
	
	// 卸载旧装备，更新装备ID
	TryUnEquipItem(EquipmentSlot->ItemID);
	EquipmentSlot->ItemID=ItemID;
	
	// 生成武器模型
	if (ItemFragment_Equipable->EquipmentMesh.IsValid())
	{
		
		USkeletalMeshComponent* EquipmentSKC=NewObject<USkeletalMeshComponent>();
		EquipmentSKC->RegisterComponent();
	
		EquipmentSKC->SetSkeletalMesh(ItemFragment_Equipable->EquipmentMesh.LoadSynchronous());
		EquipmentSKC->SetWorldLocation(CharactorSKC->GetComponentLocation());
		EquipmentSKC->SetLeaderPoseComponent(CharactorSKC);

		if (ItemFragment_Equipable->AttachSocket.IsValid())
		{
			EquipmentSKC->AttachToComponent(CharactorSKC,FAttachmentTransformRules::SnapToTargetIncludingScale,ItemFragment_Equipable->AttachSocket);

		}
	
		EquipmentSlot->EquipmentSKC=EquipmentSKC;
	
	}

	
	// 替换动画示例
	if (ItemFragment_Equipable->LinkABP.IsValid())
	{
		CharactorSKC->LinkAnimClassLayers(ItemFragment_Equipable->LinkABP.LoadSynchronous());
	};
	
	
	// 授予GE和GA

	if (ItemFragment_Equipable->EquipmentEffects.Num()>0)
	{
		for (auto effect : ItemFragment_Equipable->EquipmentEffects)
		{
			if (effect.IsValid()&&ASC)
			{
				
				UGameplayEffect* EffectObj=effect.LoadSynchronous()->GetDefaultObject<UGameplayEffect>();
				FActiveGameplayEffectHandle ActiveGameplayEffectHandle=ASC->ApplyGameplayEffectToSelf(EffectObj,1,ASC->MakeEffectContext());
				AppliedEffects.Add(ActiveGameplayEffectHandle,ItemID);
			}
		}
	}
	
	if (ItemFragment_Equipable->EquipmentGrantedAbilities.Num()>0)
	{
		for (auto AbilityClass : ItemFragment_Equipable->EquipmentGrantedAbilities)
		{
			if (AbilityClass.IsValid()&&ASC)
			{
				FGameplayAbilitySpec AbilitySpec{AbilityClass.LoadSynchronous(),1};
				FGameplayAbilitySpecHandle GiveAbility=ASC->GiveAbility(AbilitySpec);
				
				GrantAbilities.Add(GiveAbility,ItemID);
				
			}
		}
	}
	
	OnItemEquiped.Broadcast(*EquipmentSlot);
	
	return true;
}

bool UCommonInvenstoryComponent::TryUnEquipItem(int32 ItemID)
{
	bool bHasFrag;
	FItemFragment_Equipable* ItemFragment_Equipable= GetItemFragmentRefByTag<FItemFragment_Equipable>(ItemID,ItemFragmentTag::EquipmentableFragmentTag,bHasFrag);
	if (!ItemFragment_Equipable||!GetOwner()) return false;
	
	ACharacter* Character=Cast<ACharacter>(GetOwner());
	if (!Character) return false;
	USkeletalMeshComponent* CharactorSKC=Character->GetMesh();
	UAbilitySystemComponent* ASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Character);
	
	//更新装备槽信息
	FEquipmentSlot* EquipmentSlot=GetEquipmentSlot(ItemFragment_Equipable->EquipmentSlot);
	EquipmentSlot->ItemID=-1;
	if (EquipmentSlot->EquipmentSKC)
	{
		EquipmentSlot->EquipmentSKC->DetachFromComponent( FDetachmentTransformRules::KeepWorldTransform);
		EquipmentSlot->EquipmentSKC->DestroyComponent();
	}
	
	// 断开动画示例，移除GE和GA
	CharactorSKC->UnlinkAnimClassLayers(ItemFragment_Equipable->LinkABP.Get());

	if (!AppliedEffects.IsEmpty())
	{
		for (auto effectHandlePair : AppliedEffects)
		{
			if (effectHandlePair.Value==ItemID && ASC)
			{
				ASC->RemoveActiveGameplayEffect(effectHandlePair.Key);
				AppliedEffects.Remove(effectHandlePair.Key);
			}
			
		}
	}
	
	if (!GrantAbilities.IsEmpty())
	{
		for (auto AbilityPair : GrantAbilities)
		{
			if (AbilityPair.Value==ItemID&&ASC)
			{
				ASC->ClearAbility(AbilityPair.Key);
				GrantAbilities.Remove(AbilityPair.Key);
			}
		}
	}
	
	OnItemUnEquiped.Broadcast(*EquipmentSlot);
	
	return true;	
	
}



FEquipmentSlot* UCommonInvenstoryComponent::GetEquipmentSlot(FGameplayTag SlotTag)
{
	for (FEquipmentSlot& EquipmentSlotInfo : EquipmentSlots)
	{
		if (EquipmentSlotInfo.EquipmentSlot==SlotTag)
		{
			return &EquipmentSlotInfo;
		}
		
	}
	
	return nullptr;
	
}

bool UCommonInvenstoryComponent::HasItem(int32 ItemID, int32& ItemQuantity) 
{
	int32 Num=0;

	//item 可堆叠，返回堆叠值
	if (CanStackableItem(ItemID))
	{
		bool   bHasFrag;
		
		if (FItemDef* Item=GetItemByID(ItemID))
		{
			FItemFragment_Stackable* ItemFragment_Stackable=Item->GetItemFragmentRefByTag<FItemFragment_Stackable>(ItemFragmentTag::StackableFragmentTag,bHasFrag);
			
			if (bHasFrag && ItemFragment_Stackable)
			{
				ItemQuantity=ItemFragment_Stackable->CurrentStack;
				Num=ItemQuantity;
			}
	
			return GetItemByID(ItemID)?true:false;
		}

	
	}

	//item 不可堆叠，计数
	else
	{
		if (Items.IsEmpty())
		{
			ItemQuantity=0;
			return false;
		};
	
		for (FItemDef ItemDef : Items)
		{
			if (ItemDef.ID==ItemID)
			{
				Num++;
			}
		
		}
		
		ItemQuantity=Num;
		
		return GetItemByID(ItemID)?true:false;
		
	}
	
	return GetItemByID(ItemID)?true:false;
	

}

bool UCommonInvenstoryComponent::CanStackableItem(int32 ItemID) 
{
	
	FItemFragmentBase StackableFragment=GetItemFragmentDefByTagFromDT(ItemID, ItemFragmentTag::StackableFragmentTag);
	
	return StackableFragment.FragmentTag==ItemFragmentTag::StackableFragmentTag;
	
	
	
}

bool UCommonInvenstoryComponent::HasAlreadyMaxStack(int32 ItemID)
{
	
	if (CanStackableItem(ItemID))
	{
		bool   bHasFrag;
		if (FItemDef* Item=GetItemByID(ItemID))
		{
			FItemFragment_Stackable* ItemFragment_Stackable=Item->GetItemFragmentRefByTag<FItemFragment_Stackable>(ItemFragmentTag::StackableFragmentTag,bHasFrag);
			return ItemFragment_Stackable->CurrentStack>=ItemFragment_Stackable->MaxStackSize;
		}
		
	}
	 return false;
}

FItemFragmentBase UCommonInvenstoryComponent::GetItemFragmentDefByTagFromDT(int32 ItemID, const FGameplayTag& FragmentTag)
{
	
	FItemDef ItemDef=GetItemDefByIDFromDT(ItemID);
	bool bHasFrag;
	ItemDef.GetItemFragmentRefByTag<FItemFragmentBase>(FragmentTag,bHasFrag);

	if (bHasFrag)
	{
		return *ItemDef.GetItemFragmentRefByTag<FItemFragmentBase>(FragmentTag,bHasFrag);
	}

	return FItemFragmentBase();
	
};
	

	


FItemDef UCommonInvenstoryComponent::GetItemDefByIDFromDT(int32 ItemID) 
{
	FItemDef ItemDef;
	if (!ItemDataTable||ItemDataTable->GetRowNames().Num()<=0)  return ItemDef;
	
	ItemDataTable->ForeachRow<FItemDef>("",[&](const FName& Key, const FItemDef& Value)
	{
		if (Value.ID==ItemID)
		{
			ItemDef=Value;
		}
		
		
	});
	
	return  ItemDef;
}

bool  UCommonInvenstoryComponent::GetItemFragment_Display(int32 ItemID,FItemFragment_Display& ItemFragment_Display) 
{
	bool  bHasFrag;
	if (GetItemFragmentRefByTag<FItemFragment_Display>(ItemID,ItemFragmentTag::DisplayFragmentTag,bHasFrag))
	{
		ItemFragment_Display=*GetItemFragmentRefByTag<FItemFragment_Display>(ItemID,ItemFragmentTag::DisplayFragmentTag,bHasFrag);
	}
	
	return  GetItemFragmentRefByTag<FItemFragment_Display>(ItemID,ItemFragmentTag::DisplayFragmentTag,bHasFrag)?true:false;
	
}

bool UCommonInvenstoryComponent::GetItemFragment_Stackable(int32 ItemID, FItemFragment_Stackable& ItemFragment_Stackable)
{
	bool  bHasFrag;
	if (GetItemFragmentRefByTag<FItemFragment_Stackable>(ItemID,ItemFragmentTag::StackableFragmentTag,bHasFrag))
	{
		ItemFragment_Stackable=*GetItemFragmentRefByTag<FItemFragment_Stackable>(ItemID,ItemFragmentTag::StackableFragmentTag,bHasFrag);
	}
	
	return  GetItemFragmentRefByTag<FItemFragment_Stackable>(ItemID,ItemFragmentTag::StackableFragmentTag,bHasFrag)?true:false;
	
	
};

FItemDef*  UCommonInvenstoryComponent::GetItemByID(int32 ItemID)
{
	if (Items.IsEmpty()) return nullptr;

	for (auto& Item : Items)
	{
		if (Item.ID==ItemID)
		{
			return &Item;
		}
	}
	
	return nullptr;
	
}

int32 UCommonInvenstoryComponent::GetItemIndexByID(int32 ItemID)
{
	if (Items.IsEmpty()) return -1;
	
	for (int i = 0; i < Items.Num(); ++i)
	{
		if (!Items.IsValidIndex(i)) continue;
	
		if (Items[i].ID==ItemID)
		{
			return i;
		}
		
	}
	
	return -1;
}




