// Copyright CombatAI Project. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class CombatAITarget : TargetRules
{
	public CombatAITarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
		ExtraModuleNames.AddRange(new string[] { "CombatAI" });
	}
}
