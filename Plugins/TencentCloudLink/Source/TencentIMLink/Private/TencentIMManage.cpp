#include "TencentIMManage.h"

#include "Engine/Engine.h"
#include "Misc/Guid.h"
#include "Misc/Paths.h"
#include "HAL/Platform.h"
#include "GenericPlatform/GenericPlatformProcess.h"
#include <string>
#include <stdlib.h>
#include "DebugDefs.h"

#include <memory>
#include <fstream>
#include "Async/AsyncWork.h"
#include "Misc/ScopeLock.h"
#include "Async/TaskGraphInterfaces.h"
#include "CoreGlobals.h"
#include "Misc/ConfigCacheIni.h"

#include <vector>
#include <string>
#include <map>

#include "TencentCallBacks.h"
#include "TencentDataType.h"

// #if PLATFORM_WINDOWS
// #pragma optimize("",off) 
// #endif

FTencentIMManage* FTencentIMManage::TencentIM = nullptr;

FTencentIMManage::FTencentIMManage(): IM()
{
}

FTencentIMManage::~FTencentIMManage()
{
}

FTencentIMManage* FTencentIMManage::Get()
{
	if (!TencentIM)
	{
		TencentIM = new FTencentIMManage();
		V2TIMSDKConfig timConfig;
		V2TIMManager::GetInstance()->InitSDK(SDKAppID, timConfig);
		//if (V2TIMManager::GetInstance()->InitSDK(SDKAppID, timConfig))
		// {
		// 	exit(1);
		// }

		// Tencent_IM.InitLocalAccounts();
	}

	return TencentIM;
}

void FTencentIMManage::Destroy()
{
	if (TencentIM)
	{
		//Save cofig
		// TencentIM->GetIM().SaveAccounts();

		V2TIMManager::GetInstance()->UnInitSDK();
		delete TencentIM;
	}

	TencentIM = nullptr;
}

FString FTencentIMManage::FTencentIM::GetSDKVersion()
{
	return "";
}

int64 FTencentIMManage::FTencentIM::GetServerTime()
{
	return 0;
}

V2TIMManager* FTencentIMManage::FTencentIM::GetInstance()
{
	return V2TIMManager::GetInstance();
}

void FTencentIMManage::FTencentIM::InitAccounts(const FString& InUserId, const FString& InIMUserSigId)
{
}

void FTencentIMManage::FTencentIM::LogIn(const FString& InUserId, const FString& InIMUserSigId)
{
	class LoginCallback : public V2TIMCallback
	{
	public:
		LoginCallback()
		{
		};

		~LoginCallback()
		{
		};

		void OnSuccess() override
		{
			UE_LOG(LogTemp, Log, TEXT("<== login OnSuccess"));
		};

		void OnError(int error_code, const V2TIMString& error_message) override
		{
			UE_LOG(LogTemp, Log, TEXT("<== login failed OnError ======: %d"), error_code);
		};
	};
	LoginCallback* login_callback_ = new LoginCallback();
	const char* imTestUserId = TCHAR_TO_ANSI(*InUserId);
	#if PLATFORM_ANDROID
		if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
		{
			jmethodID GetPackageNameMethodID = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "genTestUserSig", "(ILjava/lang/String;Ljava/lang/String;)Ljava/lang/String;", false);
			jstring jsUserId = Env->NewStringUTF(imTestUserId);
			jstring jsKey = Env->NewStringUTF(SECRETKEY);
			jstring JstringResult = (jstring)FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis,GetPackageNameMethodID, SDKAppID, jsUserId, jsKey);
			FString FinalResult = FJavaHelper::FStringFromLocalRef(Env, JstringResult);
			auto twoHundredAnsi = StringCast<ANSICHAR>(*FinalResult);
			const char* userSig = twoHundredAnsi.Get();
			GetInstance()->Login(static_cast<V2TIMString>(imTestUserId), static_cast<V2TIMString>(userSig), login_callback_);
		}
	#else
	const char* userSig = GenerateTestUserSig().genTestUserSig(imTestUserId, SDKAppID, SECRETKEY);
	FString Test = userSig;
	UE_LOG(LogTemp, Warning, TEXT("UserID::  %s"), *Test);
	GetInstance()->Login(static_cast<V2TIMString>(imTestUserId), static_cast<V2TIMString>(userSig), login_callback_);
	#endif
}

// void FTencentIMManage::FTencentIM::LogOut()
// {
//
// 	// GetInstance()->Logout();
// }

FString FTencentIMManage::FTencentIM::GetLoginUser()
{
	const std::string TempUserId(GetInstance()->GetLoginUser().CString());
	FString OutUserName = TempUserId.c_str();
	return OutUserName;
}

