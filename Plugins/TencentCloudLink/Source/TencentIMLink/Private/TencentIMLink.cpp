// Copyright Epic Games, Inc. All Rights Reserved.

#include "TencentIMLink.h"
#include "Engine/Engine.h"
#include "Misc/Guid.h"
#include "Misc/Paths.h"
#include "HAL/Platform.h"
#include "GenericPlatform/GenericPlatformProcess.h"
#include <string>
#include <stdlib.h>
#include "DebugDefs.h"
#define LOCTEXT_NAMESPACE "FTencentIMLinkModule"

void FTencentIMLinkModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	V2TIMSDKConfig timConfig;
#if PLATFORM_ANDROID
	timConfig.initPath = static_cast<V2TIMString>("D:\\");
	timConfig.logPath = static_cast<V2TIMString>("D:\\");
#endif
	V2TIMManager::GetInstance()->InitSDK(SDKAppID, timConfig);

}

void FTencentIMLinkModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FTencentIMLinkModule, TencentIMLink)