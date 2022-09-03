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
		FString SendC2CTextMessage(FString text, FString userId) const;

		/*
		 * 3.4 发送单聊自定义（信令）消息（最大支持 8KB）
		 */
		FString SendC2CCustomMessage(const V2TIMBuffer& customData, const FString& userID) const;

		/*
		 * 3.5 发送群聊普通文本消息（最大支持 8KB）
		 */

		FString SendGroupTextMessage(const FString& text, const FString& groupID, V2TIMMessagePriority priority) const;

		/**
		 *	3.6 发送群聊自定义（信令）消息（最大支持 8KB）
		*/
		FString SendGroupCustomMessage(const V2TIMBuffer& customData, const FString& groupID, V2TIMMessagePriority priority) const;


		/*
		 * 4.1 设置群组监听器
		 */
		void AddGroupListener(V2TIMGroupListener* listener);

		/*
		 * 4.2 移除群组监听器
		 */
		void RemoveGroupListener(V2TIMGroupListener* listener);

		/*
		 * 4.3 创建群组
		* 参数
			* groupType	群类型，我们为您预定义好了几种常用的群类型，您也可以在控制台定义自己需要的群类型：
			* "Work" ：工作群，成员上限 200 人，不支持由用户主动加入，需要他人邀请入群，适合用于类似微信中随意组建的工作群（对应老版本的 Private 群）。
			* "Public" ：公开群，成员上限 2000 人，任何人都可以申请加群，但加群需群主或管理员审批，适合用于类似 QQ 中由群主管理的兴趣群。
			* "Meeting" ：会议群，成员上限 6000 人，任何人都可以自由进出，且加群无需被审批，适合用于视频会议和在线培训等场景（对应老版本的 ChatRoom 群）。
			* "Community" ：社群，成员上限 100000 人，任何人都可以自由进出，且加群无需被审批，适合用于知识分享和游戏交流等超大社区群聊场景。5.8 版本开始支持，需要您购买旗舰版套餐。
			* "AVChatRoom" ：直播群，人数无上限，任何人都可以自由进出，消息吞吐量大，适合用作直播场景中的高并发弹幕聊天室。
			* groupID	自定义群组 ID，可以传空字符串，此时系统会自动分配 groupID，并通过 callback 返回。 "Community" 类型自定义群组 ID 必须以 "@TGS#_" 作为前缀。
			* groupName	群名称，不能为空字符串。
		*注意：
			* 不支持在同一个 SDKAPPID 下创建两个相同 groupID 的群。
			* 社群（Community）功能仅 5.8.1668 增强版及以上版本支持，需购买旗舰版套餐包并申请开通后方可使用。
			* 直播群（AVChatRoom）：在进程重启或重新登录之后，如果想继续接收直播群的消息，请您调用 joinGroup 重新加入直播群。 
		 */
		void CreateGroup(const FString& groupType, const FString& groupID, const FString& groupName, V2TIMValueCallback<V2TIMString>* callback) const;


		/*
		 * 4.4 加入群组
		* 请注意如下特殊逻辑:
			工作群（Work）：不能主动入群，只能通过群成员调用 V2TIMGroupManager::InviteUserToGroup() 接口邀请入群。
			公开群（Public）：申请入群后，需要管理员审批，管理员在收到 V2TIMGroupListener::OnReceiveJoinApplication 回调后调用 V2TIMGroupManager::GetGroupApplicationList() 接口处理加群请求。
			其他群：可以直接入群。
			直播群（AVChatRoom）：在进程重启或重新登录之后，如果想继续接收直播群的消息，请您调用 joinGroup 重新加入直播群。
			直播群（AVChatRoom）：直播群新成员可以查看入群前消息，该功能为 IM 旗舰版功能，购买旗舰版套餐包后可使用，详见价格说明
		 */
		void JoinGroup(const FString& groupID, const FString& message, V2TIMCallback* callback) const;

		/*
		 * 4.5 退出群组
		 * 在公开群（Public）、会议（Meeting）和直播群（AVChatRoom）中，群主是不可以退群的，群主只能调用 DismissGroup 解散群组。
		 */
		void QuitGroup(const FString& groupID, V2TIMCallback* callback) const;

		/*
		* 4.6解散群组
		* 注意
		*	请注意如下特殊逻辑:
		*	Work：任何人都无法解散群组。
		*	其他群：群主可以解散群组。
		 */
		void DismissGroup(const FString& groupID, V2TIMCallback* callback) const;

#pragma endregion InitIM

#pragma region IMUserInfo
		void GetUsersInfo(const TArray<FString>& userIDList, V2TIMValueCallback<V2TIMUserFullInfoVector>* callback);

#pragma endregion IMUserInfo


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