ELoginStatus FTencentIMManage::FTencentIM::GetLoginStatus()
{
	//todo return type
	return GetLoginStatus(GetInstance()->GetLoginStatus());
}

void FTencentIMManage::FTencentIM::AddSimpleMsgListener(V2TIMSimpleMsgListener* listener)
{
	GetInstance()->AddSimpleMsgListener(listener);
}

void FTencentIMManage::FTencentIM::RemoveSimpleMsgListener(V2TIMSimpleMsgListener* listener)
{
	GetInstance()->RemoveSimpleMsgListener(listener);
}


// SendCallback SendCallbackIns;

// FString FTencentIMManage::FTencentIM::SendC2CTextMessage(FString text, FString userId) const
// {
// 	return ToFString(GetInstance()->SendC2CTextMessage(ToIMString(text), ToIMString(userId), &SendCallbackIns));
// }
//
// FString FTencentIMManage::FTencentIM::SendC2CCustomMessage(const V2TIMBuffer& customData, const FString& userID) const
// {
// 	//todo 返回代理
// 	return ToFString(GetInstance()->SendC2CCustomMessage(customData, ToIMString(userID), nullptr));
// }
//
// FString FTencentIMManage::FTencentIM::SendGroupTextMessage(const FString& text, const FString& groupID, EIMMessagePriority priority)
// {
// 	SendCallback callback;
// 	return ToFString(GetInstance()->SendGroupTextMessage(ToIMString(text), ToIMString(groupID), GetMessagePriority(priority), &callback));
// }
//
// FString FTencentIMManage::FTencentIM::SendGroupCustomMessage(const V2TIMBuffer& customData, const FString& groupID, EIMMessagePriority priority) 
// {
// 	SendCallback callback;
// 	return ToFString(GetInstance()->SendGroupCustomMessage(customData, ToIMString(groupID), GetMessagePriority(priority), &callback));
// }

void FTencentIMManage::FTencentIM::AddGroupListener(V2TIMGroupListener* listener)
{
	GetInstance()->AddGroupListener(listener);
}

void FTencentIMManage::FTencentIM::RemoveGroupListener(V2TIMGroupListener* listener)
{
	GetInstance()->RemoveGroupListener(listener);
}

void FTencentIMManage::FTencentIM::CreateGroup(const FString& groupType, const FString& groupID, const FString& groupName,
                                               V2TIMValueCallback<V2TIMString>* callback) const
{
	
	GetInstance()->CreateGroup(ToIMString(groupType), ToIMString(groupType), ToIMString(groupType), callback);
}

void FTencentIMManage::FTencentIM::JoinGroup(const FString& groupID, const FString& message, V2TIMCallback* callback) const
{
	//todo 代理
	GetInstance()->JoinGroup(ToIMString(groupID), ToIMString(message), callback);
}

void FTencentIMManage::FTencentIM::QuitGroup(const FString& groupID, V2TIMCallback* callback) const
{
	GetInstance()->QuitGroup(ToIMString(groupID), callback);
}

void FTencentIMManage::FTencentIM::DismissGroup(const FString& groupID, V2TIMCallback* callback) const
{
	GetInstance()->DismissGroup(ToIMString(groupID), callback);
}

void FTencentIMManage::FTencentIM::GetUsersInfo(const TArray<FString>& userIDList, V2TIMValueCallback<V2TIMUserFullInfoVector>* callback)
{
	// GetInstance()->GetUsersInfo();

	//todo 类型转换, 函数调用
	TArray<FString> UserIDList;
	// GetInstance()->GetUsersInfo(userIDList, , );
}

void FTencentIMManage::FTencentIM::SetSelfInfo(const V2TIMUserFullInfo& info, V2TIMCallback* callback)
{
	
	GetInstance()->SetSelfInfo(info, callback);
}

V2TIMMessageManager* FTencentIMManage::FTencentIM::GetMessageManager()
{
	return GetInstance()->GetMessageManager();
}

V2TIMGroupManager* FTencentIMManage::FTencentIM::GetGroupManager()
{
	return GetInstance()->GetGroupManager();
}

V2TIMConversationManager* FTencentIMManage::FTencentIM::GetConversationManager()
{
	return GetInstance()->GetConversationManager();
}

V2TIMFriendshipManager* FTencentIMManage::FTencentIM::GetFriendshipManager()
{
	return GetInstance()->GetFriendshipManager();
}

V2TIMSignalingManager* FTencentIMManage::FTencentIM::GetSignalingManager()
{
	return GetInstance()->GetSignalingManager();
}

void FTencentIMManage::FTencentIM::CallExperimentalAPI(const FString& api, const void* param, V2TIMValueCallback<V2TIMBaseObject>* callback)
{
	//todo callback
	GetInstance()->CallExperimentalAPI(ToIMString(api), param, callback);
}

