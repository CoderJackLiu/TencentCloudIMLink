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
#include "V2TIMMessageManager.h"
#include "V2TIMConversation.h"

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
		/*
		 * 5.1 获取用户资料
		 *
		 * 注意：
		 * 获取自己的资料，传入自己的 ID 即可。
		 * userIDList 建议一次最大 100 个，因为数量过多可能会导致数据包太大被后台拒绝，后台限制数据包最大为 1MB。
		 */
		void GetUsersInfo(const TArray<FString>& userIDList, V2TIMValueCallback<V2TIMUserFullInfoVector>* callback);

		/*
		 * 5.2 修改个人资料
		 *
		 */
		void SetSelfInfo(const V2TIMUserFullInfo& info, V2TIMCallback* callback);

		/*
		 * 5.3 查询用户状态，从 6.3 版本开始支持
		 * userIDList	需要获取的用户 ID
		 *
		 * 如果您想查询自己的自定义状态，您只需要传入自己的 userID 即可
		 * 当您批量查询时，接口只会返回查询成功的用户状态信息；当所有用户均查询失败时，接口会报错
		 */
		//todo 目前版本不支持6.3后支持；
		//void GetUserStatus(const TArray<FString>& userIDList, V2TIMValueCallback<V2TIMUserStatusVector>* callback);
		//SetSelfStatus()
		//SubscribeUserStatus()
		//UnsubscribeUserStatus()

		//return 高级群组管理类实例
		V2TIMMessageManager* GetMessageManager();

		//return 高级群组管理类实例
		V2TIMGroupManager* GetGroupManager();

		//return 会话管理类实例
		V2TIMConversationManager* GetConversationManager();

		//return 关系链管理类实例
		V2TIMFriendshipManager* GetFriendshipManager();

		//TODO sdk 版本不足
		//离线推送功能类实例
		// V2TIMOfflinePushManager* GetOfflinePushManager();

		//信令功能
		V2TIMSignalingManager* GetSignalingManager();

		//实验性 API 接口
		void CallExperimentalAPI(const FString& api, const void* param, V2TIMValueCallback<V2TIMBaseObject>* callback);

#pragma endregion IMUserInfo

