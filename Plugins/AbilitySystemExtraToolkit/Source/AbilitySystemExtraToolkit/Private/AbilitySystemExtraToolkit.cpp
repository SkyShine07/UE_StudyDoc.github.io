// Copyright Epic Games, Inc. All Rights Reserved.

#include "AbilitySystemExtraToolkit.h"

#define LOCTEXT_NAMESPACE "FAbilitySystemExtraToolkitModule"


namespace EventTagExtra
{
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_Hit,"GameplayEvent.Hit");
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_Shoot,"GameplayEvent.Shoot");
	
	
}


void FAbilitySystemExtraToolkitModule::StartupModule()
{
	
}

void FAbilitySystemExtraToolkitModule::ShutdownModule()
{
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAbilitySystemExtraToolkitModule, AbilitySystemExtraToolkit)