void FTencentIMManage::FTencentIM::AddAdvancedMsgListener(V2TIMAdvancedMsgListener* listener)
{
	GetMessageManager()->AddAdvancedMsgListener(listener);
}


//todo !!!!!!
void FTencentIMManage::FTencentIM::RemoveAdvancedMsgListener(V2TIMAdvancedMsgListener* listener)
{
	GetMessageManager()->AddAdvancedMsgListener(listener);
}

V2TIMMessage FTencentIMManage::FTencentIM::CreateTextMessage(const FString& text)
{
	return GetMessageManager()->CreateTextMessage(ToIMString(text));
}

V2TIMMessage FTencentIMManage::FTencentIM::CreateTextAtMessage(const FString& text, const TArray<FString>& atUserList)
{
	return GetMessageManager()->CreateTextAtMessage(ToIMString(text), ToIMStringArray(atUserList));
}

V2TIMMessage FTencentIMManage::FTencentIM::CreateCustomMessage(const V2TIMBuffer& data)
{
	return GetMessageManager()->CreateCustomMessage(data);
}

V2TIMMessage FTencentIMManage::FTencentIM::CreateCustomMessage(const V2TIMBuffer& data, const FString& description, const FString& extension)
{
	return GetMessageManager()->CreateCustomMessage(data, ToIMString(description), ToIMString(extension));
}

V2TIMMessage FTencentIMManage::FTencentIM::CreateImageMessage(const FString& imagePath)
{
	return GetMessageManager()->CreateImageMessage(ToIMString(imagePath));
}

V2TIMMessage FTencentIMManage::FTencentIM::CreateSoundMessage(const FString& soundPath, int32 duration)
{
	return GetMessageManager()->CreateSoundMessage(ToIMString(soundPath), duration);
}

V2TIMMessage FTencentIMManage::FTencentIM::CreateVideoMessage(const FString& videoFilePath, const FString& type, int32 duration,
                                                              const FString& snapshotPath)
{
	return GetMessageManager()->CreateVideoMessage(ToIMString(videoFilePath), ToIMString(type), duration, ToIMString(snapshotPath));
}

V2TIMMessage FTencentIMManage::FTencentIM::CreateFileMessage(const FString& filePath, const FString& fileName)
{
	return GetMessageManager()->CreateFileMessage(ToIMString(filePath), ToIMString(fileName));
}

V2TIMMessage FTencentIMManage::FTencentIM::CreateLocationMessage(const FString& desc, double longitude, double latitude)
{
	return GetMessageManager()->CreateLocationMessage(ToIMString(desc), longitude, latitude);
}

V2TIMMessage FTencentIMManage::FTencentIM::CreateFaceMessage(int32 index, const V2TIMBuffer& data)
{
	return GetMessageManager()->CreateFaceMessage(index, data);
}

V2TIMMessage FTencentIMManage::FTencentIM::CreateMergerMessage(const V2TIMMessageVector& messageList, const FString& title, const TArray<FString>& abstractList,
                                                               const FString& compatibleText)
{
	return GetMessageManager()->CreateMergerMessage(messageList, ToIMString(title), ToIMStringArray(abstractList), ToIMString(compatibleText));
}

V2TIMMessage FTencentIMManage::FTencentIM::CreateForwardMessage(const V2TIMMessage& message)
{
	return GetMessageManager()->CreateForwardMessage(message);
}

// V2TIMMessage FTencentIMManage::FTencentIM::CreateTargetedGroupMessage(const V2TIMMessage& message, const TArray<FString>& receiverList)
// {
// 	return GetMessageManager()->CreateTargetedGroupMessage(text);
//
// }

FString FTencentIMManage::FTencentIM::SendMessage(V2TIMMessage& message, const FString& receiver, const FString& groupID, V2TIMMessagePriority priority,
                                                  bool onlineUserOnly, const V2TIMOfflinePushInfo& offlinePushInfo, V2TIMSendCallback* callback)
{
	return ToFString(GetMessageManager()->SendMessage(message, ToIMString(receiver), ToIMString(groupID), priority, onlineUserOnly, offlinePushInfo, callback));
}

void FTencentIMManage::FTencentIM::SetC2CReceiveMessageOpt(const TArray<FString>& userIDList, V2TIMReceiveMessageOpt opt, V2TIMCallback* callback)
{
	GetMessageManager()->SetC2CReceiveMessageOpt(ToIMStringArray(userIDList), opt, callback);
}

