// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/SimpleMsgListener.h"

#include "TencentIMLibrary.h"

void USimpleMsgListener::OnRecvC2CTextMessage_Implementation(const FString& msgID, const FTIMUserFullInfo& sender, const FString& text)
{
}

void USimpleMsgListener::OnRecvC2CCustomMessage_Implementation(const FString& msgID, const FTIMUserFullInfo& sender, const FBuffer& Buffer)
{
}

void USimpleMsgListener::OnRecvGroupTextMessage_Implementation(const FString& msgID, const FString& groupID, const FTIMGroupMemberFullInfo& sender, const FString& text)
{
}

void USimpleMsgListener::OnRecvGroupCustomMessage_Implementation(const FString& msgID, const FString& groupID, const FTIMGroupMemberFullInfo& sender, const FBuffer& customData)
{
}

void USimpleMsgListener::OnRecvC2CTextMessage(const V2TIMString& msgID, const V2TIMUserFullInfo& sender, const V2TIMString& text)
{
	V2TIMSimpleMsgListener::OnRecvC2CTextMessage(msgID, sender, text);
	OnRecvC2CTextMessage(UTencentIMLibrary::ToFString(msgID), UTencentIMLibrary::ToTIMUserFullInfo(sender), UTencentIMLibrary::ToFString(text));
}

void USimpleMsgListener::OnRecvC2CCustomMessage(const V2TIMString& msgID, const V2TIMUserFullInfo& sender, const V2TIMBuffer& customData)
{
	V2TIMSimpleMsgListener::OnRecvC2CCustomMessage(msgID, sender, customData);
	OnRecvC2CCustomMessage(UTencentIMLibrary::ToFString(msgID), UTencentIMLibrary::ToTIMUserFullInfo(sender), UTencentIMLibrary::ToBuffer(customData));
}

void USimpleMsgListener::OnRecvGroupTextMessage(const V2TIMString& msgID, const V2TIMString& groupID, const V2TIMGroupMemberFullInfo& sender, const V2TIMString& text)
{
	V2TIMSimpleMsgListener::OnRecvGroupTextMessage(msgID, groupID, sender, text);
	OnRecvGroupTextMessage(UTencentIMLibrary::ToFString(msgID), UTencentIMLibrary::ToFString(groupID), UTencentIMLibrary::ToTIMGroupMemberFullInfo(sender), UTencentIMLibrary::ToFString(text));
}

void USimpleMsgListener::OnRecvGroupCustomMessage(const V2TIMString& msgID, const V2TIMString& groupID, const V2TIMGroupMemberFullInfo& sender, const V2TIMBuffer& customData)
{
	V2TIMSimpleMsgListener::OnRecvGroupCustomMessage(msgID, groupID, sender, customData);
	OnRecvGroupCustomMessage(UTencentIMLibrary::ToFString(msgID), UTencentIMLibrary::ToFString(groupID), UTencentIMLibrary::ToTIMGroupMemberFullInfo(sender), UTencentIMLibrary::ToBuffer(customData));

}
