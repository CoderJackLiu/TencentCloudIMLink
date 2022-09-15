// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class TencentCloudIMLibrary : ModuleRules
{
	public TencentCloudIMLibrary(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;
		
		// Load the header files of all platforms, because the header files are the same, so select the header files in the Android directory to load
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Android", "ImSDK_Android_CPP", "include"));

		if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            string DllPath = Path.Combine(ModuleDirectory, "Windows", "ImSDK_Windows_CPP", "lib", "Win64", "ImSDK.dll");
            string LibPath = Path.Combine(ModuleDirectory, "Windows", "ImSDK_Windows_CPP", "lib", "Win64", "ImSDK.lib");
            // Add the import library
            PublicAdditionalLibraries.Add(LibPath);

            // Ensure that the DLL is staged along with the executable
            RuntimeDependencies.Add(DllPath);
        }
        else if (Target.Platform == UnrealTargetPlatform.Mac)
        {
            PublicFrameworks.Add(Path.Combine(ModuleDirectory, "Mac", "ImSDKForMac_CPP.framework"));

            RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "Mac", "ImSDKForMac_CPP.framework/Versions/A/ImSDKForMac_CPP"));
        }
		else if (Target.Platform == UnrealTargetPlatform.IOS)
        {



            Framework IOSFramework = new Framework(
                "ImSDK_CPP",
                Path.Combine(ModuleDirectory, "IOS", "ImSDK_CPP.framework.zip"),
                "ImSDK_CPP"
            );

            PublicAdditionalFrameworks.Add(
                IOSFramework
            );


        }
        else if (Target.Platform == UnrealTargetPlatform.Android)
        {
            PrivateDependencyModuleNames.AddRange(new string[] { "Launch" });

            AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(ModuleDirectory, "Android", "ImSDK_Android_CPP", "APL_armv7.xml"));


            string[] archs = { "armeabi-v7a", "arm64-v8a", "x86", "x86_64" };
			
            for (int i = 0; i < archs.Length; i++)
            {
                string AndroidSOPath = Path.Combine(ModuleDirectory, "Android", "ImSDK_Android_CPP", "libs", archs[i], "libImSDK.so");
                PublicAdditionalLibraries.Add(AndroidSOPath);
            }
            

        }
	}
}
