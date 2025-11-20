// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "NativeGameplayTags.h"
#include "Modules/ModuleManager.h"


namespace EventTagExtra
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Hit);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Shoot);
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_WeaponEquip);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_WeaponUnEquip);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_SpellSplashCast);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_SpellSplashCast);

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Charactor_Status_WeaponEquiped);
	
	
}


class FAbilitySystemExtraToolkitModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
