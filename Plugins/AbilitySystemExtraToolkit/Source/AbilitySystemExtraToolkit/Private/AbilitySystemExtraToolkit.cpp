// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystemExtraToolkit.h"

#define LOCTEXT_NAMESPACE "FAbilitySystemExtraToolkitModule"


namespace EventTagExtra
{
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_Hit,"GameplayEvent.Hit");
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_Shoot,"GameplayEvent.Shoot");
	
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_WeaponEquip,"GameplayEvent.WeaponEquip");
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_WeaponUnEquip,"GameplayEvent.WeaponUnEquip");
	
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_SpellSplashCast,"GameplayEvent.SpellSplashCast");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_SpellSplashCast,"GameplayCue.SpellSplashCast");

	UE_DEFINE_GAMEPLAY_TAG(Charactor_Status_WeaponEquiped,"Charactor.Status.WeaponEquiped");

	
	
}


void FAbilitySystemExtraToolkitModule::StartupModule()
{
	
}

void FAbilitySystemExtraToolkitModule::ShutdownModule()
{
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAbilitySystemExtraToolkitModule, AbilitySystemExtraToolkit)