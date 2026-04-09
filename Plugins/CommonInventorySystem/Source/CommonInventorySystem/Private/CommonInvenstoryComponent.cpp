// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonInvenstoryComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "NativeGameplayTags.h"
#include "ItemFragment.h"

//UE_DEFINE_GAMEPLAY_TAG_STATIC(RoleWepaonEquipedTag,"Role.State.WeaponEquipped")

UCommonInvenstoryComponent::UCommonInvenstoryComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

}

bool UCommonInvenstoryComponent::TryChangedItemNum(int32 ItemID, int32 Quantity)
{
	bool ReturnValue=false;
	
	if (Quantity>0)
	{
		ReturnValue=TryAddItem(ItemID, Quantity);
		
	}

	if (Quantity<0)
	{
		 ReturnValue=TryRemoveItem(ItemID, Quantity*-1);
	}


	if (ReturnValue)
	{
		OnItemNumChanged.Broadcast(ItemID,Quantity);
	}
	
	/*if (GEngine)
	{
		int32 ItemQuantity;
		HasItem(ItemID,ItemQuantity);
		
		GEngine->AddOnScreenDebugMessage(-1,3,FColor::Green,
		FString::Printf(TEXT("%d Total Has Changed : %d !!"),ItemID,ItemQuantity));
	}
	*/
			
	
	return ReturnValue;
	
}

void  UCommonInvenstoryComponent::TrySwapItemByIndex(int32 ItemID_1_index , int32 ItemID_2_index )
{
	if (!Items.IsValidIndex(ItemID_1_index)||!Items.IsValidIndex(ItemID_2_index)) return ;
	
	Items.Swap(ItemID_1_index,ItemID_2_index);
	OnItemSwap.Broadcast();
	
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1,3,FColor::Green,
		FString::Printf(TEXT("%d and %d Has Swap!!"),ItemID_1_index,ItemID_2_index));
	}
	
	//UE_LOG(LogTemp,Warning,TEXT("%d and %d Has Swap!!"),ItemID_1_index,ItemID_2_index);
	
}

/*
bool  UCommonInvenstoryComponent::TryUseItem(int32 ItemID)
{
	bool bSuccess=false;
	int32 ItemCurrentQuantity;
	FItemDef* Item=GetItemByID(ItemID);
	if (!HasItem(ItemID,ItemCurrentQuantity)||!Item) return false;
	
	
	// 应用所有的 效果片段
	for (auto& ItemFragment : GetItemByID(ItemID)->ItemFragments)
	{
		FItemFragmentBase* ItemFragmentBase=ItemFragment.GetMutablePtr<FItemFragmentBase>();
		if (ItemFragmentBase)
		{
			FItemFragment_Effect* Effect=static_cast<FItemFragment_Effect*>(ItemFragmentBase);
			ApplyGEsAndGrantAbilitiesToOwner(Effect->Effects,Effect->Abilities,ItemID);
		}
	}
	
	bSuccess=TryConsumeItem(ItemID,1)||bSuccess;



	return bSuccess;
	
}
*/

bool UCommonInvenstoryComponent::TryConsumeItem(int32 ItemID, int32 Quantity)
{
	int32 ItemQuantity;
	bool bHasTag;
	FItemFragment_Consumable* ItemFragment_Consumable=GetItemFragmentRefByTag<FItemFragment_Consumable>(ItemID,ItemFragmentTag::ConsumableFragmentTag,bHasTag);
	if (!HasItem(ItemID,ItemQuantity)||!bHasTag||!ItemFragment_Consumable) return false;

	// 应用GE 和 GA
	ApplyGEsAndGrantAbilitiesToOwner(ItemFragment_Consumable->Effects,ItemFragment_Consumable->Abilities,ItemID);
	
	if (!ItemFragment_Consumable->bConsumeOnUse) return false;
	return TryChangedItemNum(ItemID,Quantity*-1);
	
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
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1,3,FColor::Red,
		FString::Printf(TEXT("%d  HasRemove Call!!"),Quantity));
			
	}
	
	// 可堆叠的item 移除
	if (CanStackableItem(ItemID))
	{
		FItemDef* Item=GetItemByID(ItemID);
		if (!Item) return false;
		
		bool bHasFrag;
		FItemFragment_Stackable* ItemFragment_Stackable=GetItemFragmentRefByTag<FItemFragment_Stackable>(ItemID,
			ItemFragmentTag::StackableFragmentTag,bHasFrag);
		
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
		for (int i = 0; i < Quantity; ++i)
		{
			int32 Index=Items.IndexOfByPredicate([&](const FItemDef& Item)
			{
		return Item.ID==ItemID;
			});
		
			if (!Items.IsValidIndex(Index))  return false;;
			Items.RemoveAt(Index);
		}
	
	
		return true;
		
	};

	return false;
	
}

