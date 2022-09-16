// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/FriendshipListenerWidget.h"

void UFriendshipListenerWidget::NativeConstruct()
{
	Super::NativeConstruct();
	//添加监听器
	V2TIMManager::GetInstance()->GetFriendshipManager()->AddFriendListener(this);
}

void UFriendshipListenerWidget::NativeDestruct()
{
	V2TIMManager::GetInstance()->GetFriendshipManager()->RemoveFriendListener(this);
	Super::NativeDestruct();
}

void UFriendshipListenerWidget::OnFriendApplicationListAdded_Implementation(const TArray<FTIMFriendApplication>& UserAPPList)
{
}

void UFriendshipListenerWidget::OnFriendListAdded_Implementation(const TArray<FTIMFriendInfo>& NewFriendList)
{
}

void UFriendshipListenerWidget::OnFriendListDeleted_Implementation(const TArray<FString>& DeleteFriendsList)
{
}

void UFriendshipListenerWidget::OnFriendInfoChanged_Implementation(const TArray<FTIMFriendInfo>& FriendsInfoChangeList)
{
}

void UFriendshipListenerWidget::OnFriendApplicationListDeleted_Implementation(const TArray<FString>& UserAPPDeleteList)
{
}

void UFriendshipListenerWidget::OnFriendApplicationListHasRead_Implementation()
{
}

void UFriendshipListenerWidget::OnBlackListAdded_Implementation(const TArray<FTIMFriendInfo>& NewBlackListList)
{
}

void UFriendshipListenerWidget::OnBlackListDeleted_Implementation(const TArray<FString>& BlackListDeletedList)
{
}

