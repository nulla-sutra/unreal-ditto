using UnrealBuildTool;

public class DittoSkill : ModuleRules
{
	public DittoSkill(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new[]
			{
				"Core",
				//
				"Combee",
				"CombeePresets",
				"CombeeTransaction",
				"Ditto",
				"Dogars"
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore"
			}
		);
	}
}