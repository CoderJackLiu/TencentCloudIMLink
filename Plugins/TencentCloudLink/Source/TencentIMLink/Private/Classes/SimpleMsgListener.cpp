// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/SimpleMsgListener.h"

void USimpleMsgListener::OnRecvC2CTextMessage_Implementation(const FString& msgID, const FTIMUserFullInfo& sender, const FString& text)
{
	
}

void USimpleMsgListener::OnRecvC2CTextMessage(const V2TIMString& msgID, const V2TIMUserFullInfo& sender, const V2TIMString& text)
{
	V2TIMSimpleMsgListener::OnRecvC2CTextMessage(msgID, sender, text);
}

void USimpleMsgListener::OnRecvC2CCustomMessage(const V2TIMString& msgID, const V2TIMUserFullInfo& sender, const V2TIMBuffer& customData)
{
	V2TIMSimpleMsgListener::OnRecvC2CCustomMessage(msgID, sender, customData);
}

void USimpleMsgListener::OnRecvGroupTextMessage(const V2TIMString& msgID, const V2TIMString& groupID, const V2TIMGroupMemberFullInfo& sender, const V2TIMString& text)
{
	V2TIMSimpleMsgListener::OnRecvGroupTextMessage(msgID, groupID, sender, text);
}

void USimpleMsgListener::OnRecvGroupCustomMessage(const V2TIMString& msgID, const V2TIMString& groupID, const V2TIMGroupMemberFullInfo& sender, const V2TIMBuffer& customData)
{
	V2TIMSimpleMsgListener::OnRecvGroupCustomMessage(msgID, groupID, sender, customData);
	
}