void FTencentIMManage::FTencentIM::GetC2CReceiveMessageOpt(const TArray<FString>& userIDList, V2TIMValueCallback<V2TIMReceiveMessageOptInfoVector>* callback)
{
	GetMessageManager()->GetC2CReceiveMessageOpt(ToIMStringArray(userIDList), callback);
}

void FTencentIMManage::FTencentIM::SetGroupReceiveMessageOpt(const FString& groupID, V2TIMReceiveMessageOpt opt, V2TIMCallback* callback)
{
	GetMessageManager()->SetGroupReceiveMessageOpt(ToIMString(groupID), opt, callback);
}

void FTencentIMManage::FTencentIM::GetHistoryMessageList(const V2TIMMessageListGetOption& option, V2TIMValueCallback<V2TIMMessageVector>* callback)
{
	GetMessageManager()->GetHistoryMessageList(option, callback);
}

void FTencentIMManage::FTencentIM::RevokeMessage(const V2TIMMessage& message, V2TIMCallback* callback)
{
	GetMessageManager()->RevokeMessage(message, callback);
}

void FTencentIMManage::FTencentIM::MarkC2CMessageAsRead(const FString& userID, V2TIMCallback* callback)
{
	GetMessageManager()->MarkC2CMessageAsRead(ToIMString(userID), callback);
}

void FTencentIMManage::FTencentIM::MarkGroupMessageAsRead(const FString& groupID, V2TIMCallback* callback)
{
	GetMessageManager()->MarkGroupMessageAsRead(ToIMString(groupID), callback);
}

void FTencentIMManage::FTencentIM::MarkAllMessageAsRead(V2TIMCallback* callback)
{
	GetMessageManager()->MarkAllMessageAsRead(callback);
}

void FTencentIMManage::FTencentIM::DeleteMessages(const V2TIMMessageVector& messages, V2TIMCallback* callback)
{
	GetMessageManager()->DeleteMessages(messages, callback);
}

void FTencentIMManage::FTencentIM::ClearC2CHistoryMessage(const FString& userID, V2TIMCallback* callback)
{
	GetMessageManager()->ClearC2CHistoryMessage(ToIMString(userID), callback);
}

void FTencentIMManage::FTencentIM::ClearGroupHistoryMessage(const FString& groupID, V2TIMCallback* callback)
{
	GetMessageManager()->ClearGroupHistoryMessage(ToIMString(groupID), callback);
}

FString FTencentIMManage::FTencentIM::InsertGroupMessageToLocalStorage(V2TIMMessage& message, const FString& groupID, const FString& sender,
                                                                       V2TIMValueCallback<V2TIMMessage>* callback)
{
	return ToFString(GetMessageManager()->InsertGroupMessageToLocalStorage(message, ToIMString(groupID), ToIMString(sender), callback));
}

FString FTencentIMManage::FTencentIM::InsertC2CMessageToLocalStorage(V2TIMMessage& message, const FString& userID, const FString& sender,
                                                                     V2TIMValueCallback<V2TIMMessage>* callback)
{
	return ToFString(GetMessageManager()->InsertC2CMessageToLocalStorage(message, ToIMString(userID), ToIMString(sender), callback));
}

void FTencentIMManage::FTencentIM::FindMessages(const TArray<FString>& messageIDList, V2TIMValueCallback<V2TIMMessageVector>* callback)
{
	GetMessageManager()->FindMessages(ToIMStringArray(messageIDList), callback);
}

void FTencentIMManage::FTencentIM::SearchLocalMessages(const V2TIMMessageSearchParam& searchParam, V2TIMValueCallback<V2TIMMessageSearchResult>* callback)
{
	GetMessageManager()->SearchLocalMessages(searchParam, callback);
}

//todo

/////////////////////////////////////////
///
///
/////////////////////////////////////////

void FTencentIMManage::FTencentIM::CreateGroup(const V2TIMGroupInfo& info, const V2TIMCreateGroupMemberInfoVector& memberList, V2TIMValueCallback<V2TIMString>* callback)
{
	//todo 类型转换
	GetGroupManager()->CreateGroup(info, memberList, callback);
}

void FTencentIMManage::FTencentIM::GetJoinedGroupList(V2TIMValueCallback<V2TIMGroupInfoVector>* callback)
{
	GetGroupManager()->GetJoinedGroupList(callback);
}

void FTencentIMManage::FTencentIM::GetGroupsInfo(const TArray<FString>& groupIDList, V2TIMValueCallback<V2TIMGroupInfoResultVector>* callback)
{
	GetGroupManager()->GetGroupsInfo(ToIMStringArray(groupIDList), callback);
}

void FTencentIMManage::FTencentIM::SearchGroups(const V2TIMGroupSearchParam& searchParam, V2TIMValueCallback<V2TIMGroupInfoVector>* callback)
{
	GetGroupManager()->SearchGroups(searchParam, callback);
}

