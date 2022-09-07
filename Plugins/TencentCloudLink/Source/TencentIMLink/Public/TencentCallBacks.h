// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"


using namespace std;
#include "CoreMinimal.h"
#include "Misc/Paths.h"
#include <string>


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
// #else
// #include "GenerateTestUserSig.h"
#endif

#if PLATFORM_WINDOWS
#include "Windows/WindowsCriticalSection.h"
#else
#if PLATFORM_LINUX
#include "Linux/LinuxCriticalSection.h"
#else
#if PLATFORM_MAC
#include "Mac/MacCriticalSection.h"
#endif
#endif
#endif


#include "TencentCallBacks.generated.h"

/**
 * 
 */

// class SendCallback : public V2TIMSendCallback
// {
// public:
// 	SendCallback()
// 	{
// 	};
//
// 	~SendCallback()
// 	{
// 	};
//
// 	void OnProgress(uint32_t progress) override
// 	{
// 	}
//
// 	void OnSuccess(const V2TIMMessage& value) override
// 	{
// 		const std::string TempStr = value.nickName.CString();
// 		UE_LOG(LogTemp, Log, TEXT("<== logOut OnSuccess %s"),*TempStr.c_str());
// 	};
//
// 	void OnError(int error_code, const V2TIMString& error_message) override
// 	{
// 		UE_LOG(LogTemp, Log, TEXT("<== logOut failed OnError ======: %d"), error_code);
// 	};
// };

UCLASS()
class TENCENTIMLINK_API UTencentCallBacks : public UObject
{
	GENERATED_BODY()
};
