// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "CoreMinimal.h"

#include "V2TIMBuffer.h"
#include "V2TIMConversationManager.h"
#include "V2TIMDefine.h"
#include "V2TIMManager.h"
#include "V2TIMMessage.h"
#include "V2TIMMessageManager.h"
#include "V2TIMGroupManager.h"
#include "V2TIMFriendshipManager.h"
#include "V2TIMSignaling.h"
#include "V2TIMSignalingManager.h"

#if PLATFORM_ANDROID
#include "Runtime/Launch/Public/Android/AndroidJNI.h"
#include "Runtime/Core/Public/Android/AndroidJavaEnv.h"
#include "Runtime/ApplicationCore/Public/Android/AndroidApplication.h"
#else
#include "GenerateTestUserSig.h"
#endif



class FTencentIMLinkModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
