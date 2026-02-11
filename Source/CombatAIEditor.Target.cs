// Copyright CombatAI Project. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class CombatAIEditorTarget : TargetRules
{
	public CombatAIEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
		ExtraModuleNames.AddRange(new string[] { "CombatAI" });
	}
}
