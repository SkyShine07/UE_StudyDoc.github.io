// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Plugin_Study : ModuleRules
{
	public Plugin_Study(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"NavigationSystem",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"Niagara",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { "AbilitySystemExtraToolkit" });

		PublicIncludePaths.AddRange(new string[] {
			"Plugin_Study",
			"Plugin_Study/Variant_Strategy",
			"Plugin_Study/Variant_Strategy/UI",
			"Plugin_Study/Variant_TwinStick",
			"Plugin_Study/Variant_TwinStick/AI",
			"Plugin_Study/Variant_TwinStick/Gameplay",
			"Plugin_Study/Variant_TwinStick/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