void FTencentIMManage::FTencentIM::SetGroupInfo(const V2TIMGroupInfo& info, V2TIMCallback* callback)
{
	GetGroupManager()->SetGroupInfo(info, callback);
}

void FTencentIMManage::FTencentIM::InitGroupAttributes(const FString& groupID, const V2TIMGroupAttributeMap& attributes, V2TIMCallback* callback)
{
	GetGroupManager()->InitGroupAttributes(ToIMString(groupID), attributes, callback);
}

void FTencentIMManage::FTencentIM::SetGroupAttributes(const FString& groupID, const V2TIMGroupAttributeMap& attributes, V2TIMCallback* callback)
{
	GetGroupManager()->SetGroupAttributes(ToIMString(groupID), attributes, callback);
}

void FTencentIMManage::FTencentIM::DeleteGroupAttributes(const FString& groupID, const TArray<FString>& keys, V2TIMCallback* callback)
{
	GetGroupManager()->DeleteGroupAttributes(ToIMString(groupID), ToIMStringArray(keys), callback);
}

void FTencentIMManage::FTencentIM::GetGroupAttributes(const FString& groupID, const TArray<FString>& keys, V2TIMValueCallback<V2TIMGroupAttributeMap>* callback)
{
	GetGroupManager()->GetGroupAttributes(ToIMString(groupID), ToIMStringArray(keys), callback);
}

void FTencentIMManage::FTencentIM::GetGroupOnlineMemberCount(const FString& groupID, V2TIMValueCallback<uint32_t>* callback)
{
	GetGroupManager()->GetGroupOnlineMemberCount(ToIMString(groupID), callback);
}

void FTencentIMManage::FTencentIM::GetGroupMemberList(const FString& groupID, int32 filter, uint64_t nextSeq, V2TIMValueCallback<V2TIMGroupMemberInfoResult>* callback)
{
	GetGroupManager()->GetGroupMemberList(ToIMString(groupID), filter, nextSeq, callback);
}

void FTencentIMManage::FTencentIM::GetGroupMembersInfo(const FString& groupID, TArray<FString> memberList, V2TIMValueCallback<V2TIMGroupMemberFullInfoVector>* callback)
{
	GetGroupManager()->GetGroupMembersInfo(ToIMString(groupID), ToIMStringArray(memberList), callback);
}

void FTencentIMManage::FTencentIM::SearchGroupMembers(const V2TIMGroupMemberSearchParam& param, V2TIMValueCallback<V2TIMGroupSearchGroupMembersMap>* callback)
{
	GetGroupManager()->SearchGroupMembers(param, callback);
}

void FTencentIMManage::FTencentIM::SetGroupMemberInfo(const FString& groupID, const V2TIMGroupMemberFullInfo& info, V2TIMCallback* callback)
{
	GetGroupManager()->SetGroupMemberInfo(ToIMString(groupID), info, callback);
}

void FTencentIMManage::FTencentIM::MuteGroupMember(const FString& groupID, const FString& userID, int32 seconds, V2TIMCallback* callback)
{
	GetGroupManager()->MuteGroupMember(ToIMString(groupID), ToIMString(userID), seconds, callback);
}

void FTencentIMManage::FTencentIM::InviteUserToGroup(const FString& groupID, const TArray<FString>& userList,
                                                     V2TIMValueCallback<V2TIMGroupMemberOperationResultVector>* callback)
{
	GetGroupManager()->InviteUserToGroup(ToIMString(groupID), ToIMStringArray(userList), callback);
}

void FTencentIMManage::FTencentIM::KickGroupMember(const FString& groupID, const TArray<FString>& memberList, const FString& reason,
                                                   V2TIMValueCallback<V2TIMGroupMemberOperationResultVector>* callback)
{
	GetGroupManager()->KickGroupMember(ToIMString(groupID), ToIMStringArray(memberList), ToIMString(reason), callback);
}

void FTencentIMManage::FTencentIM::SetGroupMemberRole(const FString& groupID, const FString& userID, int32 role, V2TIMCallback* callback)
{
	GetGroupManager()->SetGroupMemberRole(ToIMString(groupID), ToIMString(userID), role, callback);
}

// void FTencentIMManage::FTencentIM::MarkGroupMemberList(const FString& groupID, const TArray<FString>& memberList, int32 markType, bool enableMark,
// 	V2TIMCallback* callback)
// {
// 	GetGroupManager()->MarkGroupMemberList(info, memberList,callback);
//
// }

void FTencentIMManage::FTencentIM::TransferGroupOwner(const FString& groupID, const FString& userID, V2TIMCallback* callback)
{
	GetGroupManager()->TransferGroupOwner(ToIMString(groupID), ToIMString(userID), callback);
}

