// Tonks © 2020 Felipe Anargyrou. All Rights Reserved

using UnrealBuildTool;
using System.Collections.Generic;

public class TonksTarget : TargetRules
{
	public TonksTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "Tonks" } );
	}
}
