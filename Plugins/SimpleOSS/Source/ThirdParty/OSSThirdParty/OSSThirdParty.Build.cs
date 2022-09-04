// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class OSSThirdParty : ModuleRules
{
	//项目路径 插件在项目里面 可以获取的项目路径
    public string OSSProjectDirectory
    {
        get
        {
            return Path.GetFullPath(
                  Path.Combine(ModuleDirectory, "../../../../../")
            );
        }
    }

    private void CopyOSSDllAndLibToProjectBinaries(string Filepath, ReadOnlyTargetRules Target)
    {
        string BinariesDirectory = Path.Combine(OSSProjectDirectory, "Binaries", Target.Platform.ToString());

        string Filename = Path.GetFileName(Filepath);

        if (!Directory.Exists(BinariesDirectory))
        {
            Directory.CreateDirectory(BinariesDirectory);
        }

        if (!File.Exists(Path.Combine(BinariesDirectory, Filename)))
        {
            File.Copy(Filepath, Path.Combine(BinariesDirectory, Filename), true);
        }

        RuntimeDependencies.Add(Path.Combine(BinariesDirectory, Filename));
    }

    public OSSThirdParty(ReadOnlyTargetRules Target) : base(Target)
    {
        Type = ModuleType.External;

        string ThirdPartyPath = Path.GetFullPath(Path.Combine(ModuleDirectory, "../OSSThirdParty/"));
        string OssPathInclude = Path.Combine(ThirdPartyPath, "oss_c_sdk");
        string Third_partyPath = Path.Combine(ThirdPartyPath, "third_party/include");
        PublicIncludePaths.Add(OssPathInclude);
        PublicIncludePaths.Add(Third_partyPath);

        //Include of third-party libraries on which alicloud SDK depends 
        {
            PublicIncludePaths.Add(Path.Combine(Third_partyPath, "curl"));
            PublicIncludePaths.Add(Path.Combine(Third_partyPath, "mxml"));
            PublicIncludePaths.Add(Path.Combine(Third_partyPath, "aprutil"));
            PublicIncludePaths.Add(Path.Combine(Third_partyPath, "apr"));
        }

        string OSSPlatformFile = "";
        if (Target.Platform == UnrealTargetPlatform.Win64)
		{
            OSSPlatformFile = Path.Combine(ThirdPartyPath, "third_party/lib/x64");
            PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "x64/Release", "oss_c_sdk.lib"));
            CopyOSSDllAndLibToProjectBinaries(Path.Combine(ThirdPartyPath, "x64/Release", "oss_c_sdk.lib"), Target);

			//AddEngineThirdPartyPrivateStaticDependencies(Target, "libcurl");
		}
        //else if (Target.Platform == UnrealTargetPlatform.Win32)
        //{
        //    OSSPlatformFile = Path.Combine(ThirdPartyPath, "third_party/lib/Win32");
        //    PublicAdditionalLibraries.Add(Path.Combine(ThirdPartyPath, "Win32/Release", "oss_c_sdk.lib"));
        //    CopyOSSDllAndLibToProjectBinaries(Path.Combine(ThirdPartyPath, "Win32/Release", "oss_c_sdk.lib"), Target);
       // }
        else if (Target.Platform == UnrealTargetPlatform.Mac)
        {
            
        }

        PublicAdditionalLibraries.Add(Path.Combine(OSSPlatformFile, "libapr-1.lib"));
        PublicAdditionalLibraries.Add(Path.Combine(OSSPlatformFile, "libapriconv-1.lib"));
        PublicAdditionalLibraries.Add(Path.Combine(OSSPlatformFile, "libaprutil-1.lib"));
        PublicAdditionalLibraries.Add(Path.Combine(OSSPlatformFile, "libcurl_a.lib"));
        PublicAdditionalLibraries.Add(Path.Combine(OSSPlatformFile, "libexpat.lib"));
        PublicAdditionalLibraries.Add(Path.Combine(OSSPlatformFile, "mxml1.lib"));

        PublicDelayLoadDLLs.Add(Path.Combine(OSSPlatformFile, "libapr-1.dll"));
        PublicDelayLoadDLLs.Add(Path.Combine(OSSPlatformFile, "libapriconv-1.dll"));
        PublicDelayLoadDLLs.Add(Path.Combine(OSSPlatformFile, "libaprutil-1.dll"));
        PublicDelayLoadDLLs.Add(Path.Combine(OSSPlatformFile, "libcurl.dll"));
        PublicDelayLoadDLLs.Add(Path.Combine(OSSPlatformFile, "libexpat.dll"));
        PublicDelayLoadDLLs.Add(Path.Combine(OSSPlatformFile, "mxml1.dll"));
        PublicDelayLoadDLLs.Add(Path.Combine(OSSPlatformFile, "libeay32.dll"));
        PublicDelayLoadDLLs.Add(Path.Combine(OSSPlatformFile, "ssleay32.dll"));
        PublicDelayLoadDLLs.Add(Path.Combine(OSSPlatformFile, "zlibwapi.dll"));

        CopyOSSDllAndLibToProjectBinaries(Path.Combine(OSSPlatformFile, "libapr-1.dll"), Target);
        CopyOSSDllAndLibToProjectBinaries(Path.Combine(OSSPlatformFile, "libapriconv-1.dll"), Target);
        CopyOSSDllAndLibToProjectBinaries(Path.Combine(OSSPlatformFile, "libaprutil-1.dll"), Target);
        CopyOSSDllAndLibToProjectBinaries(Path.Combine(OSSPlatformFile, "libcurl.dll"), Target);
        CopyOSSDllAndLibToProjectBinaries(Path.Combine(OSSPlatformFile, "libexpat.dll"), Target);
        CopyOSSDllAndLibToProjectBinaries(Path.Combine(OSSPlatformFile, "mxml1.dll"), Target);
        CopyOSSDllAndLibToProjectBinaries(Path.Combine(OSSPlatformFile, "libeay32.dll"), Target);
        CopyOSSDllAndLibToProjectBinaries(Path.Combine(OSSPlatformFile, "ssleay32.dll"), Target);
        CopyOSSDllAndLibToProjectBinaries(Path.Combine(OSSPlatformFile, "zlibwapi.dll"), Target);

        CopyOSSDllAndLibToProjectBinaries(Path.Combine(OSSPlatformFile, "libapr-1.lib"), Target);
        CopyOSSDllAndLibToProjectBinaries(Path.Combine(OSSPlatformFile, "libapriconv-1.lib"), Target);
        CopyOSSDllAndLibToProjectBinaries(Path.Combine(OSSPlatformFile, "libaprutil-1.lib"), Target);
		CopyOSSDllAndLibToProjectBinaries(Path.Combine(OSSPlatformFile, "libcurl_a.lib"), Target);
        CopyOSSDllAndLibToProjectBinaries(Path.Combine(OSSPlatformFile, "libexpat.lib"), Target);
        CopyOSSDllAndLibToProjectBinaries(Path.Combine(OSSPlatformFile, "mxml1.lib"), Target);

      //  bUsePrecompiled = true;
    }
}
