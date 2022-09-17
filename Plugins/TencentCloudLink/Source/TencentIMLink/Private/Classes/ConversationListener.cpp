// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/ConversationListener.h"

void UConversationListener::NativeConstruct()
{
	Super::NativeConstruct();
	V2TIMManager::GetInstance()->AddGroupListener(this);
	V2TIMManager::GetInstance()->GetMessageManager()->AddAdvancedMsgListener(this);
	V2TIMManager::GetInstance()->AddSimpleMsgListener(this);
	// V2TIMManager::GetInstance()->GetConversationManager()->AddConversationListener(this);

}

void UConversationListener::NativeDestruct()
{
	V2TIMManager::GetInstance()->RemoveGroupListener(this);
	V2TIMManager::GetInstance()->GetMessageManager()->RemoveAdvancedMsgListener(this);
	Super::NativeDestruct();
}

void UConversationListener::OnRecvGroupCustomMessage_Implementation(const FString& msgID, const FString& groupID, const FTIMGroupMemberFullInfo& sender, const FBuffer& customData)
{
}

void UConversationListener::OnRecvGroupTextMessage_Implementation(const FString& msgID, const FString& groupID, const FTIMGroupMemberFullInfo& sender, const FString& text)
{
}

void UConversationListener::OnRecvC2CCustomMessage_Implementation(const FString& msgID, const FTIMUserFullInfo& sender, const FBuffer& customData)
{
}

void UConversationListener::OnRecvC2CTextMessage_Implementation(const FString& msgID, const FTIMUserFullInfo& sender, const FString& text)
{
}

void UConversationListener::OnGroupAttributeChanged_Implementation(const FString& groupID, const TMap<FString, FString>& groupAttributeMap)
{
}

void UConversationListener::OnReceiveRESTCustomData_Implementation(const FString& groupID, const FBuffer& customData)
{
}

void UConversationListener::OnQuitFromGroup_Implementation(const FString& groupID)
{
}

void UConversationListener::OnRevokeAdministrator_Implementation(const FString& groupID, const FTIMGroupMemberInfo& opUser, const TArray<FTIMGroupMemberInfo>& memberList)
{
}

void UConversationListener::OnGrantAdministrator_Implementation(const FString& groupID, const FTIMGroupMemberInfo& opUser, const TArray<FTIMGroupMemberInfo>& memberList)
{
}

void UConversationListener::OnApplicationProcessed_Implementation(const FString& groupID, const FTIMGroupMemberInfo& opUser, bool isAgreeJoin, const FString& opReason)
{
}

void UConversationListener::OnReceiveJoinApplication_Implementation(const FString& groupID, const FTIMGroupMemberInfo& member, const FString& opReason)
{
}

void UConversationListener::OnGroupInfoChanged_Implementation(const FString& groupID, const TArray<FTIMGroupChangeInfo>& changeInfos)
{
}

void UConversationListener::OnGroupRecycled_Implementation(const FString& groupID, const FTIMGroupMemberInfo& opUser)
{
}

void UConversationListener::OnGroupDismissed_Implementation(const FString& groupID, const FTIMGroupMemberInfo& opUser)
{
}

void UConversationListener::OnGroupCreated_Implementation(const FString& groupID)
{
}

void UConversationListener::OnMemberInfoChanged_Implementation(const FString& groupID, const TArray<FTIMGroupMemberChangeInfo>& v2TIMGroupMemberChangeInfoList)
{
}

void UConversationListener::OnMemberKicked_Implementation(const FString& groupID, const FTIMGroupMemberInfo& opUser, const TArray<FTIMGroupMemberInfo>& memberList)
{
}

void UConversationListener::OnMemberInvited_Implementation(const FString& groupID, const FTIMGroupMemberInfo& opUser, const TArray<FTIMGroupMemberInfo>& memberList)
{
}

void UConversationListener::OnMemberLeave_Implementation(const FString& groupID, const FTIMGroupMemberInfo& member)
{
}

void UConversationListener::OnMemberEnter_Implementation(const FString& groupID, const TArray<FTIMGroupMemberInfo>& memberList)
{
}

void UConversationListener::OnRecvMessageModified_Implementation(const FTIMMessage& message)
{
}

void UConversationListener::OnRecvMessageRevoked_Implementation(const FString& message)
{
}

void UConversationListener::OnReceiveNewMessage_Implementation(const FString& UserName, const FString& message)
{
}
