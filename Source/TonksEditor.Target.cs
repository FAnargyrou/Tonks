// Tonks © 2020 Felipe Anargyrou. All Rights Reserved

using UnrealBuildTool;
using System.Collections.Generic;

public class TonksEditorTarget : TargetRules
{
	public TonksEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "Tonks" } );
	}
}
