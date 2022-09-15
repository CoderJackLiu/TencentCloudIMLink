// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"
#include "V2TIMManager.h"
#include "V2TIMMessageManager.h"
#include "V2TIMFriendshipManager.h"
#include "V2TIMConversationManager.h"
#include "V2TIMGroupManager.h"
#include "V2TIMOfflinePushManager.h"
#include "V2TIMSignalingManager.h"

class FTencentCloudIMModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	/** Handle to the test dll we will load */
	void*	ExampleLibraryHandle;
};