void FTencentIMManage::FTencentIM::GetGroupApplicationList(V2TIMValueCallback<V2TIMGroupApplicationResult>* callback)
{
	GetGroupManager()->GetGroupApplicationList(callback);
}

void FTencentIMManage::FTencentIM::AcceptGroupApplication(const V2TIMGroupApplication& application, const FString& reason, V2TIMCallback* callback)
{
	GetGroupManager()->AcceptGroupApplication(application, ToIMString(reason), callback);
}

void FTencentIMManage::FTencentIM::RefuseGroupApplication(const V2TIMGroupApplication& application, const FString& reason, V2TIMCallback* callback)
{
	GetGroupManager()->RefuseGroupApplication(application, ToIMString(reason), callback);
}

void FTencentIMManage::FTencentIM::SetGroupApplicationRead(V2TIMCallback* callback)
{
	GetGroupManager()->SetGroupApplicationRead(callback);
}

//////////////////////////////////////////////////////////
///
///IMSignalingManager
//////////////////////////////////////////////////////////
void FTencentIMManage::FTencentIM::AddSignalingListener(V2TIMSignalingListener* listener)
{
	GetSignalingManager()->AddSignalingListener(listener);
}

void FTencentIMManage::FTencentIM::RemoveSignalingListener(V2TIMSignalingListener* listener)
{
	GetSignalingManager()->RemoveSignalingListener(listener);
}

FString FTencentIMManage::FTencentIM::Invite(const FString& invitee, const FString& data, bool onlineUserOnly, const V2TIMOfflinePushInfo& offlinePushInfo, int timeout,
                                             V2TIMCallback* callback)
{
	return ToFString(GetSignalingManager()->Invite(ToIMString(invitee), ToIMString(data), onlineUserOnly, offlinePushInfo, timeout, callback));
}

FString FTencentIMManage::FTencentIM::InviteInGroup(const FString& groupID, const TArray<FString>& inviteeList, const FString& data, bool onlineUserOnly, int timeout,
                                                    V2TIMCallback* callback)
{
	return ToFString(GetSignalingManager()->InviteInGroup(ToIMString(groupID), ToIMStringArray(inviteeList), ToIMString(data), onlineUserOnly, timeout, callback));
}

void FTencentIMManage::FTencentIM::Cancel(const FString& inviteID, const FString& data, V2TIMCallback* callback)
{
	GetSignalingManager()->Cancel(ToIMString(inviteID), ToIMString(data), callback);
}

void FTencentIMManage::FTencentIM::Accept(const FString& inviteID, const FString& data, V2TIMCallback* callback)
{
	GetSignalingManager()->Accept(ToIMString(inviteID), ToIMString(data), callback);
}

void FTencentIMManage::FTencentIM::Reject(const FString& inviteID, const FString& data, V2TIMCallback* callback)
{
	GetSignalingManager()->Reject(ToIMString(inviteID), ToIMString(data), callback);
}

V2TIMSignalingInfo FTencentIMManage::FTencentIM::GetSignalingInfo(const V2TIMMessage& msg)
{
	return GetSignalingManager()->GetSignalingInfo(msg);
}

void FTencentIMManage::FTencentIM::AddInvitedSignaling(const V2TIMSignalingInfo& info, V2TIMCallback* callback)
{
	GetSignalingManager()->AddInvitedSignaling(info, callback);
}

void FTencentIMManage::FTencentIM::AddConversationListener(V2TIMConversationListener* listener)
{
	GetConversationManager()->AddConversationListener(listener);
}

void FTencentIMManage::FTencentIM::RemoveConversationListener(V2TIMConversationListener* listener)
{
	GetConversationManager()->AddConversationListener(listener);
}

void FTencentIMManage::FTencentIM::GetConversationList(uint64_t nextSeq, int32 count, V2TIMValueCallback<V2TIMConversationResult>* callback)
{
	GetConversationManager()->GetConversationList(nextSeq, count, callback);
}

void FTencentIMManage::FTencentIM::GetConversation(const FString& conversationID, V2TIMValueCallback<V2TIMConversation>* callback)
{
	GetConversationManager()->GetConversation(ToIMString(conversationID), callback);
}

void FTencentIMManage::FTencentIM::GetConversationList(const TArray<FString>& conversationIDList, V2TIMValueCallback<V2TIMVConversationVector>* callback)
{
	GetConversationManager()->GetConversationList(ToIMStringArray(conversationIDList), callback);
}

void FTencentIMManage::FTencentIM::DeleteConversation(const FString& conversationID, V2TIMCallback* callback)
{
	GetConversationManager()->DeleteConversation(ToIMString(conversationID), callback);
}

