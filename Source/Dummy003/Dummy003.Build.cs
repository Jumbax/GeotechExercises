// Copyright Geodesic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Dummy003 : ModuleRules
{
	public Dummy003(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core" });
	}
}
