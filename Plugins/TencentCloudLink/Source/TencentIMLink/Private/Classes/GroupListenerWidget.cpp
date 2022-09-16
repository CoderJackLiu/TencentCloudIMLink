// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/GroupListenerWidget.h"
void UGroupListenerWidget::NativeConstruct()
{
	Super::NativeConstruct();
	V2TIMManager::GetInstance()->AddGroupListener(this);
}

void UGroupListenerWidget::NativeDestruct()
{
	V2TIMManager::GetInstance()->RemoveGroupListener(this);
	Super::NativeDestruct();
}
void UGroupListenerWidget::OnMemberEnter_Implementation(const FString& groupID, const TArray<FTIMGroupMemberInfo>& memberList)
{
}

void UGroupListenerWidget::OnMemberLeave_Implementation(const FString& groupID, const FTIMGroupMemberInfo& member)
{
}

void UGroupListenerWidget::OnMemberInvited_Implementation(const FString& groupID, const FTIMGroupMemberInfo& opUser, const TArray<FTIMGroupMemberInfo>& memberList)
{
}

void UGroupListenerWidget::OnMemberKicked_Implementation(const FString& groupID, const FTIMGroupMemberInfo& opUser, const TArray<FTIMGroupMemberInfo>& memberList)
{
}

void UGroupListenerWidget::OnMemberInfoChanged_Implementation(const FString& groupID, const TArray<FTIMGroupMemberChangeInfo>& v2TIMGroupMemberChangeInfoList)
{
}

void UGroupListenerWidget::OnGroupCreated_Implementation(const FString& groupID)
{
}

void UGroupListenerWidget::OnGroupDismissed_Implementation(const FString& groupID, const FTIMGroupMemberInfo& opUser)
{
}

void UGroupListenerWidget::OnGroupRecycled_Implementation(const FString& groupID, const FTIMGroupMemberInfo& opUser)
{
}

void UGroupListenerWidget::OnGroupInfoChanged_Implementation(const FString& groupID, const TArray<FTIMGroupChangeInfo>& changeInfos)
{
}

void UGroupListenerWidget::OnReceiveJoinApplication_Implementation(const FString& groupID, const FTIMGroupMemberInfo& member, const FString& opReason)
{
}

void UGroupListenerWidget::OnApplicationProcessed_Implementation(const FString& groupID, const FTIMGroupMemberInfo& opUser, bool isAgreeJoin, const FString& opReason)
{
	
}

void UGroupListenerWidget::OnGrantAdministrator_Implementation(const FString& groupID, const FTIMGroupMemberInfo& opUser, const TArray<FTIMGroupMemberInfo>& memberList)
{
}

void UGroupListenerWidget::OnRevokeAdministrator_Implementation(const FString& groupID, const FTIMGroupMemberInfo& opUser, const TArray<FTIMGroupMemberInfo>& memberList)
{
}

void UGroupListenerWidget::OnQuitFromGroup_Implementation(const FString& groupID)
{
}

void UGroupListenerWidget::OnReceiveRESTCustomData_Implementation(const FString& groupID, const FBuffer& customData)
{
}

void UGroupListenerWidget::OnGroupAttributeChanged_Implementation(const FString& groupID, const TMap<FString, FString>& groupAttributeMap)
{
}


