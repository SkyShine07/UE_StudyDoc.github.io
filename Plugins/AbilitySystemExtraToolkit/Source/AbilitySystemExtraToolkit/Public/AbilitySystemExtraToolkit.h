// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "NativeGameplayTags.h"
#include "Modules/ModuleManager.h"


namespace EventTagExtra
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Hit);
	
}


class FAbilitySystemExtraToolkitModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
