﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/FriendshipListener.h"

void UFriendshipListener::OnFriendApplicationListAdded_Implementation(const TArray<FTIMFriendApplication>& UserAPPList)
{
}

void UFriendshipListener::OnFriendListAdded_Implementation(const TArray<FTIMFriendInfo>& NewFriendList)
{
}

void UFriendshipListener::OnFriendListDeleted_Implementation(const TArray<FString>& DeleteFriendsList)
{
}

void UFriendshipListener::OnFriendInfoChanged_Implementation(const TArray<FTIMFriendInfo>& FriendsInfoChangeList)
{
}

void UFriendshipListener::OnFriendApplicationListDeleted_Implementation(const TArray<FString>& UserAPPDeleteList)
{
}

void UFriendshipListener::OnFriendApplicationListHasRead_Implementation()
{
}

void UFriendshipListener::OnBlackListAdded_Implementation(const TArray<FTIMFriendInfo>& NewBlackListList)
{
}

void UFriendshipListener::OnBlackListDeleted_Implementation(const TArray<FString>& BlackListDeletedList)
{
}