#pragma region IMMessageManager

		/*
		 * 1.1 添加高级消息的事件监听器
		 */
		void AddAdvancedMsgListener(V2TIMAdvancedMsgListener* listener);

		/*
		 * 1.2 移除高级消息监听器
		 */
		void RemoveAdvancedMsgListener(V2TIMAdvancedMsgListener* listener);

		/*
		 * 2.1 创建文本消息
		 */
		V2TIMMessage CreateTextMessage(const FString& text);

		/*
		 * 2.2 创建文本消息，并且可以附带 @ 提醒功能
		 * 提醒消息仅适用于在群组中发送的消息
		 * 参数：
		 * atUserList	需要 @ 的用户列表，如果需要 @ALL，请传入 kImSDK_MesssageAtALL 常量字符串。 举个例子，假设该条文本消息希望@提醒 denny 和 lucy 两个用户，
		 * 同时又希望 @所有人，atUserList 传 {"denny","lucy",kImSDK_MesssageAtALL}
		 *
		 * atUserList 使用注意事项
		 * 默认情况下，最多支持 @ 30个用户，超过限制后，消息会发送失败。
		 * atUserList 的总数不能超过默认最大数，包括 @ALL。
		 * 直播群（AVChatRoom）不支持发送 @ 消息。
		 */
		V2TIMMessage CreateTextAtMessage(const FString& text, const TArray<FString>& atUserList);


		V2TIMMessage CreateCustomMessage(const V2TIMBuffer& data);

		V2TIMMessage CreateCustomMessage(const V2TIMBuffer& data, const FString& description, const FString& extension);

		V2TIMMessage CreateImageMessage(const FString& imagePath);

		V2TIMMessage CreateSoundMessage(const FString& soundPath, int32 duration);

		V2TIMMessage CreateVideoMessage(const FString& videoFilePath, const FString& type, int32 duration, const FString& snapshotPath);

		V2TIMMessage CreateFileMessage(const FString& filePath, const FString& fileName);

		V2TIMMessage CreateLocationMessage(const FString& desc, double longitude, double latitude);

		V2TIMMessage CreateFaceMessage(int32 index, const V2TIMBuffer& data);

		V2TIMMessage CreateMergerMessage(const V2TIMMessageVector& messageList, const FString& title, const TArray<FString>& abstractList,
		                                 const FString& compatibleText);

		V2TIMMessage CreateForwardMessage(const V2TIMMessage& message);

		//V2TIMMessage CreateTargetedGroupMessage(const V2TIMMessage& message, const TArray<FString>& receiverList);

		FString SendMessage(V2TIMMessage& message, const FString& receiver, const FString& groupID, V2TIMMessagePriority priority,
		                    bool onlineUserOnly, const V2TIMOfflinePushInfo& offlinePushInfo, V2TIMSendCallback* callback);

		void SetC2CReceiveMessageOpt(const TArray<FString>& userIDList, V2TIMReceiveMessageOpt opt, V2TIMCallback* callback);

		void GetC2CReceiveMessageOpt(const TArray<FString>& userIDList, V2TIMValueCallback<V2TIMReceiveMessageOptInfoVector>* callback);

		void SetGroupReceiveMessageOpt(const FString& groupID, V2TIMReceiveMessageOpt opt, V2TIMCallback* callback);

		void GetHistoryMessageList(const V2TIMMessageListGetOption& option, V2TIMValueCallback<V2TIMMessageVector>* callback);

		void RevokeMessage(const V2TIMMessage& message, V2TIMCallback* callback);

		// 		void ModifyMessage	(	const V2TIMMessage & 	message,
		// V2TIMCompleteCallback< V2TIMMessage > * 	callback 
		// )
		void MarkC2CMessageAsRead(const FString& userID, V2TIMCallback* callback);

		void MarkGroupMessageAsRead(const FString& groupID, V2TIMCallback* callback);

		void MarkAllMessageAsRead(V2TIMCallback* callback);

		void DeleteMessages(const V2TIMMessageVector& messages, V2TIMCallback* callback);

		void ClearC2CHistoryMessage(const FString& userID, V2TIMCallback* callback);

		void ClearGroupHistoryMessage(const FString& groupID, V2TIMCallback* callback);

		FString InsertGroupMessageToLocalStorage(V2TIMMessage& message, const FString& groupID, const FString& sender,
		                                         V2TIMValueCallback<V2TIMMessage>* callback);

		FString InsertC2CMessageToLocalStorage(V2TIMMessage& message, const FString& userID, const FString& sender,
		                                       V2TIMValueCallback<V2TIMMessage>* callback);

		void FindMessages(const TArray<FString>& messageIDList, V2TIMValueCallback<V2TIMMessageVector>* callback);

		void SearchLocalMessages(const V2TIMMessageSearchParam& searchParam, V2TIMValueCallback<V2TIMMessageSearchResult>* callback);

		//void SendMessageReadReceipts(const V2TIMMessageVector& messageList, V2TIMCallback* callback);

		//void GetMessageReadReceipts(const V2TIMMessageVector& messageList, V2TIMValueCallback<V2TIMMessageReceiptVector>* callback);

		// void GetGroupMessageReadMemberList(const V2TIMMessage& message, V2TIMGroupMessageReadMembersFilter filter, uint64_t nextSeq, int32 count,
		//                                    V2TIMValueCallback<V2TIMGroupMessageReadMemberList>* callback);

#pragma endregion IMMessageManager

