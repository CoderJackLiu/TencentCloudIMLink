#pragma once
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
#else
#include "GenerateTestUserSig.h"
#endif

#if PLATFORM_WINDOWS
#include "Windows/WindowsCriticalSection.h"
// #else
// #if PLATFORM_LINUX
// #include "Linux/LinuxCriticalSection.h"
// #else
// #if PLATFORM_MAC
// #include "Mac/MacCriticalSection.h"
// #endif
// #endif
#endif


class TENCENTIMLINK_API FTencentIMManage
{
public:
	FTencentIMManage();
	~FTencentIMManage();

	static FTencentIMManage* Get();
	static void Destroy();

	struct TENCENTIMLINK_API FTencentIM
	{
		friend class TencentIMManage;

		FString GetSDKVersion();

		int64 GetServerTime();

		static V2TIMManager* GetInstance();

#pragma region InitIM
		/**
		 *
		*/
		void InitAccounts(const FString& InUserId, const FString& InIMUserSigId);

		/*
		 * 
		 */
		void LogIn(const FString& InUserId, const FString& InIMUserSigId);

		/*
		 * 
		 */
		void LogOut();

		/*
		 * 
		 */
		FString GetLoginUser();

		/*
		 * 
		 */
		V2TIMLoginStatus GetLoginStatus();

		/*
		 * 3.1 设置基本消息（文本消息和自定义消息）的事件监听器
		 */
		void AddSimpleMsgListener(V2TIMSimpleMsgListener* listener);

		/*
		 * 
		 */
		void RemoveSimpleMsgListener(V2TIMSimpleMsgListener* listener);

		/*
		 * 
		 */
		FString SendC2CTextMessage(FString text, FString userId);

		/*
		 * 3.4 发送单聊自定义（信令）消息（最大支持 8KB）
		 */
		FString SendC2CCustomMessage(const V2TIMBuffer& customData, const FString& userID);

		/*
		 * 3.5 发送群聊普通文本消息（最大支持 8KB）
		 */

		FString SendGroupTextMessage(const FString& text, const FString& groupID, V2TIMMessagePriority priority);

		/**
		 *	3.6 发送群聊自定义（信令）消息（最大支持 8KB）
		*/
		FString SendGroupCustomMessage(const V2TIMBuffer& customData, const FString& groupID, V2TIMMessagePriority priority);

		/*
		 * 
		 */
		void AddGroupListener(V2TIMGroupListener* listener);


		


#pragma endregion InitIM


	private:
		std::string IMUserId;
		std::string IMUserSigId;
		std::string IMSdkAppID;
		std::string IMSecretKey;
		std::int32_t IMExpireTime;
	private:
		V2TIMString ToIMString(const FString& InStr) const;
		FString ToFString(const V2TIMString& InStr) const;
	};

	FORCEINLINE FTencentIM& GetIM() { return IM; }
private:
	static FTencentIMManage* TencentIM;

	FTencentIM IM;
};

#define Tencent_IM TencentIMManage::Get()->GetIM()
