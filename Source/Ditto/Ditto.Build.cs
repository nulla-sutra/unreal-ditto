// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Ditto : ModuleRules
{
	public Ditto(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
			new string[]
			{
				// ... add public include paths required here ...
			}
		);


		PrivateIncludePaths.AddRange(
			new string[]
			{
				// ... add other private include paths required here ...
			}
		);


		PublicDependencyModuleNames.AddRange(
			new[]
			{
				"Core",
				"GameplayTags",

				"Combee",
				"CombeeUse",
				"CombeePresets",
				"CombeeTransaction",

				"UE5Coro",
				"MutableRuntime",
				"CustomizableObject"
				// ... add other public dependencies that you statically link with here ...
			}
		);

		if (Target.Type == TargetType.Editor)
		{
			PrivateDependencyModuleNames.AddRange(
				new[]
				{
					"CustomizableObjectEditor"
				}
			);
		}


		PrivateDependencyModuleNames.AddRange(
			new[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"NetCommon",
				"NetCore",

				// ... add private dependencies that you statically link with here ...	
			}
		);


		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
		);
	}
}