#pragma region IMGroupManager

		void CreateGroup(const V2TIMGroupInfo& info, const V2TIMCreateGroupMemberInfoVector& memberList, V2TIMValueCallback<V2TIMString>* callback);

		void GetJoinedGroupList(V2TIMValueCallback<V2TIMGroupInfoVector>* callback);

		void GetGroupsInfo(const TArray<FString>& groupIDList, V2TIMValueCallback<V2TIMGroupInfoResultVector>* callback);

		void SearchGroups(const V2TIMGroupSearchParam& searchParam, V2TIMValueCallback<V2TIMGroupInfoVector>* callback);

		void SetGroupInfo(const V2TIMGroupInfo& info, V2TIMCallback* callback);

		void InitGroupAttributes(const FString& groupID, const V2TIMGroupAttributeMap& attributes, V2TIMCallback* callback);

		void SetGroupAttributes(const FString& groupID, const V2TIMGroupAttributeMap& attributes, V2TIMCallback* callback);

		void DeleteGroupAttributes(const FString& groupID, const TArray<FString>& keys, V2TIMCallback* callback);

		void GetGroupAttributes(const FString& groupID, const TArray<FString>& keys, V2TIMValueCallback<V2TIMGroupAttributeMap>* callback);

		void GetGroupOnlineMemberCount(const FString& groupID, V2TIMValueCallback<uint32_t>* callback);

		void GetGroupMemberList(const FString& groupID, int32 filter, uint64_t nextSeq, V2TIMValueCallback<V2TIMGroupMemberInfoResult>* callback);

		void GetGroupMembersInfo(const FString& groupID, TArray<FString> memberList, V2TIMValueCallback<V2TIMGroupMemberFullInfoVector>* callback);

		void SearchGroupMembers(const V2TIMGroupMemberSearchParam& param, V2TIMValueCallback<V2TIMGroupSearchGroupMembersMap>* callback);

		void SetGroupMemberInfo(const FString& groupID, const V2TIMGroupMemberFullInfo& info, V2TIMCallback* callback);

		void MuteGroupMember(const FString& groupID, const FString& userID, int32 seconds, V2TIMCallback* callback);

		void InviteUserToGroup(const FString& groupID, const TArray<FString>& userList, V2TIMValueCallback<V2TIMGroupMemberOperationResultVector>* callback);

		void KickGroupMember(const FString& groupID, const TArray<FString>& memberList, const FString& reason,
		                     V2TIMValueCallback<V2TIMGroupMemberOperationResultVector>* callback);

		void SetGroupMemberRole(const FString& groupID, const FString& userID, int32 role, V2TIMCallback* callback);

		//void MarkGroupMemberList(const FString& groupID, const TArray<FString>& memberList, int32 markType, bool enableMark, V2TIMCallback* callback);

		void TransferGroupOwner(const FString& groupID, const FString& userID, V2TIMCallback* callback);

		void GetGroupApplicationList(V2TIMValueCallback<V2TIMGroupApplicationResult>* callback);

		void AcceptGroupApplication(const V2TIMGroupApplication& application, const FString& reason, V2TIMCallback* callback);

		void RefuseGroupApplication(const V2TIMGroupApplication& application, const FString& reason, V2TIMCallback* callback);

		void SetGroupApplicationRead(V2TIMCallback* callback);

		//void GetJoinedCommunityList(V2TIMValueCallback<V2TIMGroupInfoVector>* callback);


		// void 	CreateTopicInCommunity (const V2TIMString &groupID, const V2TIMTopicInfo &topicInfo, V2TIMValueCallback< V2TIMString > *callback);

		// void 	DeleteTopicFromCommunity (const V2TIMString &groupID, const TArray<FString> &topicIDList, V2TIMValueCallback< V2TIMTopicOperationResultVector > *callback);

		//void 	SetTopicInfo (const V2TIMTopicInfo &topicInfo, V2TIMCallback *callback);

		// void 	GetTopicInfoList (const V2TIMString &groupID, const TArray<FString> &topicIDList, V2TIMValueCallback< V2TIMTopicInfoResultVector > *callback);

#pragma endregion IMGroupManager

#pragma region IMSignalingManager

		void AddSignalingListener(V2TIMSignalingListener* listener);

		void RemoveSignalingListener(V2TIMSignalingListener* listener);

		FString Invite(const FString& invitee, const FString& data, bool onlineUserOnly, const V2TIMOfflinePushInfo& offlinePushInfo, int timeout, V2TIMCallback* callback);

		FString InviteInGroup(const FString& groupID, const TArray<FString>& inviteeList, const FString& data, bool onlineUserOnly, int timeout, V2TIMCallback* callback);

		void Cancel(const FString& inviteID, const FString& data, V2TIMCallback* callback);

		void Accept(const FString& inviteID, const FString& data, V2TIMCallback* callback);

		void Reject(const FString& inviteID, const FString& data, V2TIMCallback* callback);

		V2TIMSignalingInfo GetSignalingInfo(const V2TIMMessage& msg);

		void AddInvitedSignaling(const V2TIMSignalingInfo& info, V2TIMCallback* callback);

#pragma endregion IMSignalingManager

#pragma region IMConversationManager

		void AddConversationListener(V2TIMConversationListener* listener);

		void RemoveConversationListener(V2TIMConversationListener* listener);

		void GetConversationList(uint64_t nextSeq, int32 count, V2TIMValueCallback<V2TIMConversationResult>* callback);

		void GetConversation(const FString& conversationID, V2TIMValueCallback<V2TIMConversation>* callback);

		//todo list
		void GetConversationList(const TArray<FString>& conversationIDList, V2TIMValueCallback<V2TIMVConversationVector>* callback);

		//void GetConversationListByFilter(const V2TIMConversationListFilter& filter, V2TIMValueCallback<V2TIMConversationResult>* callback);

		void DeleteConversation(const FString& conversationID, V2TIMCallback* callback);

		void SetConversationDraft(const FString& conversationID, const FString& draftText, V2TIMCallback* callback);

		//void SetConversationCustomData(const TArray<FString>& conversationIDList, const V2TIMBuffer& customData, V2TIMValueCallback<V2TIMConversationOperationResultVector>* callback);

		void PinConversation(const FString& conversationID, bool isPinned, V2TIMCallback* callback);

		//void MarkConversation(const TArray<FString>& conversationIDList, uint64_t markType, bool enableMark, V2TIMValueCallback<V2TIMConversationOperationResultVector>* callback);

		void GetTotalUnreadMessageCount(V2TIMValueCallback<uint64_t>* callback);

		//void CreateConversationGroup(const FString& groupName, const TArray<FString>& conversationIDList, V2TIMValueCallback<V2TIMConversationOperationResultVector>* callback);

		//void GetConversationGroupList(V2TIMValueCallback<TArray<FString>>* callback);

		//void DeleteConversationGroup(const FString& groupName, V2TIMCallback* callback);

		//void RenameConversationGroup(const FString& oldName, const FString& newName, V2TIMCallback* callback);

		//void AddConversationsToGroup(const FString& groupName, const TArray<FString>& conversationIDList, V2TIMValueCallback<V2TIMConversationOperationResultVector>* callback);

		//void DeleteConversationsFromGroup(const FString& groupName, const TArray<FString>& conversationIDList, V2TIMValueCallback<V2TIMConversationOperationResultVector>* callback);