void UCommonInvenstoryComponent::SortItem()
{
	if (Items.IsEmpty()) return ;

	Items.Sort([&](const FItemDef& Item1,const FItemDef& Item2)
	{
		/*if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1,3,FColor::Green,
			FString::Printf(TEXT("start Sort!!")));
		}*/
		return Item1.ID<Item2.ID;
		});
	
	OnItemSwap.Broadcast();
	
}

TArray<FGameplayTag> UCommonInvenstoryComponent::GetOwnerAllItemTypes()
{
	TArray<FGameplayTag> ItemTypeTags;
	for (auto Item : Items)
	{
		ItemTypeTags.AddUnique(Item.ItemType);
	}
	return ItemTypeTags;
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
	if (!EquipmentSlot||ItemID<=0) return false;

	
	// 如何装备同样的装备，则卸载装备 ，返回Fasle;如不同，卸载旧装备，更新装备信息；
	
	if (EquipmentSlot->ItemID==ItemID)
	{
		TryUnEquipItem(EquipmentSlot->ItemID);
		return false;
	}
	
	TryUnEquipItem(EquipmentSlot->ItemID);
	
	EquipmentSlot->ItemID=ItemID;
	
	// 生成武器模型
	if (ItemFragment_Equipable->SK_EquipmentMesh.LoadSynchronous())
	{
		
		USkeletalMeshComponent* EquipmentSKC=NewObject<USkeletalMeshComponent>();
		EquipmentSKC->RegisterComponentWithWorld(GetWorld());
	
		EquipmentSKC->SetSkeletalMesh(ItemFragment_Equipable->SK_EquipmentMesh.LoadSynchronous());
		EquipmentSKC->SetLeaderPoseComponent(CharactorSKC);
		EquipmentSKC->AttachToComponent(CharactorSKC,FAttachmentTransformRules::SnapToTargetIncludingScale,ItemFragment_Equipable->AttachSocket);
		EquipmentSKC->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
		
		EquipmentSlot->EquipmentSKC=EquipmentSKC;
		
	}

	if (ItemFragment_Equipable->SM_EquipmentMesh.LoadSynchronous())
	{
		
		UStaticMeshComponent* Equipment_SM=NewObject<UStaticMeshComponent>();
		Equipment_SM->RegisterComponentWithWorld(GetWorld());
		Equipment_SM->SetStaticMesh(ItemFragment_Equipable->SM_EquipmentMesh.LoadSynchronous());
		
		Equipment_SM->AttachToComponent(CharactorSKC,FAttachmentTransformRules::SnapToTargetIncludingScale,ItemFragment_Equipable->AttachSocket);
		Equipment_SM->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
		
		EquipmentSlot->EquipmentSM=Equipment_SM;
		
	
	
	}
	
	// 替换动画示例
	if (!ItemFragment_Equipable->LinkABP.IsNull()&&ItemFragment_Equipable->LinkABP.LoadSynchronous())
	{
		CharactorSKC->LinkAnimClassLayers(ItemFragment_Equipable->LinkABP.LoadSynchronous());
	};
	
	// 应用 GE 和 授予GA
	ApplyGEsAndGrantAbilitiesToOwner(ItemFragment_Equipable->Effects,ItemFragment_Equipable->Abilities,ItemID);
	
	
	// 从背包中移除Item
	TryChangedItemNum(ItemID,-1);
	
	OnItemEquiped.Broadcast(*EquipmentSlot);
	
	return true;
}

