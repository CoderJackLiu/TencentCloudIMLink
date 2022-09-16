// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/FriendConversationWidget.h"



void UFriendConversationWidget::NativeConstruct()
{
	Super::NativeConstruct();

	V2TIMManager::GetInstance()->GetMessageManager()->AddAdvancedMsgListener(this);
	
}

void UFriendConversationWidget::NativeDestruct()
{
	V2TIMManager::GetInstance()->GetMessageManager()->RemoveAdvancedMsgListener(this);

	Super::NativeDestruct();
}

void UFriendConversationWidget::OnRecvMessageModified_Implementation(const FTIMMessage& message)
{
}

void UFriendConversationWidget::OnRecvMessageRevoked_Implementation(const FString& message)
{
}

void UFriendConversationWidget::OnReceiveNewMessage_Implementation(const FString& UserName, const FString& message)
{
}
