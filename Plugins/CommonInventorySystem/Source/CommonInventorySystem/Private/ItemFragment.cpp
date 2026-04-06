#include "ItemFragment.h"


namespace ItemFragmentTag
{
	UE_DEFINE_GAMEPLAY_TAG(ConsumableFragmentTag,"ItemFragment.StaticData.Consumable")
	UE_DEFINE_GAMEPLAY_TAG(EquipmentableFragmentTag,"ItemFragment.StaticData.Equipmentable")


	UE_DEFINE_GAMEPLAY_TAG(DisplayFragmentTag,"ItemFragment.StaticData.Display")
	UE_DEFINE_GAMEPLAY_TAG(TradableFragmentTag,"ItemFragment.StaticData.Tradable")
	UE_DEFINE_GAMEPLAY_TAG(RestrictionsFragmentTag,"ItemFragment.StaticData.Restrictions")


	UE_DEFINE_GAMEPLAY_TAG(DurabilityFragmentTag,"ItemFragment.DynamicData.Durability")
	UE_DEFINE_GAMEPLAY_TAG(StackableFragmentTag,"ItemFragment.DynamicData.Stackable")
	UE_DEFINE_GAMEPLAY_TAG(QuestFragmentTag,"ItemFragment.DynamicData.Quest")
	
}