bool UCommonInvenstoryComponent::TryUnEquipItem(int32 ItemID)
{
	bool bHasFrag;
	FItemFragment_Equipable* ItemFragment_Equipable= GetItemDefByIDFromDT(ItemID).GetItemFragmentRefByTag<FItemFragment_Equipable>(ItemFragmentTag::EquipmentableFragmentTag,bHasFrag);
	if (!ItemFragment_Equipable||!GetOwner()) return false;
	
	ACharacter* Character=Cast<ACharacter>(GetOwner());
	if (!Character) return false;
	USkeletalMeshComponent* CharactorSKC=Character->GetMesh();
	UAbilitySystemComponent* ASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Character);
	
	//更新装备槽信息
	FEquipmentSlot* EquipmentSlot=GetEquipmentSlot(ItemFragment_Equipable->EquipmentSlot);
	if (!EquipmentSlot) return false;
	
	EquipmentSlot->ItemID=-1;
	
	if (EquipmentSlot->EquipmentSKC)
	{
		EquipmentSlot->EquipmentSKC->DetachFromComponent( FDetachmentTransformRules::KeepWorldTransform);
		EquipmentSlot->EquipmentSKC->DestroyComponent();
	}
	if (EquipmentSlot->EquipmentSM)
	{
		EquipmentSlot->EquipmentSM->DetachFromComponent( FDetachmentTransformRules::KeepWorldTransform);
		EquipmentSlot->EquipmentSM->DestroyComponent();
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
	
	// 从背包中添加Item
	TryChangedItemNum(ItemID,1);
	
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

bool UCommonInvenstoryComponent::CanEquipItem(int32 ItemID)
{
	bool CanEquipItem=false;
	GetItemFragmentRefByTag<FItemFragment_Equipable>(ItemID,ItemFragmentTag::EquipmentableFragmentTag,CanEquipItem);
	return CanEquipItem;
}

bool UCommonInvenstoryComponent::HasEquippedItem(int32 ItemID)
{
	FEquipmentSlot* Slot=EquipmentSlots.FindByPredicate([&](FEquipmentSlot& Slot)
	{
		return Slot.ItemID=ItemID;
	});
	
	return Slot?true:false;
	
}

void UCommonInvenstoryComponent::ApplyGEsAndGrantAbilitiesToOwner(const TArray<TSoftClassPtr<UGameplayEffect>>& GEs,
                                                                  const TArray<TSoftClassPtr<UGameplayAbility>>& Abilities,int32 ItemID)
{
	if (!GetOwner()) return ;
	UAbilitySystemComponent* ASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	if (!ASC) return ;

	if (!GEs.IsEmpty())
	{
		for (auto GE : GEs)
		{
			FActiveGameplayEffectHandle handle=ASC->ApplyGameplayEffectToSelf(
				GE.LoadSynchronous()->GetDefaultObject<UGameplayEffect>(),1,ASC->MakeEffectContext());
			AppliedEffects.Add(handle,ItemID);
			
		}
	}
	
	if (!Abilities.IsEmpty())
	{
		for (auto& GA : Abilities)
		{
			if (GA.IsNull()) continue;
			FGameplayAbilitySpec AbilitySpec(GA.LoadSynchronous(),1);
			FGameplayAbilitySpecHandle AbilitySpecHandle=ASC->GiveAbility(AbilitySpec);
			
			GrantAbilities.Add(AbilitySpecHandle,ItemID);
			
		}
	}
	
	
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
	
	FItemFragmentBase* StackableFragment=GetItemFragmentDefByTagFromDT(ItemID, ItemFragmentTag::StackableFragmentTag);
	
	return StackableFragment?true:false;
	
	
	
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

void UCommonInvenstoryComponent::GetAllItemsByType( FGameplayTag ItemType, TArray<FItemDef>& OutItems)
{
	if (Items.IsEmpty()) return ;

	for (FItemDef& Item : Items)
	{
		if (Item.ItemType==ItemType)
		{
			OutItems.Add(Item);
		}
	}
	
}

FItemFragmentBase* UCommonInvenstoryComponent::GetItemFragmentDefByTagFromDT(int32 ItemID, const FGameplayTag& FragmentTag)
{
	
	FItemDef ItemDef=GetItemDefByIDFromDT(ItemID);
	bool bHasFrag;
	ItemDef.GetItemFragmentRefByTag<FItemFragmentBase>(FragmentTag,bHasFrag);

	if (bHasFrag)
	{
		return ItemDef.GetItemFragmentRefByTag<FItemFragmentBase>(FragmentTag,bHasFrag);
	}

	return nullptr;
	
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