void FTencentIMManage::FTencentIM::SetConversationDraft(const FString& conversationID, const FString& draftText, V2TIMCallback* callback)
{
	GetConversationManager()->SetConversationDraft(ToIMString(conversationID), ToIMString(draftText), callback);
}

void FTencentIMManage::FTencentIM::PinConversation(const FString& conversationID, bool isPinned, V2TIMCallback* callback)
{
	GetConversationManager()->PinConversation(ToIMString(conversationID), isPinned, callback);
}

void FTencentIMManage::FTencentIM::GetTotalUnreadMessageCount(V2TIMValueCallback<uint64_t>* callback)
{
	GetConversationManager()->GetTotalUnreadMessageCount(callback);
}

////////////////////////////////////////////////////
///		IMFriendshipManager
///
//////////////////////////
void FTencentIMManage::FTencentIM::AddFriendListener(V2TIMFriendshipListener* listener)
{
	GetFriendshipManager()->AddFriendListener(listener);
}

void FTencentIMManage::FTencentIM::RemoveFriendListener(V2TIMFriendshipListener* listener)
{
	GetFriendshipManager()->AddFriendListener(listener);
}

void FTencentIMManage::FTencentIM::GetFriendList(V2TIMValueCallback<V2TIMFriendInfoVector>* callback)
{
	GetFriendshipManager()->GetFriendList(callback);
}

void FTencentIMManage::FTencentIM::GetFriendsInfo(const TArray<FString>& userIDList, V2TIMValueCallback<V2TIMFriendInfoResultVector>* callback)
{
	GetFriendshipManager()->GetFriendsInfo(ToIMStringArray(userIDList), callback);
}

void FTencentIMManage::FTencentIM::SetFriendInfo(const V2TIMFriendInfo& info, V2TIMCallback* callback)
{
	GetFriendshipManager()->SetFriendInfo(info, callback);
}

void FTencentIMManage::FTencentIM::SearchFriends(const V2TIMFriendSearchParam& searchParam, V2TIMValueCallback<V2TIMFriendInfoResultVector>* callback)
{
	GetFriendshipManager()->SearchFriends(searchParam, callback);
}

void FTencentIMManage::FTencentIM::AddFriend(const V2TIMFriendAddApplication& application, V2TIMValueCallback<V2TIMFriendOperationResult>* callback)
{
	GetFriendshipManager()->AddFriend(application, callback);
}

void FTencentIMManage::FTencentIM::DeleteFromFriendList(const TArray<FString>& userIDList, V2TIMFriendType deleteType, V2TIMValueCallback<V2TIMFriendOperationResultVector>* callback)
{
	GetFriendshipManager()->DeleteFromFriendList(ToIMStringArray(userIDList), deleteType, callback);
}

void FTencentIMManage::FTencentIM::CheckFriend(const TArray<FString>& userIDList, V2TIMFriendType checkType, V2TIMValueCallback<V2TIMFriendCheckResultVector>* callback)
{
	GetFriendshipManager()->CheckFriend(ToIMStringArray(userIDList), checkType, callback);
}

void FTencentIMManage::FTencentIM::GetFriendApplicationList(V2TIMValueCallback<V2TIMFriendApplicationResult>* callback)
{
	GetFriendshipManager()->GetFriendApplicationList(callback);
}

void FTencentIMManage::FTencentIM::AcceptFriendApplication(const V2TIMFriendApplication& application, V2TIMFriendAcceptType acceptType, V2TIMValueCallback<V2TIMFriendOperationResult>* callback)
{
	GetFriendshipManager()->AcceptFriendApplication(application, acceptType, callback);
}

void FTencentIMManage::FTencentIM::RefuseFriendApplication(const V2TIMFriendApplication& application, V2TIMValueCallback<V2TIMFriendOperationResult>* callback)
{
	GetFriendshipManager()->RefuseFriendApplication(application, callback);
}

void FTencentIMManage::FTencentIM::DeleteFriendApplication(const V2TIMFriendApplication& application, V2TIMCallback* callback)
{
	GetFriendshipManager()->DeleteFriendApplication(application, callback);
}

void FTencentIMManage::FTencentIM::SetFriendApplicationRead(V2TIMCallback* callback)
{
	GetFriendshipManager()->SetFriendApplicationRead(callback);
}

void FTencentIMManage::FTencentIM::AddToBlackList(const TArray<FString>& userIDList, V2TIMValueCallback<V2TIMFriendOperationResultVector>* callback)
{
	GetFriendshipManager()->AddToBlackList(ToIMStringArray(userIDList), callback);
}

