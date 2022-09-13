// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IM_Demo/TIMSDK/include/V2TIMListener.h"
#include "UObject/Object.h"
#include "Blueprint/UserWidget.h"

#include "CoreMinimal.h"
#include "TencentDataType.h"
#include "TencentIMLibrary.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/VerticalBox.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

#include "V2TIMBuffer.h"
#include "V2TIMConversationManager.h"
#include "V2TIMDefine.h"
#include "V2TIMManager.h"
#include "V2TIMMessage.h"
#include "V2TIMMessageManager.h"
#include "V2TIMGroupManager.h"
#include "V2TIMFriendshipManager.h"
#include "V2TIMSignaling.h"
#include "V2TIMSignalingManager.h"

#include "FriendConversation.generated.h"

/**
 * 
 */
UCLASS()
class TENCENTIMLINK_API UFriendConversation : public UUserWidget,public V2TIMAdvancedMsgListener
{
	GENERATED_BODY()

	
	//listener接口：
public:
	/**
	* 收到新消息
	*
	* @param message 消息
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnReceiveNewMessage(const FTIMMessage &message);
	
	
	virtual void OnRecvNewMessage(const V2TIMMessage &message) override
	{
		OnReceiveNewMessage(UTencentIMLibrary::ToMessage(message));
	}

	/**
	 * 收到 C2C 消息已读回执
	 *
	 * @param receiptList 已读回执列表
	 */
	virtual void OnRecvC2CReadReceipt(const V2TIMMessageReceiptVector &receiptList) override
	{
		
	}

	/**
	 * 收到消息撤回的通知
	 *
	 * @param messageID 消息唯一标识
	 */
	virtual void OnRecvMessageRevoked(const V2TIMString &messageID) override
	{
		
	}

	/**
	 * 消息内容被修改（第三方服务回调修改了消息内容）
	 */
	virtual void OnRecvMessageModified(const V2TIMMessage &message) override
	{
		
	}
};
