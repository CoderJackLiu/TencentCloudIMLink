// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/FriendConversation.h"



void UFriendConversation::NativeConstruct()
{
	Super::NativeConstruct();

	V2TIMManager::GetInstance()->GetMessageManager()->AddAdvancedMsgListener(this);
	
}

void UFriendConversation::NativeDestruct()
{
	Super::NativeDestruct();
}

void UFriendConversation::OnReceiveNewMessage_Implementation(const FString& UserName, const FString& message)
{
}