void FTencentIMManage::FTencentIM::DeleteFromBlackList(const TArray<FString>& userIDList, V2TIMValueCallback<V2TIMFriendOperationResultVector>* callback)
{
	GetFriendshipManager()->DeleteFromBlackList(ToIMStringArray(userIDList), callback);
}

void FTencentIMManage::FTencentIM::GetBlackList(V2TIMValueCallback<V2TIMFriendInfoVector>* callback)
{
	GetFriendshipManager()->GetBlackList(callback);
}

void FTencentIMManage::FTencentIM::CreateFriendGroup(const FString& groupName, const TArray<FString>& userIDList, V2TIMValueCallback<V2TIMFriendOperationResultVector>* callback)
{
	GetFriendshipManager()->CreateFriendGroup(ToIMString(groupName), ToIMStringArray(userIDList), callback);
}

void FTencentIMManage::FTencentIM::GetFriendGroups(const TArray<FString>& groupNameList, V2TIMValueCallback<V2TIMFriendGroupVector>* callback)
{
	GetFriendshipManager()->GetFriendGroups(ToIMStringArray(groupNameList), callback);
}

void FTencentIMManage::FTencentIM::DeleteFriendGroup(const TArray<FString>& groupNameList, V2TIMCallback* callback)
{
	GetFriendshipManager()->DeleteFriendGroup(ToIMStringArray(groupNameList), callback);
}

void FTencentIMManage::FTencentIM::RenameFriendGroup(const FString& oldName, const FString& newName, V2TIMCallback* callback)
{
	GetFriendshipManager()->RenameFriendGroup(ToIMString(oldName), ToIMString(newName), callback);
}

void FTencentIMManage::FTencentIM::AddFriendsToFriendGroup(const FString& groupName, const TArray<FString>& userIDList, V2TIMValueCallback<V2TIMFriendOperationResultVector>* callback)
{
	GetFriendshipManager()->AddFriendsToFriendGroup(ToIMString(groupName), ToIMStringArray(userIDList), callback);
}

void FTencentIMManage::FTencentIM::DeleteFriendsFromFriendGroup(const FString& groupName, const TArray<FString>& userIDList, V2TIMValueCallback<V2TIMFriendOperationResultVector>* callback)
{
	GetFriendshipManager()->DeleteFriendsFromFriendGroup(ToIMString(groupName), ToIMStringArray(userIDList), callback);
}


V2TIMString FTencentIMManage::FTencentIM::ToIMString(const FString& InStr) const
{
	const char* OutIMString = TCHAR_TO_ANSI(*InStr);
	return static_cast<V2TIMString>(OutIMString);
}

FString FTencentIMManage::FTencentIM::ToFString(const V2TIMString& InStr) const
{
	const std::string tempStr = InStr.CString();
	return tempStr.c_str();
}

V2TIMStringVector FTencentIMManage::FTencentIM::ToIMStringArray(TArray<FString> InStrArray)
{
	V2TIMStringVector StrVector;
	for (FString Str : InStrArray)
	{
		StrVector.PushBack(ToIMString(Str));
	}
	return StrVector;
}

ELoginStatus FTencentIMManage::FTencentIM::GetLoginStatus(const V2TIMLoginStatus& Status)
{
	// FString AclString = UTF8_TO_TCHAR(ACLChar);
	if (Status == V2TIM_STATUS_LOGINED)
	{
		return ELoginStatus::V2TIM_STATUS_LOGINED;
	}
	else if (Status == V2TIM_STATUS_LOGINING)
	{
		return ELoginStatus::V2TIM_STATUS_LOGINING;
	}
	else if (Status == V2TIM_STATUS_LOGOUT)
	{
		return ELoginStatus::V2TIM_STATUS_LOGOUT;
	}

	return ELoginStatus::V2TIM_STATUS_LOGOUT;
}

V2TIMMessagePriority FTencentIMManage::FTencentIM::GetMessagePriority(EIMMessagePriority InPriority)
{
	V2TIMMessagePriority Priority = V2TIM_PRIORITY_DEFAULT;
	switch (InPriority)
	{
	case EIMMessagePriority::V2TIM_PRIORITY_DEFAULT:
		Priority = V2TIMMessagePriority::V2TIM_PRIORITY_DEFAULT;
		break;
	case EIMMessagePriority::V2TIM_PRIORITY_HIGH:
		Priority = V2TIMMessagePriority::V2TIM_PRIORITY_HIGH;
		break;
	case EIMMessagePriority::V2TIM_PRIORITY_NORMAL:
		Priority = V2TIMMessagePriority::V2TIM_PRIORITY_NORMAL;
		break;
	case EIMMessagePriority::V2TIM_PRIORITY_LOW:
		Priority = V2TIMMessagePriority::V2TIM_PRIORITY_LOW;
		break;
	}
	return Priority;
}