#pragma endregion IMConversationManager

#pragma region IMFriendshipManager

		void AddFriendListener(V2TIMFriendshipListener* listener);

		void RemoveFriendListener(V2TIMFriendshipListener* listener);

		void GetFriendList(V2TIMValueCallback<V2TIMFriendInfoVector>* callback);

		void GetFriendsInfo(const TArray<FString>& userIDList, V2TIMValueCallback<V2TIMFriendInfoResultVector>* callback);

		void SetFriendInfo(const V2TIMFriendInfo& info, V2TIMCallback* callback);

		void SearchFriends(const V2TIMFriendSearchParam& searchParam, V2TIMValueCallback<V2TIMFriendInfoResultVector>* callback);

		void AddFriend(const V2TIMFriendAddApplication& application, V2TIMValueCallback<V2TIMFriendOperationResult>* callback);

		void DeleteFromFriendList(const TArray<FString>& userIDList, V2TIMFriendType deleteType, V2TIMValueCallback<V2TIMFriendOperationResultVector>* callback);

		void CheckFriend(const TArray<FString>& userIDList, V2TIMFriendType checkType, V2TIMValueCallback<V2TIMFriendCheckResultVector>* callback);

		void GetFriendApplicationList(V2TIMValueCallback<V2TIMFriendApplicationResult>* callback);

		void AcceptFriendApplication(const V2TIMFriendApplication& application, V2TIMFriendAcceptType acceptType, V2TIMValueCallback<V2TIMFriendOperationResult>* callback);

		void RefuseFriendApplication(const V2TIMFriendApplication& application, V2TIMValueCallback<V2TIMFriendOperationResult>* callback);

		void DeleteFriendApplication(const V2TIMFriendApplication& application, V2TIMCallback* callback);

		void SetFriendApplicationRead(V2TIMCallback* callback);

		void AddToBlackList(const TArray<FString>& userIDList, V2TIMValueCallback<V2TIMFriendOperationResultVector>* callback);

		void DeleteFromBlackList(const TArray<FString>& userIDList, V2TIMValueCallback<V2TIMFriendOperationResultVector>* callback);

		void GetBlackList(V2TIMValueCallback<V2TIMFriendInfoVector>* callback);

		void CreateFriendGroup(const FString& groupName, const TArray<FString>& userIDList, V2TIMValueCallback<V2TIMFriendOperationResultVector>* callback);

		void GetFriendGroups(const TArray<FString>& groupNameList, V2TIMValueCallback<V2TIMFriendGroupVector>* callback);

		void DeleteFriendGroup(const TArray<FString>& groupNameList, V2TIMCallback* callback);

		void RenameFriendGroup(const FString& oldName, const FString& newName, V2TIMCallback* callback);

		void AddFriendsToFriendGroup(const FString& groupName, const TArray<FString>& userIDList, V2TIMValueCallback<V2TIMFriendOperationResultVector>* callback);

		void DeleteFriendsFromFriendGroup(const FString& groupName, const TArray<FString>& userIDList, V2TIMValueCallback<V2TIMFriendOperationResultVector>* callback);

#pragma endregion IMFriendshipManager


	private:
		std::string IMUserId;
		std::string IMUserSigId;
		std::string IMSdkAppID;
		std::string IMSecretKey;
		std::int32_t IMExpireTime;
	private:
		V2TIMString ToIMString(const FString& InStr) const;
		FString ToFString(const V2TIMString& InStr) const;
		V2TIMStringVector ToIMStringArray(TArray<FString> InStrArray);

	};

	FORCEINLINE FTencentIM& GetIM() { return IM; }
private:
	static FTencentIMManage* TencentIM;

	FTencentIM IM;
};

#define Tencent_IM TencentIMManage::Get()->GetIM()
