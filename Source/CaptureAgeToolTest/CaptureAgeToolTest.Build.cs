// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CaptureAgeToolTest : ModuleRules
{
	public CaptureAgeToolTest(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput" });

        PrivateDependencyModuleNames.AddRange(new string[] { "UMG", "UMGEditor", "Blutility", "Json", "JsonUtilities", "DesktopPlatform", "Slate" });

    }
}
