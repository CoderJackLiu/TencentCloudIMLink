// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/SimpleMsgListenerWidget.h"

#include "TencentIMLibrary.h"

void USimpleMsgListenerWidget::NativeConstruct()
{
	Super::NativeConstruct();
	V2TIMManager::GetInstance()->AddSimpleMsgListener(this);
}

void USimpleMsgListenerWidget::NativeDestruct()
{
	V2TIMManager::GetInstance()->RemoveSimpleMsgListener(this);
	Super::NativeDestruct();
}
void USimpleMsgListenerWidget::OnRecvC2CTextMessage_Implementation(const FString& msgID, const FTIMUserFullInfo& sender, const FString& text)
{
}

void USimpleMsgListenerWidget::OnRecvC2CCustomMessage_Implementation(const FString& msgID, const FTIMUserFullInfo& sender, const FBuffer& Buffer)
{
}

void USimpleMsgListenerWidget::OnRecvGroupTextMessage_Implementation(const FString& msgID, const FString& groupID, const FTIMGroupMemberFullInfo& sender, const FString& text)
{
}

void USimpleMsgListenerWidget::OnRecvGroupCustomMessage_Implementation(const FString& msgID, const FString& groupID, const FTIMGroupMemberFullInfo& sender, const FBuffer& customData)
{
}

void USimpleMsgListenerWidget::OnRecvC2CTextMessage(const V2TIMString& msgID, const V2TIMUserFullInfo& sender, const V2TIMString& text)
{
	V2TIMSimpleMsgListener::OnRecvC2CTextMessage(msgID, sender, text);
	OnRecvC2CTextMessage(UTencentIMLibrary::ToFString(msgID), UTencentIMLibrary::ToTIMUserFullInfo(sender), UTencentIMLibrary::ToFString(text));
}

void USimpleMsgListenerWidget::OnRecvC2CCustomMessage(const V2TIMString& msgID, const V2TIMUserFullInfo& sender, const V2TIMBuffer& customData)
{
	V2TIMSimpleMsgListener::OnRecvC2CCustomMessage(msgID, sender, customData);
	OnRecvC2CCustomMessage(UTencentIMLibrary::ToFString(msgID), UTencentIMLibrary::ToTIMUserFullInfo(sender), UTencentIMLibrary::ToBuffer(customData));
}

void USimpleMsgListenerWidget::OnRecvGroupTextMessage(const V2TIMString& msgID, const V2TIMString& groupID, const V2TIMGroupMemberFullInfo& sender, const V2TIMString& text)
{
	V2TIMSimpleMsgListener::OnRecvGroupTextMessage(msgID, groupID, sender, text);
	OnRecvGroupTextMessage(UTencentIMLibrary::ToFString(msgID), UTencentIMLibrary::ToFString(groupID), UTencentIMLibrary::ToTIMGroupMemberFullInfo(sender), UTencentIMLibrary::ToFString(text));
}

void USimpleMsgListenerWidget::OnRecvGroupCustomMessage(const V2TIMString& msgID, const V2TIMString& groupID, const V2TIMGroupMemberFullInfo& sender, const V2TIMBuffer& customData)
{
	V2TIMSimpleMsgListener::OnRecvGroupCustomMessage(msgID, groupID, sender, customData);
	OnRecvGroupCustomMessage(UTencentIMLibrary::ToFString(msgID), UTencentIMLibrary::ToFString(groupID), UTencentIMLibrary::ToTIMGroupMemberFullInfo(sender), UTencentIMLibrary::ToBuffer(customData));

}
